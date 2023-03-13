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

#ifndef MIRAI_ADAPTOR_INTERFACE_HPP_
#define MIRAI_ADAPTOR_INTERFACE_HPP_

#include <functional>
#include <string>
#include <variant>

#include <libmirai/Events/Events.hpp>
#include <libmirai/Types/Types.hpp>

/// 所有mirai相关的对象的命名空间
namespace Mirai
{

class IAdaptor
{
protected:
	IAdaptor() = default;

public:
	template<typename... Args> 
	using Callback = std::function<void(Args...)>;
	using string = std::string;

	virtual ~IAdaptor() = default;


	// *********************************************
	// ******* Connection & Event related **********
	// *********************************************

	/**
	 * @brief Register callback for ConnectionEstablished event
	 * 
	 */
	virtual void OnConnectionEstablished(Callback<ClientConnectionEstablishedEvent>) = 0;

	/**
	 * @brief Register callback for receiving content
	 * 
	 */
	virtual void OnRecv(Callback<const string&>) = 0;

	/**
	 * @brief Register callback for ConnectionError event
	 * 
	 */
	virtual void OnConnectionError(Callback<ClientConnectionErrorEvent>) = 0;

	/**
	 * @brief Register callback for ConnectionClosed event
	 * 
	 */
	virtual void OnConnectionClosed(Callback<ClientConnectionClosedEvent>) = 0;
	
	/**
	 * @brief Called for opening a session to mirai-api-http
	 * 
	 * Connect should not return before ConnectionEstablished callback is called
	 * @return Session key
	 */
	virtual string Connect() = 0;

	/**
	 * @brief Called for closing a session
	 * 
	 * Diconnect should not return before ConnectionClosed callback is called
	 */
	virtual void Disconnect(const string& SessionKey) = 0;



	// *********************************************
	// *************** Request API *****************
	// *********************************************

	/// 获取版本号
	virtual string Version() = 0;

	/// 获取Bot账号
	virtual QQ_t GetBotQQ() = 0;

	/// 获取登录账号
	virtual std::vector<QQ_t> BotList() = 0;

	/// 通过messageId获取消息
	virtual std::variant<FriendMessageEvent, GroupMessageEvent, TempMessageEvent, StrangerMessageEvent> 
	MessageFromId(const string& SessionKey, MessageId_t id, UID_t target) = 0;	//? std::any or std::variant

	
	/// 获取好友列表
	virtual std::vector<User> FriendList(const string& SessionKey) = 0;

	/// 获取群列表
	virtual std::vector<Group> GroupList(const string& SessionKey) = 0;

	/// 获取群成员列表
	virtual std::vector<GroupMember> MemberList(const string& SessionKey, GID_t target) = 0;

	//TODO: 获取最新群成员列表
	

	/// 获取Bot资料
	virtual UserProfile GetBotProfile(const string& SessionKey) = 0;

	/// 获取好友资料
	virtual UserProfile GetFriendProfile(const string& SessionKey, QQ_t target) = 0;

	/// 获取群成员资料
	virtual UserProfile GetMemberProfile(const string& SessionKey, GID_t target, QQ_t MemberId) = 0;

	/// 获取QQ用户资料
	virtual UserProfile GetUserProfile(const string& SessionKey, QQ_t target) = 0;


	/// 发送好友消息
	virtual MessageId_t SendFriendMessage(const string& SessionKey, QQ_t qq, const MessageChain& message,
	                       std::optional<MessageId_t> QuoteId = std::nullopt) = 0;

	/// 发送群消息
	virtual MessageId_t SendGroupMessage(const string& SessionKey, GID_t group, const MessageChain& message,
	                      std::optional<MessageId_t> QuoteId = std::nullopt) = 0;

	/// 发送临时会话消息
	virtual MessageId_t SendTempMessage(const string& SessionKey, QQ_t qq, GID_t group, const MessageChain& message,
	                     std::optional<MessageId_t> QuoteId = std::nullopt) = 0;

	/// 发送头像戳一戳消息
	virtual void SendNudge(const string& SessionKey, QQ_t target, UID_t subject, NudgeType kind) = 0;

	/// 撤回消息
	virtual void Recall(const string& SessionKey, MessageId_t id, UID_t target) = 0;

	/// 获取漫游消息
	virtual std::vector<MessageChain> RoamingMessages(const string& SessionKey, std::time_t TimeStart, std::time_t TimeEnd, UID_t target) = 0;


	/// 查看文件列表
	virtual std::vector<GroupFileInfo> FileList(const string& SessionKey, const string& id, const string& path, UID_t target,
	              int64_t offset = 0, int64_t size = 0, bool withDownloadInfo = false) = 0;

	/// 获取文件信息
	virtual GroupFileInfo FileInfo(const string& SessionKey, const string& id, const string& path, UID_t target,
	              bool withDownloadInfo = false) = 0;

	/// 创建文件夹
	virtual GroupFileInfo FileMkdir(const string& SessionKey, const string& id, const string& path, UID_t target,
	               const string& directory) = 0;
	
	/// 删除文件
	virtual void FileDelete(const string& SessionKey, const string& id, const string& path, UID_t target) = 0;
	
	/// 移动文件
	virtual void FileMove(const string& SessionKey, const string& id, const string& path, UID_t target,
	              const string& TargetDirId, const string& TargetDirPath) = 0;
	
	/// 重命名文件
	virtual void FileRename(const string& SessionKey, const string& id, const string& path, UID_t target,
	                const string& name) = 0;


	/// 上传文件
	virtual GroupFileInfo FileUpload(const string& SessionKey, const string& path, UID_t target, const string& type,
	                const string& name, string content) = 0;
	
	/**
	 * @brief 分块上传文件
	 * 
	 * @attention Experimental API
	 */
	virtual GroupFileInfo FileUploadChunked(const string& SessionKey, const string& path, UID_t target,
	                       const string& type, const string& name,
	                       std::function<bool(size_t offset, std::ostream& sink, bool& finish)> ContentProvider) = 0;

	/// 上传图片
	virtual MiraiImage UploadImage(const string& SessionKey, const string& type, string image) = 0;

	/**
	 * @brief 分块上传图片
	 * 
	 * @attention Experimental API
	 */
	virtual MiraiImage UploadImageChunked(const string& SessionKey, const string& type,
	                        std::function<bool(size_t offset, std::ostream& sink, bool& finish)> ContentProvider) = 0;
	/// 上传音频
	virtual MiraiAudio UploadAudio(const string& SessionKey, const string& type, string Audio) = 0;
	
	/**
	 * @brief 分块上传音频
	 * 
	 * @attention Experimental API
	 */
	virtual MiraiAudio UploadAudioChunked(const string& SessionKey, const string& type,
	                        std::function<bool(size_t offset, std::ostream& sink, bool& finish)> ContentProvider) = 0;


	/// 删除好友
	virtual void DeleteFriend(const string& SessionKey, QQ_t target) = 0;


	/// 禁言群成员
	virtual void Mute(const string& SessionKey, GID_t target, QQ_t member, std::chrono::seconds time) = 0;

	/// 解除群成员禁言
	virtual void Unmute(const string& SessionKey, GID_t target, QQ_t member) = 0;

	/// 移除群成员
	virtual void Kick(const string& SessionKey, GID_t target, QQ_t member, const string& message) = 0;

	/// 退出群聊
	virtual void Quit(const string& SessionKey, GID_t target) = 0;

	/// 全体禁言
	virtual void MuteAll(const string& SessionKey, GID_t target) = 0;

	/// 解除全体禁言
	virtual void UnmuteAll(const string& SessionKey, GID_t target) = 0;

	/// 设置群精华消息
	virtual void SetEssence(const string& SessionKey, MessageId_t id, GID_t target) = 0;

	/// 获取群设置
	virtual GroupConfig GetGroupConfig(const string& SessionKey, GID_t target) = 0;

	/// 修改群设置
	virtual void SetGroupConfig(const string& SessionKey, GID_t target, const string& name,
	                     std::optional<bool> AllowConfessTalk = std::nullopt,
	                     std::optional<bool> AllowMemberInvite = std::nullopt,
	                     std::optional<bool> AutoApprove = std::nullopt,
	                     std::optional<bool> AllowAnonymousChat = std::nullopt) = 0;

	/// 获取群员设置
	virtual GroupMember GetMemberInfo(const string& SessionKey, GID_t target, QQ_t member) = 0;
	
	/// 修改群员设置
	virtual void SetMemberInfo(const string& SessionKey, GID_t target, QQ_t member, const string& name,
	                    const string& title) = 0;

	/// 修改群员管理员
	virtual void MemberAdmin(const string& SessionKey, GID_t target, QQ_t member, bool assign) = 0;

	
	/// 获取群公告
	virtual std::vector<GroupAnnouncement> AnnoList(const string& SessionKey, GID_t target, int64_t offset = 0, int64_t size = 0) = 0;
	
	/// 发布群公告
	virtual GroupAnnouncement AnnoPublish(const string& SessionKey, GID_t target, const string& content, const string& url,
	                 const string& path, const string& base64, bool ToNewMember = false, bool pinned = false,
	                 bool ShowEditCard = false, bool ShowPopup = false, bool RequireConfirm = false) = 0;
	
	/// 删除群公告
	virtual void AnnoDelete(const string& SessionKey, GID_t target, const string& fid) = 0;

	
	/// 添加好友申请
	virtual void RespNewFriendRequestEvent(const string& SessionKey, int64_t EventId, QQ_t FromId, GID_t GroupId,
	                               int operate, const string& message) = 0;
	
	/// 用户入群申请
	virtual void RespMemberJoinRequestEvent(const string& SessionKey, int64_t EventId, QQ_t FromId, GID_t GroupId,
	                                int operate, const string& message) = 0;
	
	/// Bot被邀请入群申请
	virtual void RespBotInvitedJoinGroupRequestEvent(const string& SessionKey, int64_t EventId, QQ_t FromId, GID_t GroupId,
	                                         int operate, const string& message) = 0;

	
	/// 执行命令
	virtual void CmdExecute(const string& SessionKey, const MessageChain& command) = 0;
	
	/// 注册命令
	virtual void CmdRegister(const string& SessionKey, const string& name, const std::vector<string>& alias,
	                 const string& usage, const string& description) = 0;

	
	/// Call any api, just in case
	virtual string CallAPI(const string& path, const string& method, const string& data) = 0;
};


}

#endif