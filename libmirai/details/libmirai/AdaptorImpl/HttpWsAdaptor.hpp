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

#ifndef MIRAI_HTTP_WEBSOCKET_ADAPTOR_IMPLEMENTATION_HPP_
#define MIRAI_HTTP_WEBSOCKET_ADAPTOR_IMPLEMENTATION_HPP_

#include <condition_variable>
#include <functional>
#include <mutex>
#include <string>
#include <variant>

#include <httplib.h>
#include <ixwebsocket/IXWebSocket.h>

#include <libmirai/Adaptors/HttpWsAdaptor.hpp>
#include <libmirai/Events/Events.hpp>
#include <libmirai/Types/Types.hpp>

/// 所有mirai相关的对象的命名空间
namespace Mirai::Details
{

class HttpWsAdaptor final : public IAdaptor
{
private:
	HttpWsAdaptorConfig config_;

	ix::WebSocket wsclient_;
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
	static size_t init_; // For calling WSAstartup once per program
	std::map<std::thread::id, httplib::Client> httpclients_;

	std::optional<ClientConnectionEstablishedEvent> info_;
	mutable std::mutex connectmtx_;	// Guarding connection info
	mutable std::condition_variable connectcv_;

	Callback<ClientConnectionEstablishedEvent> ConnectionEstablishedCallback_;
	Callback<const string&> TextCallback_;
	Callback<ClientConnectionErrorEvent> ConnectionErrorCallback_;
	Callback<ClientConnectionClosedEvent> ConnectionClosedCallback_;

	httplib::Client CreateClient_() const;
	httplib::Client& GetClient_();

	mutable std::mutex httpmtx_;	// Guarding httpclients map

	bool bind_ = false;

public:
	template<typename... Args> using Callback = std::function<void(Args...)>;
	using string = std::string;

	HttpWsAdaptor(HttpWsAdaptorConfig config);
	
	HttpWsAdaptor(HttpWsAdaptor&&) noexcept = delete;
	HttpWsAdaptor& operator=(HttpWsAdaptor&&) noexcept = delete;
	HttpWsAdaptor(const HttpWsAdaptor&) = delete;
	HttpWsAdaptor& operator=(const HttpWsAdaptor&) = delete;

	~HttpWsAdaptor() override;


	// *********************************************
	// ******* Connection & Event related **********
	// *********************************************

	/**
	 * @brief Register callback for ConnectionEstablished event
	 * 
	 */
	void OnConnectionEstablished(Callback<ClientConnectionEstablishedEvent> callback) override
	{
		this->ConnectionEstablishedCallback_ = callback;
	}

	/**
	 * @brief Register callback for receiving content
	 * 
	 */
	void OnRecv(Callback<const string&> callback) override
	{
		this->TextCallback_ = callback;
	}

	/**
	 * @brief Register callback for ConnectionError event
	 * 
	 */
	void OnConnectionError(Callback<ClientConnectionErrorEvent> callback) override
	{
		this->ConnectionErrorCallback_ = callback;
	}

	/**
	 * @brief Register callback for ConnectionClosed event
	 * 
	 */
	void OnConnectionClosed(Callback<ClientConnectionClosedEvent> callback) override
	{
		this->ConnectionClosedCallback_ = callback;
	}

	/**
	 * @brief Called for opening a session to mirai-api-http
	 * 
	 * Connect should not return before ConnectionEstablished callback is called
	 * @return Session key
	 */
	string Connect() override;

	/**
	 * @brief Called for closing a session
	 * 
	 * Diconnect should not return before ConnectionClosed callback is called
	 */
	void Disconnect(const string& SessionKey) override;


	// *********************************************
	// *************** Request API *****************
	// *********************************************

	/// 获取版本号
	string Version() override;

	/// 获取Bot账号
	QQ_t GetBotQQ() override;

	/// 获取登录账号
	std::vector<QQ_t> BotList() override;

	/// 通过messageId获取消息
	std::variant<FriendMessageEvent, GroupMessageEvent, TempMessageEvent, StrangerMessageEvent>
	MessageFromId(const string& SessionKey, MessageId_t id, UID_t target) override; //? std::any or std::variant


	/// 获取好友列表
	std::vector<User> FriendList(const string& SessionKey) override;

	/// 获取群列表
	std::vector<Group> GroupList(const string& SessionKey) override;

	/// 获取群成员列表
	std::vector<GroupMember> MemberList(const string& SessionKey, GID_t target) override;

	//TODO: 获取最新群成员列表


	/// 获取Bot资料
	UserProfile GetBotProfile(const string& SessionKey) override;

	/// 获取好友资料
	UserProfile GetFriendProfile(const string& SessionKey, QQ_t target) override;

	/// 获取群成员资料
	UserProfile GetMemberProfile(const string& SessionKey, GID_t target, QQ_t MemberId) override;

	/// 获取QQ用户资料
	UserProfile GetUserProfile(const string& SessionKey, QQ_t target) override;


	/// 发送好友消息
	MessageId_t SendFriendMessage(const string& SessionKey, QQ_t qq, const MessageChain& message,
	                              std::optional<MessageId_t> QuoteId = std::nullopt) override;

	/// 发送群消息
	MessageId_t SendGroupMessage(const string& SessionKey, GID_t group, const MessageChain& message,
	                             std::optional<MessageId_t> QuoteId = std::nullopt) override;

	/// 发送临时会话消息
	MessageId_t SendTempMessage(const string& SessionKey, QQ_t qq, GID_t group, const MessageChain& message,
	                            std::optional<MessageId_t> QuoteId = std::nullopt) override;

	/// 发送头像戳一戳消息
	void SendNudge(const string& SessionKey, QQ_t target, UID_t subject, NudgeType kind) override;

	/// 撤回消息
	void Recall(const string& SessionKey, MessageId_t id, UID_t target) override;

	/// 获取漫游消息
	std::vector<MessageChain> RoamingMessages(const string& SessionKey, std::time_t TimeStart, std::time_t TimeEnd,
	                                          UID_t target) override;


	/// 查看文件列表
	std::vector<GroupFileInfo> FileList(const string& SessionKey, const string& id, const string& path, UID_t target,
	                                    int64_t offset = 0, int64_t size = 0, bool withDownloadInfo = false) override;

	/// 获取文件信息
	GroupFileInfo FileInfo(const string& SessionKey, const string& id, const string& path, UID_t target,
	                       bool withDownloadInfo = false) override;

	/// 创建文件夹
	GroupFileInfo FileMkdir(const string& SessionKey, const string& id, const string& path, UID_t target,
	                        const string& directory) override;

	/// 删除文件
	void FileDelete(const string& SessionKey, const string& id, const string& path, UID_t target) override;

	/// 移动文件
	void FileMove(const string& SessionKey, const string& id, const string& path, UID_t target,
	              const string& TargetDirId, const string& TargetDirPath) override;

	/// 重命名文件
	void FileRename(const string& SessionKey, const string& id, const string& path, UID_t target,
	                const string& name) override;


	/// 上传文件
	GroupFileInfo FileUpload(const string& SessionKey, const string& path, UID_t target, const string& type,
	                         const string& name, string content) override;

	/**
	 * @brief 分块上传文件
	 * 
	 * @attention Experimental API
	 */
	GroupFileInfo
	FileUploadChunked(const string& SessionKey, const string& path, UID_t target, const string& type,
	                  const string& name,
	                  std::function<bool(size_t offset, std::ostream& sink, bool& finish)> ContentProvider) override;

	/// 上传图片
	MiraiImage UploadImage(const string& SessionKey, const string& type, string image) override;

	/**
	 * @brief 分块上传图片
	 * 
	 * @attention Experimental API
	 */
	MiraiImage
	UploadImageChunked(const string& SessionKey, const string& type,
	                   std::function<bool(size_t offset, std::ostream& sink, bool& finish)> ContentProvider) override;
	/// 上传音频
	MiraiAudio UploadAudio(const string& SessionKey, const string& type, string Audio) override;

	/**
	 * @brief 分块上传音频
	 * 
	 * @attention Experimental API
	 */
	MiraiAudio
	UploadAudioChunked(const string& SessionKey, const string& type,
	                   std::function<bool(size_t offset, std::ostream& sink, bool& finish)> ContentProvider) override;


	/// 删除好友
	void DeleteFriend(const string& SessionKey, QQ_t target) override;


	/// 禁言群成员
	void Mute(const string& SessionKey, GID_t target, QQ_t member, std::chrono::seconds time) override;

	/// 解除群成员禁言
	void Unmute(const string& SessionKey, GID_t target, QQ_t member) override;

	/// 移除群成员
	void Kick(const string& SessionKey, GID_t target, QQ_t member, const string& message) override;

	/// 退出群聊
	void Quit(const string& SessionKey, GID_t target) override;

	/// 全体禁言
	void MuteAll(const string& SessionKey, GID_t target) override;

	/// 解除全体禁言
	void UnmuteAll(const string& SessionKey, GID_t target) override;

	/// 设置群精华消息
	void SetEssence(const string& SessionKey, MessageId_t id, GID_t target) override;

	/// 获取群设置
	GroupConfig GetGroupConfig(const string& SessionKey, GID_t target) override;

	/// 修改群设置
	void SetGroupConfig(const string& SessionKey, GID_t target, const string& name,
	                    std::optional<bool> AllowConfessTalk = std::nullopt,
	                    std::optional<bool> AllowMemberInvite = std::nullopt,
	                    std::optional<bool> AutoApprove = std::nullopt,
	                    std::optional<bool> AllowAnonymousChat = std::nullopt) override;

	/// 获取群员设置
	GroupMember GetMemberInfo(const string& SessionKey, GID_t target, QQ_t member) override;

	/// 修改群员设置
	void SetMemberInfo(const string& SessionKey, GID_t target, QQ_t member, const string& name,
	                   const string& title) override;

	/// 修改群员管理员
	void MemberAdmin(const string& SessionKey, GID_t target, QQ_t member, bool assign) override;


	/// 获取群公告
	std::vector<GroupAnnouncement> AnnoList(const string& SessionKey, GID_t target, int64_t offset = 0,
	                                        int64_t size = 0) override;

	/// 发布群公告
	GroupAnnouncement AnnoPublish(const string& SessionKey, GID_t target, const string& content, const string& url,
	                              const string& path, const string& base64, bool ToNewMember = false,
	                              bool pinned = false, bool ShowEditCard = false, bool ShowPopup = false,
	                              bool RequireConfirm = false) override;

	/// 删除群公告
	void AnnoDelete(const string& SessionKey, GID_t target, const string& fid) override;


	/// 添加好友申请
	void RespNewFriendRequestEvent(const string& SessionKey, int64_t EventId, QQ_t FromId, GID_t GroupId, int operate,
	                               const string& message) override;

	/// 用户入群申请
	void RespMemberJoinRequestEvent(const string& SessionKey, int64_t EventId, QQ_t FromId, GID_t GroupId, int operate,
	                                const string& message) override;

	/// Bot被邀请入群申请
	void RespBotInvitedJoinGroupRequestEvent(const string& SessionKey, int64_t EventId, QQ_t FromId, GID_t GroupId,
	                                         int operate, const string& message) override;


	/// 执行命令
	void CmdExecute(const string& SessionKey, const MessageChain& command) override;

	/// 注册命令
	void CmdRegister(const string& SessionKey, const string& name, const std::vector<string>& alias,
	                 const string& usage, const string& description) override;


	/// Call any api, just in case
	string CallAPI(const string& path, const string& method, const string& data) override;
};


} // namespace Mirai

#endif