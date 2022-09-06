// Copyright (C) 2022 Numendacil and contributors
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef _MIRAI_CLIENT_HPP_
#define _MIRAI_CLIENT_HPP_

#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <functional>
#include <limits>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Events/BotInvitedJoinGroupRequestEvent.hpp>
#include <libmirai/Events/FriendMessageEvent.hpp>
#include <libmirai/Events/GroupMessageEvent.hpp>
#include <libmirai/Events/MemberJoinRequestEvent.hpp>
#include <libmirai/Events/MiraiClientEvents.hpp>
#include <libmirai/Events/NewFriendRequestEvent.hpp>
#include <libmirai/Events/StrangerMessageEvent.hpp>
#include <libmirai/Events/TempMessageEvent.hpp>
#include <libmirai/Messages/MessageChain.hpp>
#include <libmirai/Types/Types.hpp>
#include <libmirai/Utils/Logger.hpp>
#include <libmirai/Utils/SessionConfig.hpp>

/// 所有mirai相关的对象的命名空间
namespace Mirai
{

namespace Details
{

class HttpClientImpl;
class MessageClientImpl;

} // namespace Details

namespace Utils
{

class ThreadPool;

}

/**
 * @brief 提供与mirai-api-http网络交互的封装
 * 
 * 所有与mirai的交互都需要通过该类来实现。使用时可以通过调用http
 * api以及注册事件回调函数来完成消息的处理与发送。
 *
 * 连接设置保存在 `SessionConfigs` 类中，更改设置后需要重新连接mirai来使新配置生效。
 *
 * 所有非常量方法均有可能会抛出异常，异常的种类见 `Exceptions.hpp`。
 */
class MiraiClient
{
public:
	template<typename Event> using EventCallback = std::function<void(Event)>;

protected:
	using EventHandler = std::function<void(const nlohmann::json&)>;

	mutable std::mutex _mtx;
	mutable std::condition_variable _cv;

	SessionConfigs _config{};

	std::shared_ptr<ILogger> _logger;

	std::string _SessionKey{};
	bool _SessionKeySet = false;
	bool _connected = false; // Probably can be removed, but just in case

	std::unique_ptr<Details::HttpClientImpl> _HttpClient;
	std::unique_ptr<Details::MessageClientImpl> _MessageClient;

	std::unique_ptr<Utils::ThreadPool> _ThreadPool;

	ClientConnectionEstablishedEvent _HandshakeInfo{};

	EventCallback<ClientConnectionEstablishedEvent> _ConnectionEstablishedCallback;
	EventCallback<ClientConnectionClosedEvent> _ConnectionClosedCallback;
	EventCallback<ClientConnectionErrorEvent> _ConnectionErrorCallback;
	EventCallback<ClientParseErrorEvent> _ParseErrorCallback;

	std::unordered_map<std::string, EventHandler> _EventHandlers{};

	bool _ReadSessionKey(const nlohmann::json& data);
	void _DispatchEvent(const nlohmann::json& data);

	template<typename T> class _has_type_
	{
		using yes_type = char;
		using no_type = long;
		template<typename U> static yes_type test(decltype(&U::_TYPE_));
		template<typename U> static no_type test(...);

	public:
		static constexpr bool value = sizeof(test<T>(0)) == sizeof(yes_type);
	};

	template<typename Event> constexpr static void _type_check_()
	{
		static_assert(std::is_base_of<EventBase, Event>::value, "Event must be a derived class of MessageBase");
		static_assert(_has_type_<Event>::value, "Event must contain a static atrribute _TYPE_");
	};

	EventCallback<ClientConnectionEstablishedEvent> _GetEstablishedCallback() const
	{
		std::lock_guard<std::mutex> lk(this->_mtx);
		return this->_ConnectionEstablishedCallback;
	}

	EventCallback<ClientConnectionClosedEvent> _GetClosedCallback() const
	{
		std::lock_guard<std::mutex> lk(this->_mtx);
		return this->_ConnectionClosedCallback;
	}

	EventCallback<ClientConnectionErrorEvent> _GetErrorCallback() const
	{
		std::lock_guard<std::mutex> lk(this->_mtx);
		return this->_ConnectionErrorCallback;
	}

	EventCallback<ClientParseErrorEvent> _GetParseErrorCallback() const
	{
		std::lock_guard<std::mutex> lk(this->_mtx);
		return this->_ParseErrorCallback;
	}

	ILogger& _GetLogger() const { return *(this->_logger); }

public:
	MiraiClient();
	MiraiClient(SessionConfigs config);
	MiraiClient(const MiraiClient&) = delete;
	MiraiClient& operator=(const MiraiClient&) = delete;
	MiraiClient(MiraiClient&& rhs) noexcept;
	MiraiClient& operator=(MiraiClient&& rhs) noexcept;
	~MiraiClient();

	/// 获取连接mirai-api-http的session key，若尚未建立链接则返回 `std::nullopt`
	std::optional<std::string> GetSessionKey() const
	{
		return (this->_SessionKeySet) ? std::optional<std::string>(this->_SessionKey) : std::nullopt;
	}

	/// 获取连接配置
	SessionConfigs GetSessionConfig() const { return this->_config; }

	/// 设置日志记录类
	void SetLogger(std::shared_ptr<ILogger> logger) { this->_logger = logger; }

	/// 获取日志记录类
	std::shared_ptr<ILogger> GetLogger() const { return this->_logger; }

	/// 获取BotQQ账号
	QQ_t GetBotQQ() const { return this->_config.BotQQ; }

	/// 返回是否已成功连接mirai-api-http
	bool isConnected() const { return this->_connected && this->_SessionKeySet; }

	/// 返回cpp-mirai-client的版本号
	constexpr std::string_view GetVersion() { return CPP_MIRAI_CLIENT_VERSION; }

	/**
	 * @brief 注册事件回调函数
	 * 
	 * 模版参数 `EventType` 必须为 `EventBase` 的派生类，且定义了 `EventType::_TYPE_` 属性
	 * @tparam EventType 事件类型
	 * @param callback 回调函数
	 */
	template<typename EventType> void On(EventCallback<EventType> callback)
	{
		_type_check_<EventType>();
		std::lock_guard<std::mutex> lk(this->_mtx);
		this->_EventHandlers[std::string(EventType::_TYPE_)] = [callback, this](const nlohmann::json& j)
		{
			EventType event(this);
			event.FromJson(j);
			callback(event);
		};
	}


	/**
	 * @brief 设置连接选项
	 * 
	 * @param config 连接配置
	 */
	void SetSessionConfig(const SessionConfigs& config) { this->_config = config; };

	/**
	 * @brief 设置连接选项
	 * 
	 * @param path 配置文件(JSON格式)的路径
	 */
	void SetSessionConfig(const std::string& path) { this->_config.FromFile(path); };

	/**
	 * @brief 设置连接选项
	 * 
	 * @param json_config JSON格式的配置
	 */
	void SetSessionConfig(const nlohmann::json& json_config) { this->_config.FromJson(json_config); };


	/**
	 * @brief 连接mirai-api-http
	 * 
	 * 若成功返回说明已成功建立连接，否则会抛出异常。
	 * `Connect()` 返回前会调用 `ClientConnectionEstablishedEvent` 的回调函数
	 */
	void Connect();

	/**
	 * @brief 断开与mirai-api-http的连接
	 * 
	 * 若成功返回说明已成功断开连接，否则会抛出异常。
	 * `Disconnect()` 返回前会调用 `ClientConnectionClosedEvent` 的回调函数，并等待所有正在运行中的事件回调函数结束。
	 */
	void Disconnect();


	using string = std::string;
	using json = nlohmann::json;

	/**
	 * @brief 获取mirai-api-http插件的版本号
	 * 
	 * @return  mirai-api-http插件的版本号 “x.x.x”
	 */
	string GetMiraiApiHttpVersion();

	/**
	 * @brief 获取mirai中可用的QQBot列表
	 * 
	 * @return `std::vector<QQ_t>`
	 */
	std::vector<QQ_t> GetBotList();

	/**
	 * @brief 从消息id获取好友消息
	 * 
	 * @param id 消息id
	 * @param qq 好友QQ
	 * @return `FriendMessageEvent`
	 */
	FriendMessageEvent GetFriendMessage(MessageId_t id, QQ_t qq);

	/**
	 * @brief 从消息id获取群聊消息
	 * 
	 * @param id 消息id
	 * @param GroupId 群聊id
	 * @return `GroupMessageEvent` 
	 */
	GroupMessageEvent GetGroupMessage(MessageId_t id, GID_t GroupId);

	/**
	 * @brief 从消息id获取临时消息
	 * 
	 * @param id 消息id
	 * @param GroupId 群聊id
	 * @return `TempMessageEvent` 
	 */
	TempMessageEvent GetTempMessage(MessageId_t id, GID_t GroupId);

	/**
	 * @brief 从消息id获取陌生人消息
	 * 
	 * @param id 消息id
	 * @param qq 陌生人QQ
	 * @return `StrangerMessageEvent` 
	 */
	StrangerMessageEvent GetStrangerMessage(MessageId_t id, QQ_t qq);

	/**
	 * @brief 获取好友列表
	 * 
	 * @return `std::vector<User>` 
	 */
	std::vector<User> GetFriendList();

	/**
	 * @brief 获取群聊列表
	 * 
	 * @return `std::vector<Group>` 
	 */
	std::vector<Group> GetGroupList();

	/**
	 * @brief 获取群成员列表
	 * 
	 * @param GroupId 群聊id
	 * @return `std::vector<GroupMember>` 
	 */
	std::vector<GroupMember> GetMemberList(GID_t GroupId);

	/**
	 * @brief 获取Bot用户资料
	 * 
	 * @return `UserProfile` 
	 */
	UserProfile GetBotProfile();

	/**
	 * @brief 获取好友用户资料
	 * 
	 * @param qq 好友QQ
	 * @return `UserProfile` 
	 */
	UserProfile GetFriendProfile(QQ_t qq);

	/**
	 * @brief 获取群成员用户资料
	 * 
	 * @param GroupId 群聊id
	 * @param MemberId 群成员QQ
	 * @return `UserProfile` 
	 */
	UserProfile GetMemberProfile(GID_t GroupId, QQ_t MemberId);

	/**
	 * @brief 获取用户资料
	 * 
	 * @param qq 用户QQ
	 * @return `UserProfile` 
	 */
	UserProfile GetUserProfile(QQ_t qq);

	/**
	 * @brief 发送好友消息
	 * 
	 * @param qq 好友QQ
	 * @param message 消息内容
	 * @param QuoteId 引用回复内容的消息id
	 * @param ignoreInvalid 是否忽略无效消息，见 `MessageChain::ToJson()`
	 * @return 发送的消息id
	 */
	MessageId_t SendFriendMessage(QQ_t qq, const MessageChain& message,
	                              std::optional<MessageId_t> QuoteId = std::nullopt, bool ignoreInvalid = false);

	/**
	 * @brief 发送群聊消息
	 * 
	 * @param GroupId 群聊id
	 * @param message 消息内容
	 * @param QuoteId 引用回复内容的消息id
	 * @param ignoreInvalid 是否忽略无效消息，见 `MessageChain::ToJson()`
	 * @return 发送的消息id
	 */
	MessageId_t SendGroupMessage(GID_t GroupId, const MessageChain& message,
	                             std::optional<MessageId_t> QuoteId = std::nullopt, bool ignoreInvalid = false);

	/**
	 * @brief 发送临时会话消息
	 * 
	 * @param MemberId 群成员id
	 * @param GroupId 群聊id
	 * @param message 消息内容
	 * @param QuoteId 引用回复内容的消息id
	 * @param ignoreInvalid 是否忽略无效消息，见 `MessageChain::ToJson()`
	 * @return 发送的消息id
	 */
	MessageId_t SendTempMessage(QQ_t MemberId, GID_t GroupId, const MessageChain& message,
	                            std::optional<MessageId_t> QuoteId = std::nullopt, bool ignoreInvalid = false);

	/**
	 * @brief 发送头像戳一戳消息
	 * 
	 * @param target 戳一戳目标
	 */
	void SendNudge(const NudgeTarget& target);

	/**
	 * @brief 发送好友戳一戳消息
	 * 
	 * @param qq 好友QQ
	 */
	void NudgeFriend(QQ_t qq);

	/**
	 * @brief 发送群聊戳一戳消息
	 * 
	 * @param MemberId 戳一戳对象QQ
	 * @param GroupId 群聊id
	 */
	void NudgeGroup(QQ_t MemberId, GID_t GroupId);

	/**
	 * @brief 发送陌生人戳一戳消息
	 * 
	 * @param qq 陌生人QQ
	 */
	void NudgeStranger(QQ_t qq);

	/**
	 * @brief 撤回好友消息
	 * 
	 * @param id 消息id
	 * @param qq 好友QQ
	 */
	void RecallFriendMessage(MessageId_t id, QQ_t qq);

	/**
	 * @brief 撤回群聊消息
	 * 
	 * @param id 消息id
	 * @param GroupId 群聊id
	 */
	void RecallGroupMessage(MessageId_t id, GID_t GroupId);

	/**
	 * @brief 获取好友漫游消息
	 * 
	 * @param qq 好友QQ
	 * @param TimeStart 消息起始时间
	 * @param TimeEnd 消息结束时间
	 * @return `std::vector<MessageChain>` 
	 */
	std::vector<MessageChain> GetRoamingFriendMessage(QQ_t qq, std::time_t TimeStart = 0,
	                                                  std::time_t TimeEnd = std::numeric_limits<std::time_t>::max());

	/**
	 * @brief 获取群文件列表
	 * 
	 * @param GroupId 群聊id
	 * @param dir 父文件夹路径，默认为根目录
	 * @param offset 分页偏移
	 * @param size 分页大小，默认为不分页（即返回所有结果）
	 * @param withDownloadInfo 是否附带下载地址
	 * @return `std::vector<GroupFileInfo>`
	 */
	std::vector<GroupFileInfo> GetGroupFileList(GID_t GroupId, const FilePath& dir = {}, int64_t offset = 0,
	                                            int64_t size = 0, bool withDownloadInfo = false);

	/**
	 * @brief 获取群文件信息
	 * 
	 * @param GroupId 群聊id
	 * @param dir 群文件路径
	 * @param withDownloadInfo 是否附带下载地址
	 * @return `GroupFileInfo` 
	 */
	GroupFileInfo GetGroupFileInfo(GID_t GroupId, const FilePath& dir, bool withDownloadInfo = false);

	/**
	 * @brief 获取群文件信息
	 * 
	 * @param GroupId 群聊id
	 * @param file 群文件信息，会从中读取路径并把剩余信息填入
	 * @param withDownloadInfo 是否附带下载地址
	 */
	void GetGroupFileInfo(GID_t GroupId, GroupFileInfo& file, bool withDownloadInfo = false);

	/**
	 * @brief 创建群文件夹
	 * 
	 * 目前QQ仅支持在根目录下创建文件夹，不许文件夹嵌套
	 * @param GroupId 群聊id
	 * @param directory 文件夹命名
	 * @return 新建的文件夹信息
	 */
	GroupFileInfo CreateGroupFileDirectory(GID_t GroupId, const string& directory);

	/**
	 * @brief 删除群文件
	 * 
	 * @param GroupId 群聊id
	 * @param dir 群文件路径
	 */
	void RemoveGroupFile(GID_t GroupId, const FilePath& dir);

	/**
	 * @brief 移动群文件
	 * 
	 * @param GroupId 群聊id
	 * @param FileDir 群文件路径
	 * @param MoveToDir 目标文件夹路径
	 */
	void MoveGroupFile(GID_t GroupId, const FilePath& FileDir, const FilePath& MoveToDir);

	/**
	 * @brief 重命名群文件
	 * 
	 * @param GroupId 群聊id
	 * @param FileDir 群文件路径
	 * @param NewName 新文件名
	 */
	void RenameGroupFile(GID_t GroupId, const FilePath& FileDir, const string& NewName);

	/**
	 * @brief 上传群文件
	 * 
	 * @param GroupId 群聊id
	 * @param UploadPath 上传路径
	 * @param name 文件名称
	 * @param content 文件内容
	 * @return 上传的群文件信息 
	 */
	GroupFileInfo UploadGroupFile(GID_t GroupId, const string& UploadPath, const string& name, const string& content);

	/**
	 * @brief 上传群文件
	 * 
	 * @param GroupId 群聊id
	 * @param UploadPath 上传路径
	 * @param name 文件名称
	 * @param file 文件流
	 * @return 上传的群文件信息 
	 */
	GroupFileInfo UploadGroupFile(GID_t GroupId, const string& UploadPath, const string& name, std::istream& file);

	/**
	 * @brief 上传群文件
	 * 
	 * @param GroupId 群聊id
	 * @param UploadPath 上传路径
	 * @param path 本地文件路径
	 * @return 上传的群文件信息 
	 */
	GroupFileInfo UploadGroupFile(GID_t GroupId, const string& UploadPath, const std::filesystem::path& path);

	/**
	 * @brief 上传好友图片
	 * 
	 * @param content 图片文件内容（原始二进制，不是base64编码）
	 * @return `FriendImage` 
	 */
	FriendImage UploadFriendImage(const string& content);
	/**
	 * @brief 上传好友图片
	 * 
	 * @param file 图片文件流
	 * @return `FriendImage` 
	 */
	FriendImage UploadFriendImage(std::istream& file);

	/**
	 * @brief 上传群聊图片
	 * 
	 * @param content 图片文件内容（原始二进制，不是base64编码）
	 * @return `GroupImage` 
	 */
	GroupImage UploadGroupImage(const string& content);
	/**
	 * @brief 上传群聊图片
	 * 
	 * @param file 图片文件流
	 * @return `GroupImage` 
	 */
	GroupImage UploadGroupImage(std::istream& file);

	/**
	 * @brief 上传临时会话图片
	 * 
	 * @param content 图片文件内容（原始二进制，不是base64编码）
	 * @return `TempImage` 
	 */
	TempImage UploadTempImage(const string& content);
	/**
	 * @brief 上传临时会话图片
	 * 
	 * @param file 图片文件流
	 * @return `TempImage` 
	 */
	TempImage UploadTempImage(std::istream& file);

	/**
	 * @brief 上传群聊语音
	 * 
	 * @param content 语音文件内容（原始二进制，不是base64编码）
	 * @return `GroupAudio` 
	 */
	GroupAudio UploadGroupAudio(const string& content);
	/**
	 * @brief 上传群聊语音
	 * 
	 * @param file 语音文件流
	 * @return `GroupAudio` 
	 */
	GroupAudio UploadGroupAudio(std::istream& file);

	/**
	 * @brief 删除好友
	 * 
	 * @param qq 好友QQ
	 */
	void DeleteFriend(QQ_t qq);

	/**
	 * @brief 禁言群成员
	 * 
	 * @param GroupId 群聊id
	 * @param member 成员QQ
	 * @param time 禁言时间
	 */
	void Mute(GID_t GroupId, QQ_t member, std::chrono::seconds time);
	/**
	 * @brief 禁言群成员
	 * 
	 * @param member 群成员
	 * @param time 禁言时间
	 */
	void Mute(const GroupMember& member, std::chrono::seconds time);

	/**
	 * @brief 解除群成员禁言
	 * 
	 * @param GroupId 群聊id
	 * @param member 成员QQ
	 */
	void Unmute(GID_t GroupId, QQ_t member);
	/**
	 * @brief 解除群成员禁言
	 * 
	 * @param member 群成员
	 */
	void Unmute(const GroupMember& member);

	/**
	 * @brief 移除群成员
	 * 
	 * @param GroupId 群聊id
	 * @param member 成员QQ
	 * @param message 附带信息
	 */
	void Kick(GID_t GroupId, QQ_t member, const string& message);
	/**
	 * @brief 移除群成员
	 * 
	 * @param member 群成员
	 * @param message 附带信息
	 */
	void Kick(const GroupMember& member, const string& message);

	/**
	 * @brief 退出群聊
	 * 
	 * @param GroupId 群聊id
	 */
	void LeaveGroup(GID_t GroupId);

	/**
	 * @brief 禁言全体成员
	 * 
	 * @param GroupId 群聊id
	 */
	void MuteAll(GID_t GroupId);

	/**
	 * @brief 解除全体禁言
	 * 
	 * @param GroupId 群聊id
	 */
	void UnmuteAll(GID_t GroupId);

	/**
	 * @brief 设置群精华消息
	 * 
	 * @param GroupId 群聊id
	 * @param MessageId 消息id
	 */
	void SetEssence(GID_t GroupId, MessageId_t MessageId);

	/**
	 * @brief 获取群设置
	 * 
	 * @param GroupId 群聊id
	 * @return `GroupConfig` 
	 */
	GroupConfig GetGroupConfig(GID_t GroupId);

	/**
	 * @brief 修改群设置
	 * 
	 * 目前仅能修改群名称与是否允许邀请入群
	 * @param GroupId 群聊id
	 * @param name 群聊名称，默认保持原设置
	 * @param AllowMemberInvite 是否允许邀请入群，默认保持原设置
	 */
	void SetGroupConfig(GID_t GroupId, const string& name = "", std::optional<bool> AllowMemberInvite = std::nullopt);

	/**
	 * @brief 获取群成员资料
	 * 
	 * @param GroupId 群聊id
	 * @param member 成员QQ
	 * @return `GroupMember` 
	 */
	GroupMember GetMemberInfo(GID_t GroupId, QQ_t member);

	/**
	 * @brief 设置群成员资料
	 * 
	 * @param GroupId 群聊id
	 * @param member 成员QQ
	 * @param name 群名片
	 * @param title 群头衔
	 */
	void SetMemberInfo(GID_t GroupId, QQ_t member, const string& name = "", const string& title = "");

	/**
	 * @brief 设置群管理员
	 * 
	 * @param GroupId 群聊id
	 * @param member 成员QQ
	 * @param assign 设置管理员/撤销管理员
	 */
	void SetGroupAdmin(GID_t GroupId, QQ_t member, bool assign = true);

	/**
	 * @brief 获取群公告列表
	 * 
	 * @param GroupId 群聊id
	 * @param offset 分页偏移
	 * @param size 分页大小，默认为不分页（即返回所有结果）
	 * @return `std::vector<GroupAnnouncement>` 
	 */
	std::vector<GroupAnnouncement> GetAnnouncementList(GID_t GroupId, int64_t offset = 0, int64_t size = 0);

	/**
	 * @brief 发布群公告
	 * 
	 * @param GroupId 群聊id
	 * @param content 公告内容
	 * @param cover 公告图片，传空值为不附带图片
	 * @param ToNewMember 是否发给新成员
	 * @param pinned 是否置顶
	 * @param ShowEditCard 是否引导群成员修改群名片
	 * @param ShowPopup 是否弹出公告
	 * @param RequireConfirm 是否需要群员确认
	 * @return 发布的群公告 
	 */
	GroupAnnouncement PublishAnnouncement(GID_t GroupId, const string& content, const MiraiImage& cover = {},
	                                      bool ToNewMember = false, bool pinned = false, bool ShowEditCard = false,
	                                      bool ShowPopup = false, bool RequireConfirm = false);

	/**
	 * @brief 删除群公告
	 * 
	 * @param GroupId 群聊id
	 * @param fid 群公告id
	 */
	void DeleteAnnouncement(GID_t GroupId, const string& fid);
	/**
	 * @brief 删除群公告
	 * 
	 * @param announcement 群公告对象
	 */
	void DeleteAnnouncement(const GroupAnnouncement& announcement);

	/**
	 * @brief 处理添加好友申请事件 `NewFriendRequestEvent`
	 * 
	 * @param EventId 事件id
	 * @param FromId 申请人qq
	 * @param GroupId 申请人来自的群聊id，可能为 `0_gid`
	 * @param operation 处理操作
	 * @param message 回复消息
	 */
	void RespNewFriendRequestEvent(int64_t EventId, QQ_t FromId, GID_t GroupId, NewFriendRequestOp operation,
	                               const string& message);
	/**
	 * @brief 处理添加好友申请事件 `NewFriendRequestEvent`
	 * 
	 * @param event 好友申请事件
	 * @param operation 处理操作
	 * @param message 回复消息
	 */
	void RespNewFriendRequestEvent(const NewFriendRequestEvent& event, NewFriendRequestOp operation,
	                               const string& message);

	/**
	 * @brief 处理用户申请入群事件 `MemberJoinRequestEvent`
	 * 
	 * @param EventId 事件id
	 * @param FromId 申请人qq
	 * @param GroupId 群聊id
	 * @param operation 处理操作
	 * @param message 回复消息
	 */
	void RespMemberJoinRequestEvent(int64_t EventId, QQ_t FromId, GID_t GroupId, MemberJoinRequestOp operation,
	                                const string& message);
	/**
	 * @brief 处理用户申请入群事件 `MemberJoinRequestEvent`
	 * 
	 * @param event 用户申请入群事件
	 * @param operation 处理操作
	 * @param message 回复消息
	 */
	void RespMemberJoinRequestEvent(const MemberJoinRequestEvent& event, MemberJoinRequestOp operation,
	                                const string& message);

	/**
	 * @brief 处理Bot被邀请入群事件 `BotInvitedJoinGroupRequestEvent`
	 * 
	 * @param EventId 事件id
	 * @param FromId 申请人qq
	 * @param GroupId 群聊id
	 * @param operation 处理操作
	 * @param message 回复消息
	 */
	void RespBotInvitedJoinGroupRequestEvent(int64_t EventId, QQ_t FromId, GID_t GroupId,
	                                         BotInvitedJoinGroupRequestOp operation, const string& message);
	/**
	 * @brief 处理Bot被邀请入群事件 `BotInvitedJoinGroupRequestEvent`
	 * 
	 * @param event Bot被邀请入群事件
	 * @param operation 处理操作
	 * @param message 回复消息
	 */
	void RespBotInvitedJoinGroupRequestEvent(const BotInvitedJoinGroupRequestEvent& event,
	                                         BotInvitedJoinGroupRequestOp operation, const string& message);

	/**
	 * @brief 注册指令
	 * 
	 * @param name 指令名称
	 * @param alias 指令别名
	 * @param usage 使用说明
	 * @param description 指令描述
	 */
	void RegisterCommand(const string& name, const std::vector<string>& alias, const string& usage,
	                     const string& description);

	/**
	 * @brief 执行指令
	 * 
	 * @param command 指令内容
	 */
	void ExecuteCommand(const MessageChain& command);

	/**
	 * @brief 直接向mirai-api-http发送POST请求
	 * 
	 * 备用方法，可用于hack新功能或提供临时替代，正常情况下不推荐使用
	 * @param path 路径
	 * @param content POST内容
	 * @param ContentType 内容格式
	 * @return `nlohman::json`  
	 */
	json PostRaw(const string& path, const string& content, const string& ContentType);

	/**
	 * @brief 直接向mirai-api-http发送GET请求
	 * 
	 * @param path 路径
	 * @param params query参数
	 * @return `nlohman::json` 
	 */
	json GetRaw(const string& path, const std::multimap<string, string> params);
};

template<>
void MiraiClient::On<ClientConnectionEstablishedEvent>(EventCallback<ClientConnectionEstablishedEvent> callback);
template<> void MiraiClient::On<ClientConnectionErrorEvent>(EventCallback<ClientConnectionErrorEvent> callback);
template<> void MiraiClient::On<ClientConnectionClosedEvent>(EventCallback<ClientConnectionClosedEvent> callback);
template<> void MiraiClient::On<ClientParseErrorEvent>(EventCallback<ClientParseErrorEvent> callback);

} // namespace Mirai

#endif