#ifndef _MIRAI_CLIENT_IMPL_HPP_
#define _MIRAI_CLIENT_IMPL_HPP_

#include <map>
#include <string>
#include <optional>
#include <vector>

#include <httplib.h>
#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

namespace Mirai::Details
{

class HttpClientImpl
{
protected:
	friend class MiraiClient;

	httplib::Client _client;

public:
	HttpClientImpl(
		const std::string& scheme_host_port, 
		std::chrono::milliseconds connection_timeout = std::chrono::seconds(30),
		std::chrono::milliseconds read_timeout = std::chrono::seconds(5),
		std::chrono::milliseconds write_timeout = std::chrono::seconds(5),
		bool keep_alive = true, 
		bool redirect = true
	) : _client(scheme_host_port)
	{
		this->_client.set_connection_timeout(connection_timeout.count() / 1000, (connection_timeout.count() % 1000) * 1000);
		this->_client.set_read_timeout(read_timeout.count() / 1000, (read_timeout.count() % 1000) * 1000);
		this->_client.set_write_timeout(write_timeout.count() / 1000, (write_timeout.count() % 1000) * 1000);

		this->_client.set_keep_alive(keep_alive);
		this->_client.set_follow_location(redirect);
	}
	HttpClientImpl(const HttpClientImpl&) = delete;
	HttpClientImpl(HttpClientImpl&&) noexcept = delete;
	HttpClientImpl& operator=(const HttpClientImpl&) = delete;
	HttpClientImpl& operator=(HttpClientImpl&&) noexcept = delete;

	using json = nlohmann::json;

	// json Verify(const std::string& VerifyKey);
	// json Bind(const std::string& SessionKey, QQ_t qq);
	json SessionInfo(const std::string& SessionKey);
	// json Release(const std::string& SessionKey, QQ_t qq);

	// json CountMessage(const std::string& SessionKey);
	// json FetchMessage(const std::string& SessionKey, int count);
	// json FetchLatestMessage(const std::string& SessionKey, int count);
	// json PeekMessage(const std::string& SessionKey, int count);
	// json PeekLatestMessage(const std::string& SessionKey, int count);

	json About();
	json BotList();

	json MessageFromId(const std::string& SessionKey, MessageId_t id, UID_t target);

	json FriendList(const std::string& SessionKey);
	json GroupList(const std::string& SessionKey);
	json MemberList(const std::string& SessionKey, GID_t target);

	json BotProfile(const std::string& SessionKey);
	json FriendProfile(const std::string& SessionKey, QQ_t target);
	json MemberProfile(const std::string& SessionKey, GID_t target, QQ_t MemberId);
	json UserProfile(const std::string& SessionKey, QQ_t target);

	json SendFriendMessage(const std::string& SessionKey, QQ_t qq, const json& message, std::optional<MessageId_t> QuoteId = std::nullopt);
	json SendGroupMessage(const std::string& SessionKey, GID_t group, const json& message, std::optional<MessageId_t> QuoteId = std::nullopt);
	json SendTempMessage(const std::string& SessionKey, QQ_t qq, GID_t group, const json& message, std::optional<MessageId_t> QuoteId = std::nullopt);
	json SendNudge(const std::string& SessionKey, QQ_t target, UID_t subject, const std::string& kind);
	json Recall(const std::string& SessionKey, MessageId_t id, UID_t target);

	json FileList(const std::string& SessionKey, const std::string& id, const std::string& path, UID_t target, 
				int64_t offset = 0, int64_t size = 0, bool withDownloadInfo = false);
	json FileInfo(const std::string& SessionKey, const std::string& id, const std::string& path, UID_t target, bool withDownloadInfo = false);
	json FileMkdir(const std::string& SessionKey, const std::string& id, const std::string& path, UID_t target, const std::string& directory);
	json FileDelete(const std::string& SessionKey, const std::string& id, const std::string& path, UID_t target);
	json FileMove(const std::string& SessionKey, const std::string& id, const std::string& path, UID_t target, 
				const std::string& TargetDirId, const std::string& TargetDirPath);
	json FileRename(const std::string& SessionKey, const std::string& id, const std::string& path, UID_t target, const std::string& name);
	json FileUpload(const std::string& SessionKey, const std::string& path, UID_t target, const std::string& type, const std::string& name, const std::string& content);
	
	json UploadImage(const std::string& SessionKey, const std::string& type, const std::string& image);
	json UploadAudio(const std::string& SessionKey, const std::string& type, const std::string& Audio);

	json DeleteFriend(const std::string& SessionKey, QQ_t target);
	
	json Mute(const std::string& SessionKey, GID_t target, QQ_t member, std::chrono::seconds time);
	json Unmute(const std::string& SessionKey, GID_t target, QQ_t member);
	json Kick(const std::string& SessionKey, GID_t target, QQ_t member, const std::string& message);
	json Quit(const std::string& SessionKey, GID_t target);
	json MuteAll(const std::string& SessionKey, GID_t target);
	json UnmuteAll(const std::string& SessionKey, GID_t target);
	json SetEssence(const std::string& SessionKey, MessageId_t id, GID_t target);
	json GetGroupConfig(const std::string& SessionKey, GID_t target);
	json PostGroupConfig(const std::string& SessionKey, GID_t target, const std::string& name,
						std::optional<bool> AllowConfessTalk = std::nullopt, std::optional<bool> AllowMemberInvite = std::nullopt,
						std::optional<bool> AutoApprove = std::nullopt, std::optional<bool> AllowAnonymousChat = std::nullopt);
	json GetMemberInfo(const std::string& SessionKey, GID_t target, QQ_t member);
	json PostMemberInfo(const std::string& SessionKey, GID_t target, QQ_t member, const std::string& name, const std::string& title);
	json MemberAdmin(const std::string& SessionKey, GID_t target, QQ_t member, bool assign);

	json AnnoList(const std::string& SessionKey, GID_t target, int64_t offset = 0, int64_t size = 0);
	json AnnoPublish(const std::string& SessionKey, GID_t target, const std::string& content, 
					const std::string& url, const std::string& path, const std::string& base64, 
					bool ToNewMember = false, bool pinned = false,
					bool ShowEditCard = false, bool ShowPopup = false, 
					bool RequireConfirm = false);
	json AnnoDelete(const std::string& SessionKey, GID_t target, const std::string& fid);

	json RespNewFriendRequestEvent(const std::string& SessionKey, int64_t EventId, QQ_t FromId, GID_t GroupId, int operate, const std::string& message);
	json RespMemberJoinRequestEvent(const std::string& SessionKey, int64_t EventId, QQ_t FromId, GID_t GroupId, int operate, const std::string& message);
	json RespBotInvitedJoinGroupRequestEvent(const std::string& SessionKey, int64_t EventId, QQ_t FromId, GID_t GroupId, int operate, const std::string& message);

	json CmdExecute(const std::string& SessionKey, const json& command);
	json CmdRegister(const std::string& SessionKey, const std::string& name, const std::vector<std::string>& alias, const std::string& usage,
					const std::string& description);

	json PostRaw(const std::string& path, const std::string& content, const std::string& ContentType);
	json GetRaw(const std::string& path, const std::multimap<std::string, std::string> params);
};

}
#endif