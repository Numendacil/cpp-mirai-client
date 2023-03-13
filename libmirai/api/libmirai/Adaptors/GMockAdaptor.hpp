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

#if __has_include(<gmock/gmock.h>)

#ifndef MIRAI_GMOCK_ADAPTOR_HPP_
#define MIRAI_GMOCK_ADAPTOR_HPP_

#include <memory>
#include <string_view>
#include <gmock/gmock.h>

#include "IAdaptor.hpp"

/// 所有mirai相关的对象的命名空间
namespace Mirai
{

class GMockAdaptor : public IAdaptor 
{
	static constexpr std::string_view SESSION_KEY = "SessionKey";

public:
	template<typename... Args> 
	using Callback = std::function<void(Args...)>;
	using string = std::string;

	GMockAdaptor() = default;
	~GMockAdaptor() override = default;

	Callback<ClientConnectionEstablishedEvent> ConnectionEstablishedCallback;
	Callback<const string&> TextCallback;
	Callback<ClientConnectionErrorEvent> ConnectionErrorCallback;
	Callback<ClientConnectionClosedEvent> ConnectionClosedCallback;


	// *********************************************
	// ******* Connection & Event related **********
	// *********************************************

	/**
	 * @brief Register callback for ConnectionEstablished event
	 * 
	 */
	void OnConnectionEstablished(Callback<ClientConnectionEstablishedEvent> callback) override
	{
		this->ConnectionEstablishedCallback = callback;
	}

	/**
	 * @brief Register callback for receiving content
	 * 
	 */
	void OnRecv(Callback<const string&> callback) override
	{
		this->TextCallback = callback;
	}

	/**
	 * @brief Register callback for ConnectionError event
	 * 
	 */
	void OnConnectionError(Callback<ClientConnectionErrorEvent> callback) override
	{
		this->ConnectionErrorCallback = callback;
	}

	/**
	 * @brief Register callback for ConnectionClosed event
	 * 
	 */
	void OnConnectionClosed(Callback<ClientConnectionClosedEvent> callback) override
	{
		this->ConnectionClosedCallback = callback;
	}
	
	/**
	 * @brief Called for opening a session to mirai-api-http
	 * 
	 * Connect should not return before ConnectionEstablished callback is called
	 * @return Session key
	 */
	string Connect() override
	{

		if (this->ConnectionEstablishedCallback)
		{
			ClientConnectionEstablishedEvent event;

			event.uri = "/api";
			event.headers = {
				{"Date", "Tue, 07 Mar 2023 07:35:38 GMT"},
				{"Server", "ktor-server-core/1.6.7"}
			};
			event.protocol = "HTTP";
			event.SessionKey = SESSION_KEY;
			event.BotProfile.id = 12345_qq;
			event.BotProfile.nickname = "Elanor";
			event.BotProfile.remark = "Elanor";

			this->ConnectionEstablishedCallback(std::move(event));
		}
		return string(SESSION_KEY);
	}

	/**
	 * @brief Called for closing a session
	 * 
	 * Diconnect should not return before ConnectionClosed callback is called
	 */
	void Disconnect(const string& SessionKey) override
	{
		if (SessionKey != SESSION_KEY)
			return;

		if (!this->ConnectionClosedCallback)
			return;

		ClientConnectionClosedEvent event;
		event.code = 1000;
		event.reason = "Normal Closure";
		event.remote = false;
		
		this->ConnectionClosedCallback(std::move(event));
	}



	// *********************************************
	// *************** Request API *****************
	// *********************************************

	/// 获取版本号
	MOCK_METHOD(string, Version, (), (override));

	/// 获取Bot账号
	MOCK_METHOD(QQ_t, GetBotQQ, (), (override));

	/// 获取登录账号
	MOCK_METHOD(std::vector<QQ_t>, BotList, (), (override));

	/// 通过messageId获取消息
	MOCK_METHOD((std::variant<FriendMessageEvent, GroupMessageEvent, TempMessageEvent, StrangerMessageEvent>), 
	MessageFromId, (const string& SessionKey, MessageId_t id, UID_t target), (override));	//? std::any or std::variant

	
	/// 获取好友列表
	MOCK_METHOD(std::vector<User>, FriendList, (const string& SessionKey), (override));

	/// 获取群列表
	MOCK_METHOD(std::vector<Group>, GroupList, (const string& SessionKey), (override));

	/// 获取群成员列表
	MOCK_METHOD(std::vector<GroupMember>, MemberList, (const string& SessionKey, GID_t target), (override));

	//TODO: 获取最新群成员列表
	

	/// 获取Bot资料
	MOCK_METHOD(UserProfile, GetBotProfile, (const string& SessionKey), (override));

	/// 获取好友资料
	MOCK_METHOD(UserProfile, GetFriendProfile, (const string& SessionKey, QQ_t target), (override));

	/// 获取群成员资料
	MOCK_METHOD(UserProfile, GetMemberProfile, (const string& SessionKey, GID_t target, QQ_t MemberId), (override));

	/// 获取QQ用户资料
	MOCK_METHOD(UserProfile, GetUserProfile, (const string& SessionKey, QQ_t target), (override));


	/// 发送好友消息
	MOCK_METHOD(MessageId_t, SendFriendMessage, (const string& SessionKey, QQ_t qq, const MessageChain& message,
	                       std::optional<MessageId_t> QuoteId), (override));

	/// 发送群消息
	MOCK_METHOD(MessageId_t, SendGroupMessage, (const string& SessionKey, GID_t group, const MessageChain& message,
	                      std::optional<MessageId_t> QuoteId), (override));

	/// 发送临时会话消息
	MOCK_METHOD(MessageId_t, SendTempMessage, (const string& SessionKey, QQ_t qq, GID_t group, const MessageChain& message,
	                     std::optional<MessageId_t> QuoteId), (override));

	/// 发送头像戳一戳消息
	MOCK_METHOD(void, SendNudge, (const string& SessionKey, QQ_t target, UID_t subject, NudgeType kind), (override));

	/// 撤回消息
	MOCK_METHOD(void, Recall, (const string& SessionKey, MessageId_t id, UID_t target), (override));

	/// 获取漫游消息
	MOCK_METHOD(std::vector<MessageChain>, RoamingMessages, (const string& SessionKey, std::time_t TimeStart, std::time_t TimeEnd, UID_t target), (override));


	/// 查看文件列表
	MOCK_METHOD(std::vector<GroupFileInfo>, FileList, (const string& SessionKey, const string& id, const string& path, UID_t target,
	              int64_t offset, int64_t size, bool withDownloadInfo), (override));

	/// 获取文件信息
	MOCK_METHOD(GroupFileInfo, FileInfo, (const string& SessionKey, const string& id, const string& path, UID_t target,
	              bool withDownloadInfo), (override));

	/// 创建文件夹
	MOCK_METHOD(GroupFileInfo, FileMkdir, (const string& SessionKey, const string& id, const string& path, UID_t target,
	               const string& directory), (override));
	
	/// 删除文件
	MOCK_METHOD(void, FileDelete, (const string& SessionKey, const string& id, const string& path, UID_t target), (override));
	
	/// 移动文件
	MOCK_METHOD(void, FileMove, (const string& SessionKey, const string& id, const string& path, UID_t target,
	              const string& TargetDirId, const string& TargetDirPath), (override));
	
	/// 重命名文件
	MOCK_METHOD(void, FileRename, (const string& SessionKey, const string& id, const string& path, UID_t target,
	                const string& name), (override));


	/// 上传文件
	MOCK_METHOD(GroupFileInfo, FileUpload, (const string& SessionKey, const string& path, UID_t target, const string& type,
	                const string& name, string content), (override));
	
	/**
	 * @brief 分块上传文件
	 * 
	 * @attention Experimental API
	 */
	MOCK_METHOD(GroupFileInfo, FileUploadChunked, (const string& SessionKey, const string& path, UID_t target,
	                       const string& type, const string& name,
	                       std::function<bool(size_t offset, std::ostream& sink, bool& finish)> ContentProvider), (override));

	/// 上传图片
	MOCK_METHOD(MiraiImage, UploadImage, (const string& SessionKey, const string& type, string image), (override));

	/**
	 * @brief 分块上传图片
	 * 
	 * @attention Experimental API
	 */
	MOCK_METHOD(MiraiImage, UploadImageChunked, (const string& SessionKey, const string& type,
	                        std::function<bool(size_t offset, std::ostream& sink, bool& finish)> ContentProvider), (override));
	/// 上传音频
	MOCK_METHOD(MiraiAudio, UploadAudio, (const string& SessionKey, const string& type, string Audio), (override));
	
	/**
	 * @brief 分块上传音频
	 * 
	 * @attention Experimental API
	 */
	MOCK_METHOD(MiraiAudio, UploadAudioChunked, (const string& SessionKey, const string& type,
	                        std::function<bool(size_t offset, std::ostream& sink, bool& finish)> ContentProvider), (override));


	/// 删除好友
	MOCK_METHOD(void, DeleteFriend, (const string& SessionKey, QQ_t target), (override));


	/// 禁言群成员
	MOCK_METHOD(void, Mute, (const string& SessionKey, GID_t target, QQ_t member, std::chrono::seconds time), (override));

	/// 解除群成员禁言
	MOCK_METHOD(void, Unmute, (const string& SessionKey, GID_t target, QQ_t member), (override));

	/// 移除群成员
	MOCK_METHOD(void, Kick, (const string& SessionKey, GID_t target, QQ_t member, const string& message), (override));

	/// 退出群聊
	MOCK_METHOD(void, Quit, (const string& SessionKey, GID_t target), (override));

	/// 全体禁言
	MOCK_METHOD(void, MuteAll, (const string& SessionKey, GID_t target), (override));

	/// 解除全体禁言
	MOCK_METHOD(void, UnmuteAll, (const string& SessionKey, GID_t target), (override));

	/// 设置群精华消息
	MOCK_METHOD(void, SetEssence, (const string& SessionKey, MessageId_t id, GID_t target), (override));

	/// 获取群设置
	MOCK_METHOD(GroupConfig, GetGroupConfig, (const string& SessionKey, GID_t target), (override));

	/// 修改群设置
	MOCK_METHOD(void, SetGroupConfig, (const string& SessionKey, GID_t target, const string& name,
	                     std::optional<bool> AllowConfessTalk,
	                     std::optional<bool> AllowMemberInvite,
	                     std::optional<bool> AutoApprove,
	                     std::optional<bool> AllowAnonymousChat), (override));

	/// 获取群员设置
	MOCK_METHOD(GroupMember, GetMemberInfo, (const string& SessionKey, GID_t target, QQ_t member), (override));
	
	/// 修改群员设置
	MOCK_METHOD(void, SetMemberInfo, (const string& SessionKey, GID_t target, QQ_t member, const string& name,
	                    const string& title), (override));

	/// 修改群员管理员
	MOCK_METHOD(void, MemberAdmin, (const string& SessionKey, GID_t target, QQ_t member, bool assign), (override));

	
	/// 获取群公告
	MOCK_METHOD(std::vector<GroupAnnouncement>, AnnoList, (const string& SessionKey, GID_t target, int64_t offset, int64_t size), (override));
	
	/// 发布群公告
	MOCK_METHOD(GroupAnnouncement, AnnoPublish, (const string& SessionKey, GID_t target, const string& content, const string& url,
	                 const string& path, const string& base64, bool ToNewMember, bool pinned,
	                 bool ShowEditCard, bool ShowPopup, bool RequireConfirm), (override));
	
	/// 删除群公告
	MOCK_METHOD(void, AnnoDelete, (const string& SessionKey, GID_t target, const string& fid), (override));

	
	/// 添加好友申请
	MOCK_METHOD(void, RespNewFriendRequestEvent, (const string& SessionKey, int64_t EventId, QQ_t FromId, GID_t GroupId,
	                               int operate, const string& message), (override));
	
	/// 用户入群申请
	MOCK_METHOD(void, RespMemberJoinRequestEvent, (const string& SessionKey, int64_t EventId, QQ_t FromId, GID_t GroupId,
	                                int operate, const string& message), (override));
	
	/// Bot被邀请入群申请
	MOCK_METHOD(void, RespBotInvitedJoinGroupRequestEvent, (const string& SessionKey, int64_t EventId, QQ_t FromId, GID_t GroupId,
	                                         int operate, const string& message), (override));

	
	/// 执行命令
	MOCK_METHOD(void, CmdExecute, (const string& SessionKey, const MessageChain& command), (override));
	
	/// 注册命令
	MOCK_METHOD(void, CmdRegister, (const string& SessionKey, const string& name, const std::vector<string>& alias,
	                 const string& usage, const string& description), (override));

	
	/// Call any api, just in case
	MOCK_METHOD(string, CallAPI, (const string& path, const string& method, const string& data), (override));
};

template <typename Callable>
auto MakeGMockAdaptor(Callable&& func)
-> std::enable_if_t<
	std::is_invocable_v<Callable, GMockAdaptor&>, 
	std::unique_ptr<IAdaptor>>
{
	auto adaptor = std::make_unique<GMockAdaptor>();
	func(*adaptor);
	return adaptor;
}

} // namespace Mirai

#endif

#endif