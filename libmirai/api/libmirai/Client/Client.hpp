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

#ifndef MIRAI_CLIENT_HPP_
#define MIRAI_CLIENT_HPP_

#include <filesystem>
#include <fstream>
#include <functional>
#include <limits>
#include <memory>
#include <optional>
#include <shared_mutex>
#include <string>
#include <thread>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

#include <libmirai/Messages/MessageChain.hpp>
#include <libmirai/Types/Types.hpp>
#include <libmirai/Utils/Logger.hpp>
#include <libmirai/Events/Events.hpp>

#include <libmirai/Adaptors/IAdaptor.hpp>

/// 所有mirai相关的对象的命名空间
namespace Mirai
{

namespace Utils
{

class ThreadPool;

}

/**
 * @brief 提供与mirai-api-http网络交互的封装
 * 
 * 所有与mirai的交互都需要通过该类来实现。使用时可以通过调用网络api
 * 以及注册事件回调函数来完成消息的处理与发送。
 *
 * 连接设置保存在 `SessionConfigs` 类中，更改设置后需要重新连接mirai来使新配置生效。
 *
 * 所有非常量方法均有可能会抛出异常，异常的种类见 `Exceptions.hpp`。
 */
class MiraiClient
{
private:
	// type traits
	struct traits
	{
		template<typename T, typename TypeList> struct is_event_type_;

		template<typename T, size_t... I>
		struct is_event_type_<T, std::index_sequence<I...>> : public std::disjunction<std::is_same<T, GetEventType_t<EventTypesList[I]>>...>
		{
		};

		template<typename T>
		using is_event_type = is_event_type_<T, std::make_index_sequence<EventTypesList.size()>>;

		template<typename T>
		struct event_callback_variant_ {};

		template<size_t... I>
		struct event_callback_variant_<std::index_sequence<I...>> { using type = std::variant<std::function<void(GetEventType_t<EventTypesList[I]>)>...>; };

		using EventCallbackVariant = event_callback_variant_<std::make_index_sequence<EventTypesList.size()>>::type;

		template<typename T> 
		struct function_traits;

		template<typename EventType> 
		struct function_traits<std::function<void(EventType)>>
		{
			using type = EventType;
		};
	};

	template<typename EventType> constexpr static void type_check_()
	{
		static_assert(std::is_base_of_v<IEvent<EventType>, EventType>,
		              "EventType is not derived from IEvent<EventType>"); // NOLINT(*-array-to-pointer-decay)
		static_assert(traits::is_event_type<EventType>::value,
		              "Unsupported event type"); // NOLINT(*-array-to-pointer-decay)
	};

public:
	template<typename Event> 
	using EventCallback = std::function<void(Event)>;

protected:
	// For locking session key
	mutable std::shared_mutex mtx_;

	std::string SessionKey_{};
	bool connected_ = false;
	size_t PoolSize_ = std::thread::hardware_concurrency();

	std::shared_ptr<ILogger> logger_ = std::make_shared<NullLogger>();
	std::unique_ptr<IAdaptor> adaptor_;
	std::unique_ptr<Utils::ThreadPool> pool_;

	EventCallback<ClientConnectionEstablishedEvent> ConnectionEstablishedCallback_;
	EventCallback<ClientConnectionClosedEvent> ConnectionClosedCallback_;
	EventCallback<ClientConnectionErrorEvent> ConnectionErrorCallback_;
	EventCallback<ClientParseErrorEvent> ParseErrorCallback_;

	using EventHandler = traits::EventCallbackVariant;
	std::unordered_map<EventTypes, EventHandler> EventHandlers_{};

	std::string GetSessionKey_() const
	{
		std::shared_lock<std::shared_mutex> lk(this->mtx_);
		return this->SessionKey_;
	}

public:
	MiraiClient();
	MiraiClient(std::unique_ptr<IAdaptor> adaptor);
	MiraiClient(size_t PoolSize, std::unique_ptr<IAdaptor> adaptor);

	template <typename Adaptor, typename... Args, typename std::enable_if_t<std::is_base_of_v<IAdaptor, Adaptor>, int> = 0>
	MiraiClient(size_t PoolSize, Args&&... args) 
	: MiraiClient(PoolSize, std::make_unique<Adaptor>(std::forward<Args>(args)...))
	{} 

	MiraiClient(const MiraiClient&) = delete;
	MiraiClient& operator=(const MiraiClient&) = delete;
	MiraiClient(MiraiClient&& rhs) noexcept;
	MiraiClient& operator=(MiraiClient&& rhs) noexcept;
	~MiraiClient();

	/// 获取连接mirai-api-http的session key，若尚未建立链接则返回 `std::nullopt`
	std::optional<std::string> GetSessionKey() const
	{
		std::shared_lock<std::shared_mutex> lk(this->mtx_);
		return (this->connected_) ? std::optional<std::string>(this->SessionKey_) : std::nullopt;
	}


	/**
	 * @brief 设置适配器
	 * 
	 * @param adaptor 
	 * @return 原先的适配器，`std::unique_ptr<IAdaptor>`
	 */
	///@{
	
	std::unique_ptr<IAdaptor> SetAdaptor(std::unique_ptr<IAdaptor> adaptor)
	{
		std::unique_ptr<IAdaptor> ret = std::move(this->adaptor_);
		this->adaptor_ = std::move(adaptor);
		return ret;
	}

	template <typename Adaptor, typename... Args, typename std::enable_if_t<std::is_base_of_v<IAdaptor, Adaptor>, int> = 0>
	std::unique_ptr<IAdaptor> SetAdaptor(Args&&... args)
	{
		return this->SetAdaptor(std::make_unique<Adaptor>(std::forward<Args>(args)...));
	}

	///@}


	/// 设置日志记录类
	void SetLogger(std::shared_ptr<ILogger> logger)
	{
		this->logger_ = logger;
	}

	/// 获取日志记录类
	std::shared_ptr<ILogger> GetLogger() const
	{
		return this->logger_;
	}


	/**
	 * @brief 注册事件回调函数
	 * 
	 * @tparam EventType 事件类型
	 * @param callback 回调函数
	 */
	///@{
	template<typename EventType> void On(EventCallback<EventType> callback)
	{
		type_check_<EventType>();

		this->EventHandlers_[EventType::GetType()] = std::move(callback);
	}

	template <EventTypes Type> void On(EventCallback<GetEventType_t<Type>> callback)
	{
		this->On(std::move(callback));
	}
	///@}


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

	/// 返回是否已成功连接mirai-api-http
	bool isConnected() const
	{
		std::shared_lock<std::shared_mutex> lk(this->mtx_);
		return this->connected_;
	}


	using string = std::string;


	/**
	 * @brief 返回兼容的mirai-api-http的版本号
	 * 
	 * @return  兼容的mirai-api-http插件的版本号 “x.x.x”
	 */
	constexpr std::string_view GetCompatibleVersion() const { return "2.6.2"; }

	/**
	 * @brief 获取mirai-api-http插件的版本号
	 * 
	 * @return  mirai-api-http插件的版本号 “x.x.x”
	 */
	string GetMiraiApiHttpVersion() const;

	/**
	 * @brief 获取Bot账号
	 * 
	 * @return `QQ_t`
	 */
	QQ_t GetBotQQ() const;

	/**
	 * @brief 获取mirai中可用的QQBot列表
	 * 
	 * @return `std::vector<QQ_t>`
	 */
	std::vector<QQ_t> GetBotList() const;

	/**
	 * @brief 从消息id获取好友消息
	 * 
	 * @param id 消息id
	 * @param qq 好友QQ
	 * @return `FriendMessageEvent`
	 */
	FriendMessageEvent GetFriendMessage(MessageId_t id, QQ_t qq) const;

	/**
	 * @brief 从消息id获取群聊消息
	 * 
	 * @param id 消息id
	 * @param GroupId 群聊id
	 * @return `GroupMessageEvent` 
	 */
	GroupMessageEvent GetGroupMessage(MessageId_t id, GID_t GroupId) const;

	/**
	 * @brief 从消息id获取临时消息
	 * 
	 * @param id 消息id
	 * @param GroupId 群聊id
	 * @return `TempMessageEvent` 
	 */
	TempMessageEvent GetTempMessage(MessageId_t id, GID_t GroupId) const;

	/**
	 * @brief 从消息id获取陌生人消息
	 * 
	 * @param id 消息id
	 * @param qq 陌生人QQ
	 * @return `StrangerMessageEvent` 
	 */
	StrangerMessageEvent GetStrangerMessage(MessageId_t id, QQ_t qq) const;

	/**
	 * @brief 获取好友列表
	 * 
	 * @return `std::vector<User>` 
	 */
	std::vector<User> GetFriendList() const;

	/**
	 * @brief 获取群聊列表
	 * 
	 * @return `std::vector<Group>` 
	 */
	std::vector<Group> GetGroupList() const;

	/**
	 * @brief 获取群成员列表
	 * 
	 * @param GroupId 群聊id
	 * @return `std::vector<GroupMember>` 
	 */
	std::vector<GroupMember> GetMemberList(GID_t GroupId) const;

	/**
	 * @brief 获取Bot用户资料
	 * 
	 * @return `UserProfile` 
	 */
	UserProfile GetBotProfile() const;

	/**
	 * @brief 获取好友用户资料
	 * 
	 * @param qq 好友QQ
	 * @return `UserProfile` 
	 */
	UserProfile GetFriendProfile(QQ_t qq) const;

	/**
	 * @brief 获取群成员用户资料
	 * 
	 * @param GroupId 群聊id
	 * @param MemberId 群成员QQ
	 * @return `UserProfile` 
	 */
	UserProfile GetMemberProfile(GID_t GroupId, QQ_t MemberId) const;

	/**
	 * @brief 获取用户资料
	 * 
	 * @param qq 用户QQ
	 * @return `UserProfile` 
	 */
	UserProfile GetUserProfile(QQ_t qq) const;

	/**
	 * @brief 发送好友消息
	 * 
	 * @param qq 好友QQ
	 * @param message 消息内容
	 * @param QuoteId 引用回复内容的消息id
	 * @return 发送的消息id
	 */
	MessageId_t SendFriendMessage(QQ_t qq, const MessageChain& message,
	                              std::optional<MessageId_t> QuoteId = std::nullopt) const;

	/**
	 * @brief 发送群聊消息
	 * 
	 * @param GroupId 群聊id
	 * @param message 消息内容
	 * @param QuoteId 引用回复内容的消息id
	 * @return 发送的消息id
	 */
	MessageId_t SendGroupMessage(GID_t GroupId, const MessageChain& message,
	                             std::optional<MessageId_t> QuoteId = std::nullopt) const;

	/**
	 * @brief 发送临时会话消息
	 * 
	 * @param MemberId 群成员id
	 * @param GroupId 群聊id
	 * @param message 消息内容
	 * @param QuoteId 引用回复内容的消息id
	 * @return 发送的消息id
	 */
	MessageId_t SendTempMessage(QQ_t MemberId, GID_t GroupId, const MessageChain& message,
	                            std::optional<MessageId_t> QuoteId = std::nullopt) const;

	/**
	 * @brief 发送头像戳一戳消息
	 * 
	 * @param target 戳一戳目标
	 */
	void SendNudge(const NudgeTarget& target) const;

	/**
	 * @brief 发送好友戳一戳消息
	 * 
	 * @param qq 好友QQ
	 */
	void NudgeFriend(QQ_t qq) const;

	/**
	 * @brief 发送群聊戳一戳消息
	 * 
	 * @param MemberId 戳一戳对象QQ
	 * @param GroupId 群聊id
	 */
	void NudgeGroup(QQ_t MemberId, GID_t GroupId) const;

	/**
	 * @brief 发送陌生人戳一戳消息
	 * 
	 * @param qq 陌生人QQ
	 */
	void NudgeStranger(QQ_t qq) const;

	/**
	 * @brief 撤回好友消息
	 * 
	 * @param id 消息id
	 * @param qq 好友QQ
	 */
	void RecallFriendMessage(MessageId_t id, QQ_t qq) const;

	/**
	 * @brief 撤回群聊消息
	 * 
	 * @param id 消息id
	 * @param GroupId 群聊id
	 */
	void RecallGroupMessage(MessageId_t id, GID_t GroupId) const;

	/**
	 * @brief 获取好友漫游消息
	 * 
	 * @param qq 好友QQ
	 * @param TimeStart 消息起始时间
	 * @param TimeEnd 消息结束时间
	 * @return `std::vector<MessageChain>` 
	 */
	std::vector<MessageChain> GetRoamingFriendMessage(QQ_t qq, std::time_t TimeStart = 0,
	                                                  std::time_t TimeEnd = std::numeric_limits<std::time_t>::max()) const;

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
	                                            int64_t size = 0, bool withDownloadInfo = false) const;

	/**
	 * @brief 获取群文件信息
	 * 
	 * @param GroupId 群聊id
	 * @param dir 群文件路径
	 * @param withDownloadInfo 是否附带下载地址
	 * @return `GroupFileInfo` 
	 */
	GroupFileInfo GetGroupFileInfo(GID_t GroupId, const FilePath& dir, bool withDownloadInfo = false) const;

	/**
	 * @brief 获取群文件信息
	 * 
	 * @param GroupId 群聊id
	 * @param file 群文件信息，会从中读取路径并把剩余信息填入
	 * @param withDownloadInfo 是否附带下载地址
	 */
	void GetGroupFileInfo(GID_t GroupId, GroupFileInfo& file, bool withDownloadInfo = false) const;

	/**
	 * @brief 创建群文件夹
	 * 
	 * 目前QQ仅支持在根目录下创建文件夹，不许文件夹嵌套
	 * @param GroupId 群聊id
	 * @param directory 文件夹命名
	 * @return 新建的文件夹信息
	 */
	GroupFileInfo CreateGroupFileDirectory(GID_t GroupId, const string& directory) const;

	/**
	 * @brief 删除群文件
	 * 
	 * @param GroupId 群聊id
	 * @param dir 群文件路径
	 */
	void RemoveGroupFile(GID_t GroupId, const FilePath& dir) const;

	/**
	 * @brief 移动群文件
	 * 
	 * @param GroupId 群聊id
	 * @param FileDir 群文件路径
	 * @param MoveToDir 目标文件夹路径
	 */
	void MoveGroupFile(GID_t GroupId, const FilePath& FileDir, const FilePath& MoveToDir) const;

	/**
	 * @brief 重命名群文件
	 * 
	 * @param GroupId 群聊id
	 * @param FileDir 群文件路径
	 * @param NewName 新文件名
	 */
	void RenameGroupFile(GID_t GroupId, const FilePath& FileDir, const string& NewName) const;

	/**
	 * @brief 上传群文件
	 * 
	 * @param GroupId 群聊id
	 * @param UploadPath 上传路径
	 * @param name 文件名称
	 * @param content 文件内容
	 * @return 上传的群文件信息 
	 */
	GroupFileInfo UploadGroupFile(GID_t GroupId, const string& UploadPath, const string& name, string content) const;

	/**
	 * @brief 上传群文件
	 * 
	 * @param GroupId 群聊id
	 * @param UploadPath 上传路径
	 * @param name 文件名称
	 * @param file 文件流
	 * @return 上传的群文件信息 
	 */
	GroupFileInfo UploadGroupFile(GID_t GroupId, const string& UploadPath, const string& name, std::istream& file) const;

	/**
	 * @brief 上传群文件
	 * 
	 * Experimental: chunked data transfer is only supported in HTTP/1.1
	 *
	 * @param GroupId 群聊id
	 * @param UploadPath 上传路径
	 * @param name 文件名称
	 * @param ContentProvider 文件内容，返回false表示取消请求
	 * @return 上传的群文件信息 
	 */
	GroupFileInfo UploadGroupFile(GID_t GroupId, const string& UploadPath, const string& name,
	                              std::function<bool(size_t offset, std::ostream& sink, bool& finish)> ContentProvider) const;

	/**
	 * @brief 上传好友图片
	 * 
	 * @param content 图片文件内容（原始二进制，不是base64编码）
	 * @return `FriendImage` 
	 */
	FriendImage UploadFriendImage(string content) const;
	/**
	 * @brief 上传好友图片
	 * 
	 * @param file 图片文件流
	 * @return `FriendImage` 
	 */
	FriendImage UploadFriendImage(std::istream& file) const;
	/**
	 * @brief 上传好友图片
	 * 
	 * Experimental: chunked data transfer is only supported in HTTP/1.1
	 *
	 * @param ContentProvider 图片内容，返回false表示取消请求
	 * @return `FriendImage` 
	 */
	FriendImage UploadFriendImage(std::function<bool(size_t offset, std::ostream& sink, bool& finish)> ContentProvider) const;

	/**
	 * @brief 上传群聊图片
	 * 
	 * @param content 图片文件内容（原始二进制，不是base64编码）
	 * @return `GroupImage` 
	 */
	GroupImage UploadGroupImage(string content) const;
	/**
	 * @brief 上传群聊图片
	 * 
	 * @param file 图片文件流
	 * @return `GroupImage` 
	 */
	GroupImage UploadGroupImage(std::istream& file) const;
	/**
	 * @brief 上传群聊图片
	 * 
	 * Experimental: chunked data transfer is only supported in HTTP/1.1
	 *
	 * @param ContentProvider 图片内容，返回false表示取消请求
	 * @return `GroupImage` 
	 */
	GroupImage UploadGroupImage(std::function<bool(size_t offset, std::ostream& sink, bool& finish)> ContentProvider) const;

	/**
	 * @brief 上传临时会话图片
	 * 
	 * @param content 图片文件内容（原始二进制，不是base64编码）
	 * @return `TempImage` 
	 */
	TempImage UploadTempImage(const string& content) const;
	/**
	 * @brief 上传临时会话图片
	 * 
	 * @param file 图片文件流
	 * @return `TempImage` 
	 */
	TempImage UploadTempImage(std::istream& file) const;
	/**
	 * @brief 上传临时会话图片
	 * 
	 * Experimental: chunked data transfer is only supported in HTTP/1.1
	 *
	 * @param ContentProvider 图片内容，返回false表示取消请求
	 * @return `TempImage` 
	 */
	TempImage UploadTempImage(std::function<bool(size_t offset, std::ostream& sink, bool& finish)> ContentProvider) const;

	/**
	 * @brief 上传群聊语音
	 * 
	 * @param content 语音文件内容（原始二进制，不是base64编码）
	 * @return `GroupAudio` 
	 */
	GroupAudio UploadGroupAudio(const string& content) const;
	/**
	 * @brief 上传群聊语音
	 * 
	 * @param file 语音文件流
	 * @return `GroupAudio` 
	 */
	GroupAudio UploadGroupAudio(std::istream& file) const;
	/**
	 * @brief 上传群聊语音
	 * 
	 * Experimental: chunked data transfer is only supported in HTTP/1.1
	 *
	 * @param ContentProvider 语音内容，返回false表示取消请求
	 * @return `GroupAudio` 
	 */
	GroupAudio UploadGroupAudio(std::function<bool(size_t offset, std::ostream& sink, bool& finish)> ContentProvider) const;

	/**
	 * @brief 删除好友
	 * 
	 * @param qq 好友QQ
	 */
	void DeleteFriend(QQ_t qq) const;

	/**
	 * @brief 禁言群成员
	 * 
	 * @param GroupId 群聊id
	 * @param member 成员QQ
	 * @param time 禁言时间
	 */
	void Mute(GID_t GroupId, QQ_t member, std::chrono::seconds time) const;
	/**
	 * @brief 禁言群成员
	 * 
	 * @param member 群成员
	 * @param time 禁言时间
	 */
	void Mute(const GroupMember& member, std::chrono::seconds time) const;

	/**
	 * @brief 解除群成员禁言
	 * 
	 * @param GroupId 群聊id
	 * @param member 成员QQ
	 */
	void Unmute(GID_t GroupId, QQ_t member) const;
	/**
	 * @brief 解除群成员禁言
	 * 
	 * @param member 群成员
	 */
	void Unmute(const GroupMember& member) const;

	/**
	 * @brief 移除群成员
	 * 
	 * @param GroupId 群聊id
	 * @param member 成员QQ
	 * @param message 附带信息
	 */
	void Kick(GID_t GroupId, QQ_t member, const string& message) const;
	/**
	 * @brief 移除群成员
	 * 
	 * @param member 群成员
	 * @param message 附带信息
	 */
	void Kick(const GroupMember& member, const string& message) const;

	/**
	 * @brief 退出群聊
	 * 
	 * @param GroupId 群聊id
	 */
	void LeaveGroup(GID_t GroupId) const;

	/**
	 * @brief 禁言全体成员
	 * 
	 * @param GroupId 群聊id
	 */
	void MuteAll(GID_t GroupId) const;

	/**
	 * @brief 解除全体禁言
	 * 
	 * @param GroupId 群聊id
	 */
	void UnmuteAll(GID_t GroupId) const;

	/**
	 * @brief 设置群精华消息
	 * 
	 * @param GroupId 群聊id
	 * @param MessageId 消息id
	 */
	void SetEssence(GID_t GroupId, MessageId_t MessageId) const;

	/**
	 * @brief 获取群设置
	 * 
	 * @param GroupId 群聊id
	 * @return `GroupConfig` 
	 */
	GroupConfig GetGroupConfig(GID_t GroupId) const;

	/**
	 * @brief 修改群设置
	 * 
	 * 目前仅能修改群名称与是否允许邀请入群
	 * @param GroupId 群聊id
	 * @param name 群聊名称，默认保持原设置
	 * @param AllowMemberInvite 是否允许邀请入群，默认保持原设置
	 */
	void SetGroupConfig(GID_t GroupId, const string& name = "", std::optional<bool> AllowMemberInvite = std::nullopt) const;

	/**
	 * @brief 获取群成员资料
	 * 
	 * @param GroupId 群聊id
	 * @param member 成员QQ
	 * @return `GroupMember` 
	 */
	GroupMember GetMemberInfo(GID_t GroupId, QQ_t member) const;

	/**
	 * @brief 设置群成员资料
	 * 
	 * @param GroupId 群聊id
	 * @param member 成员QQ
	 * @param name 群名片
	 * @param title 群头衔
	 */
	void SetMemberInfo(GID_t GroupId, QQ_t member, const string& name = "", const string& title = "") const;

	/**
	 * @brief 设置群管理员
	 * 
	 * @param GroupId 群聊id
	 * @param member 成员QQ
	 * @param assign 设置管理员/撤销管理员
	 */
	void SetGroupAdmin(GID_t GroupId, QQ_t member, bool assign = true) const;

	/**
	 * @brief 获取群公告列表
	 * 
	 * @param GroupId 群聊id
	 * @param offset 分页偏移
	 * @param size 分页大小，默认为不分页（即返回所有结果）
	 * @return `std::vector<GroupAnnouncement>` 
	 */
	std::vector<GroupAnnouncement> GetAnnouncementList(GID_t GroupId, int64_t offset = 0, int64_t size = 0) const;

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
	                                      bool ShowPopup = false, bool RequireConfirm = false) const;

	/**
	 * @brief 删除群公告
	 * 
	 * @param GroupId 群聊id
	 * @param fid 群公告id
	 */
	void DeleteAnnouncement(GID_t GroupId, const string& fid) const;
	/**
	 * @brief 删除群公告
	 * 
	 * @param announcement 群公告对象
	 */
	void DeleteAnnouncement(const GroupAnnouncement& announcement) const;

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
	                               const string& message) const;
	/**
	 * @brief 处理添加好友申请事件 `NewFriendRequestEvent`
	 * 
	 * @param event 好友申请事件
	 * @param operation 处理操作
	 * @param message 回复消息
	 */
	void RespNewFriendRequestEvent(const NewFriendRequestEvent& event, NewFriendRequestOp operation,
	                               const string& message) const;

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
	                                const string& message) const;
	/**
	 * @brief 处理用户申请入群事件 `MemberJoinRequestEvent`
	 * 
	 * @param event 用户申请入群事件
	 * @param operation 处理操作
	 * @param message 回复消息
	 */
	void RespMemberJoinRequestEvent(const MemberJoinRequestEvent& event, MemberJoinRequestOp operation,
	                                const string& message) const;

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
	                                         BotInvitedJoinGroupRequestOp operation, const string& message) const;
	/**
	 * @brief 处理Bot被邀请入群事件 `BotInvitedJoinGroupRequestEvent`
	 * 
	 * @param event Bot被邀请入群事件
	 * @param operation 处理操作
	 * @param message 回复消息
	 */
	void RespBotInvitedJoinGroupRequestEvent(const BotInvitedJoinGroupRequestEvent& event,
	                                         BotInvitedJoinGroupRequestOp operation, const string& message) const;

	/**
	 * @brief 注册指令
	 * 
	 * @param name 指令名称
	 * @param alias 指令别名
	 * @param usage 使用说明
	 * @param description 指令描述
	 */
	void RegisterCommand(const string& name, const std::vector<string>& alias, const string& usage,
	                     const string& description) const;

	/**
	 * @brief 执行指令
	 * 
	 * @param command 指令内容
	 */
	void ExecuteCommand(const MessageChain& command) const;

	/**
	 * @brief 直接向mirai-api-http发送请求
	 * 
	 * 备用方法，可用于hack新功能或提供临时替代，正常情况下不推荐使用
	 * @param path 路径
	 * @param method 内容格式，可能是 "POST" "GET" 或subcommand等，取决于具体的Adaptor 
	 * @param data 请求数据
	 * @return `std::string`  
	 */
	string CallAPI(const string& path, const string& method, const string& data) const;

};

template<>
inline void MiraiClient::On<ClientConnectionEstablishedEvent>(EventCallback<ClientConnectionEstablishedEvent> callback)
{
	this->ConnectionEstablishedCallback_ = callback;
}

template<> 
inline void MiraiClient::On<ClientConnectionErrorEvent>(EventCallback<ClientConnectionErrorEvent> callback)
{
	this->ConnectionErrorCallback_ = callback;
}

template<> 
inline void MiraiClient::On<ClientConnectionClosedEvent>(EventCallback<ClientConnectionClosedEvent> callback)
{
	this->ConnectionClosedCallback_ = callback;
}

template<> 
inline void MiraiClient::On<ClientParseErrorEvent>(EventCallback<ClientParseErrorEvent> callback)
{
	this->ParseErrorCallback_ = callback;
}

} // namespace Mirai

#endif