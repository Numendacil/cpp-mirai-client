#ifndef _MIRAI_CLIENT_HPP_
#define _MIRAI_CLIENT_HPP_

#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <functional>
#include <limits>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <optional>
#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>
#include <libmirai/Types/MediaTypes.hpp>
#include <libmirai/Types/GroupSettings.hpp>
#include <libmirai/Events/Events.hpp>
#include <libmirai/Messages/MessageChain.hpp>
#include <vector>

#include "SessionConfig.hpp"

namespace Mirai
{

namespace Details
{

class HttpClientImpl;
class MessageClientImpl;

}

namespace Utils
{

class ThreadPool;

}

class MiraiClient
{
public:
	template<typename Event>
	using EventCallback = std::function<void(Event)>;
	using EventHandler = std::function<void(const nlohmann::json&)>;

protected:
	mutable std::mutex _mtx;
	mutable std::condition_variable _cv;

	SessionConfigs _config;

	std::string _SessionKey;
	bool _SessionKeySet = false;
	bool _connected = false;	// Probably can be removed, but just in case

	std::unique_ptr<Details::HttpClientImpl> _HttpClient;
	std::unique_ptr<Details::MessageClientImpl> _MessageClient;

	std::unique_ptr<Utils::ThreadPool> _ThreadPool;

	ClientConnectionEstablishedEvent _HandshakeInfo;

	EventCallback<ClientConnectionEstablishedEvent> _ConnectionEstablishedCallback;
	EventCallback<ClientConnectionClosedEvent> _ConnectionClosedCallback;
	EventCallback<ClientConnectionErrorEvent> _ConnectionErrorCallback;
	EventCallback<ClientParseErrorEvent> _ParseErrorCallback;

	std::unordered_map<std::string, EventHandler> _EventHandlers;

	bool _ReadSessionKey(const nlohmann::json& data);
	void _DispatchEvent(const nlohmann::json& data);

	template <typename T>
	class _has_type_
	{
		typedef char yes_type;
		typedef long no_type;
		template <typename U> static yes_type test(decltype(&U::_TYPE_));
		template <typename U> static no_type test(...);

		public:
		static constexpr bool value = sizeof(test<T>(0)) == sizeof(yes_type);
	};

	template <typename Event>
	constexpr static void _type_check_()
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

public:
	MiraiClient();
	MiraiClient(const SessionConfigs& config);
	MiraiClient(const MiraiClient&) = delete;
	MiraiClient& operator=(const MiraiClient&) = delete;
	MiraiClient(MiraiClient&& rhs);
	MiraiClient& operator=(MiraiClient&& rhs);
	~MiraiClient();

	std::optional<std::string> GetSessionKey() const
	{
		return (this->_SessionKeySet) ? std::optional<std::string>(this->_SessionKey) : std::nullopt;
	}

	SessionConfigs GetSessionConfig() const { return this->_config; }
	QQ_t GetBotQQ() const { return this->_config.BotQQ; }
	bool isConnected() const { return this->_connected && this->_SessionKeySet; }
	constexpr std::string_view GetVersion();

	template<typename EventType>
	void On(EventCallback<EventType> callback)
	{
		_type_check_<EventType>();
		std::lock_guard<std::mutex> lk(this->_mtx);
		this->_EventHandlers[std::string(EventType::_TYPE_)] =
			[callback, this](const nlohmann::json& j)
			{
				EventType event(this);
				event.FromJson(j);
				callback(event);
			};
	}

	void SetSessionConfig(const SessionConfigs& config) { this->_config = config; };
	void SetSessionConfig(const std::string& path) { this->_config.FromFile(path); };
	void SetSessionConfig(const nlohmann::json& json_config) { this->_config.FromJson(json_config); };
	void Connect();
	void Disconnect();


	using string = std::string;
	using json = nlohmann::json;

	string GetMiraiApiHttpVersion();
	std::vector<QQ_t> GetBotList();

	FriendMessageEvent GetFriendMessage(MessageId_t id, QQ_t qq);
	GroupMessageEvent GetGroupMessage(MessageId_t id, GID_t GroupId);
	TempMessageEvent GetTempMessage(MessageId_t id, GID_t GroupId);
	StrangerMessageEvent GetStrangerMessage(MessageId_t id, QQ_t qq);

	std::vector<User> GetFriendList();
	std::vector<Group> GetGroupList();
	std::vector<GroupMember> GetMemberList(GID_t GroupId);

	UserProfile GetBotProfile();
	UserProfile GetFriendProfile(QQ_t qq);
	UserProfile GetMemberProfile(GID_t GroupId, QQ_t MemberId);
	UserProfile GetUserProfile(QQ_t qq);

	MessageId_t SendFriendMessage(QQ_t qq, const MessageChain& message, std::optional<MessageId_t> QuoteId = std::nullopt, bool ignoreInvalid = false);
	MessageId_t SendGroupMessage(GID_t GroupId, const MessageChain& message, std::optional<MessageId_t> QuoteId = std::nullopt, bool ignoreInvalid = false);
	MessageId_t SendTempMessage(QQ_t MemberId, GID_t GroupId, const MessageChain& message, std::optional<MessageId_t> QuoteId = std::nullopt, bool ignoreInvalid = false);
	void SendNudge(const NudgeTarget& target);
	void NudgeFriend(QQ_t qq);
	void NudgeGroup(QQ_t MemberId, GID_t GroupId);
	void NudgeStranger(QQ_t qq);
	void RecallFriendMessage(MessageId_t id, QQ_t qq);
	void RecallGroupMessage(MessageId_t id, GID_t GroupId);
	std::vector<MessageChain> GetRoamingFriendMessage(QQ_t qq, std::time_t TimeStart = 0, std::time_t TimeEnd = std::numeric_limits<std::time_t>::max());

	std::vector<GroupFileInfo> GetGroupFileList(GID_t GroupId, const FilePath& dir = {}, 
		int64_t offset = 0, int64_t size = 0, bool withDownloadInfo = false);
	GroupFileInfo GetGroupFileInfo(GID_t GroupId, const FilePath& dir, bool withDownloadInfo = false);
	void GetGroupFileInfo(GID_t GroupId, GroupFileInfo& file, bool withDownloadInfo = false);
	GroupFileInfo CreateGroupFileDirectory(GID_t GroupId, const string& directory);
	void RemoveGroupFile(GID_t GroupId, const FilePath& dir);
	void MoveGroupFile(GID_t GroupId, const FilePath& FileDir, const FilePath& MoveToDir);
	void RenameGroupFile(GID_t GroupId, const FilePath& FileDir, const string& NewName);
	GroupFileInfo UploadGroupFile(GID_t GroupId, const string& UploadPath, const string& name, const string& content);
	GroupFileInfo UploadGroupFile(GID_t GroupId, const string& UploadPath, const string& name, std::istream& file);
	GroupFileInfo UploadGroupFile(GID_t GroupId, const string& UploadPath, const std::filesystem::path& path);

	FriendImage UploadFriendImage(const string& content);
	FriendImage UploadFriendImage(std::istream& file);
	GroupImage UploadGroupImage(const string& content);
	GroupImage UploadGroupImage(std::istream& file);
	TempImage UploadTempImage(const string& content);
	TempImage UploadTempImage(std::istream& file);
	GroupAudio UploadGroupAudio(const string& content);
	GroupAudio UploadGroupAudio(std::istream& file);

	void DeleteFriend(QQ_t qq);

	void Mute(GID_t GroupId, QQ_t member, std::chrono::seconds time);
	void Mute(const GroupMember& member, std::chrono::seconds time);
	void Unmute(GID_t GroupId, QQ_t member);
	void Unmute(const GroupMember& member);
	void Kick(GID_t GroupId, QQ_t member, const string& message);
	void Kick(const GroupMember& member, const string& message);
	void LeaveGroup(GID_t GroupId);
	void MuteAll(GID_t GroupId);
	void UnmuteAll(GID_t GroupId);
	void SetEssence(GID_t GroupId, MessageId_t MessageId);

	GroupConfig GetGroupConfig(GID_t GroupId);
	void SetGroupConfig(GID_t GroupId, const string& name = "", std::optional<bool> AllowMemberInvite = std::nullopt);
	GroupMember GetMemberInfo(GID_t GroupId, QQ_t member);
	void SetMemberInfo(GID_t GroupId, QQ_t member, const string& name = "", const string& title = "");
	void SetGroupAdmin(GID_t GroupId, QQ_t member, bool assign = true);
	
	std::vector<GroupAnnouncement> GetAnnouncementList(GID_t GroupId, int64_t offset = 0, int64_t size = 0);
	GroupAnnouncement PublishAnnouncement(GID_t GroupId, const string& content, 
					const MiraiImage& cover = {}, 
					bool ToNewMember = false, bool pinned = false,
					bool ShowEditCard = false, bool ShowPopup = false, 
					bool RequireConfirm = false);
	void DeleteAnnouncement(GID_t GroupId, const string& fid);
	void DeleteAnnouncement(const GroupAnnouncement& announcement);

	void RespNewFriendRequestEvent(int64_t EventId, QQ_t FromId, GID_t GroupId, NewFriendRequestOp operation, const string& message);
	void RespNewFriendRequestEvent(const NewFriendRequestEvent& event, NewFriendRequestOp operation, const string& message);
	void RespMemberJoinRequestEvent(int64_t EventId, QQ_t FromId, GID_t GroupId, MemberJoinRequestOp operation, const string& message);
	void RespMemberJoinRequestEvent(const MemberJoinRequestEvent& event, MemberJoinRequestOp operation, const string& message);
	void RespBotInvitedJoinGroupRequestEvent(int64_t EventId, QQ_t FromId, GID_t GroupId, BotInvitedJoinGroupRequestOp operation, const string& message);
	void RespBotInvitedJoinGroupRequestEvent(const BotInvitedJoinGroupRequestEvent& event, BotInvitedJoinGroupRequestOp operation, const string& message);

	void RegisterCommand(const string& name, const std::vector<string>& alias, const string& usage, const string& description);
	void ExecuteCommand(const MessageChain& command);

	json PostRaw(const string& path, const string& content, const string& ContentType);
	json GetRaw(const string& path, const std::multimap<string, string> params);

};

template<>
void MiraiClient::On<ClientConnectionEstablishedEvent>(EventCallback<ClientConnectionEstablishedEvent>);
template<>
void MiraiClient::On<ClientConnectionErrorEvent>(EventCallback<ClientConnectionErrorEvent>);
template<>
void MiraiClient::On<ClientConnectionClosedEvent>(EventCallback<ClientConnectionClosedEvent>);
template<>
void MiraiClient::On<ClientParseErrorEvent>(EventCallback<ClientParseErrorEvent>);

}

#endif