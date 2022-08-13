#include <condition_variable>
#include <exception>
#include <fstream>
#include <memory>
#include <mutex>
#include <string>

#include <nlohmann/json.hpp>
#include <httplib.h>

#include <libmirai/Impl/HttpClientImpl.hpp>
#include <libmirai/Impl/MessageClientImpl.hpp>
#include <libmirai/Utils/Common.hpp>
#include <libmirai/Utils/ThreadPool.hpp>

#include <libmirai/Events/MiraiClientEvents.hpp>
#include <libmirai/Exceptions/Exceptions.hpp>
#include <libmirai/Types/Types.hpp>

#include "Client.hpp"

namespace Mirai
{

using json = nlohmann::json;

void MiraiClient::Connect()
{
	if (this->_connected)
		return;
	
	this->_SessionKey = "";
	this->_SessionKeySet = false;
	this->_HttpClient = std::make_unique<Details::HttpClientImpl>(
		this->_config.HttpUrl, 
		this->_config.ConnectionTimeout,
		this->_config.ReadTimeout,
		this->_config.WriteTimeout
	);

	this->_MessageClient = std::make_unique<Details::MessageClientImpl>(
		this->_config.HeartbeatInterval,
		this->_config.HandshakeTimeout,
		this->_config.EnablePong,
		this->_config.EnableDeflate,
		this->_config.AutoReconnect,
		this->_config.MinRetryInterval,
		this->_config.MaxRetryInterval
	);

	this->_ThreadPool = std::make_unique<Utils::ThreadPool>(this->_config.ThreadPoolSize);

	this->_MessageClient->OnOpen(
		[this](const ix::WebSocketOpenInfo& info)
		{
			this->_connected = true;
			this->_HandshakeInfo = {
				info.uri,
				{info.headers.begin(),info.headers.end()},
				info.protocol
			};
		}
	);

	this->_MessageClient->OnError(
		[this](const ix::WebSocketErrorInfo& info)
		{
			auto callback = this->_GetErrorCallback();
			if (callback)
			{
				ClientConnectionErrorEvent event {
					info.retries,
					info.wait_time,
					info.http_status,
					info.reason,
					info.decompressionError
				};
				callback(event);
			}
		}
	);

	this->_MessageClient->OnClose(
		[this](const ix::WebSocketCloseInfo& info)
		{
			this->_connected = false;
			this->_SessionKeySet = false;
			auto callback = this->_GetClosedCallback();
			if (callback)
			{
				ClientConnectionClosedEvent event {
					info.code,
					info.reason,
					info.remote
				};
				callback(event);
			}
		}
	);

	this->_MessageClient->OnText(
		[this](const std::string& message)
		{
			try
			{
				json data = json::parse(message);

				if (!data.contains("data")) return;

				/*std::string id = data.value("syncId", "");*/
				data = data.at("data");
				if (!data.is_object()) return;

				data = Utils::ParseResponse(data);

				if (!this->_SessionKeySet)	// check for sessionKey
				{
					if (this->_ReadSessionKey(data))
					{
						auto callback = this->_GetEstablishedCallback();
						if (callback)
							callback(this->_HandshakeInfo);
						return;
					}
				}

				this->_DispatchEvent(data);

			}
			catch(const ParseError& e)
			{
				auto callback = this->_GetParseErrorCallback();
				if(this->_ParseErrorCallback)
					this->_ParseErrorCallback({e, message});
			}
			catch(const std::exception& e)
			{
				auto callback = this->_GetParseErrorCallback();
				if(this->_ParseErrorCallback)
					this->_ParseErrorCallback({ParseError{e.what(), message}, message});
			}
			catch(...)
			{
				auto callback = this->_GetParseErrorCallback();
				if(this->_ParseErrorCallback)
					this->_ParseErrorCallback({ParseError{"Unknown error", message}, message});
			}
		}
	);

	this->_MessageClient->Connect(
		this->_config.WebsocketUrl 
		+ "/all?verifyKey="
		+ this->_config.VerifyKey
		+ "&qq="
		+ this->_config.BotQQ.to_string());
	
	{
		std::unique_lock<std::mutex> lk(this->_mtx);
		if (!this->_cv.wait_for(lk, std::chrono::seconds(10), 
			[this]() -> bool { return this->_SessionKeySet; }))
		{
			// failed to get sessionKey
			this->_MessageClient->Disconnect();
			throw NetworkException(-2, "Failed to receive session key from server");
		}
	}
}

void MiraiClient::Disconnect()
{
	this->_MessageClient->Disconnect();
}

bool MiraiClient::_ReadSessionKey(const json& data)
{
	if (!(data.contains("session") && data.at("session").is_string()))
		return false;

	std::string session = data.at("session").get<std::string>();

	// Check http connection
	json resp = this->_HttpClient->SessionInfo(this->_SessionKey);
	std::string validate =  Utils::GetValue(resp, "sessionKey", "");
	if (this->_SessionKey != validate)
		throw MiraiApiHttpException(-1, "Dismatched sessionKey: \"" + this->_SessionKey
		+ "\" <-> \"" + validate);
	this->_HandshakeInfo.SessionKey = session;
	this->_HandshakeInfo.BotProfile = Utils::GetValue(resp, "qq", User{});

	{
		std::unique_lock<std::mutex> lk(this->_mtx);
		this->_SessionKey = session;
		this->_SessionKeySet = true;
		this->_cv.notify_one();
	}
	return true;

}

void MiraiClient::_DispatchEvent(const json& data)
{
	if (!(data.contains("type") && data.at("type").is_string()))
		return;
	
	std::string type = data.at("type").get<std::string>();
	EventHandler handler;
	{
		std::lock_guard<std::mutex> lk(this->_mtx);
		if (this->_EventHandlers.count(type))
			handler = this->_EventHandlers.at(type);
	}
	if(handler)
	{
		auto event = handler(nullptr);
		event->_client = this;
		event->FromJson(data);
		(void)this->_ThreadPool->enqueue(handler, std::move(event));
	}
}


template<>
void MiraiClient::On(EventCallback<ClientConnectionEstablishedEvent> callback)
{
	std::lock_guard<std::mutex> lk(this->_mtx);
	this->_ConnectionEstablishedCallback = callback;
}

template<>
void MiraiClient::On(EventCallback<ClientConnectionErrorEvent> callback)
{
	std::lock_guard<std::mutex> lk(this->_mtx);
	this->_ConnectionErrorCallback = callback;
}

template<>
void MiraiClient::On(EventCallback<ClientConnectionClosedEvent> callback)
{
	std::lock_guard<std::mutex> lk(this->_mtx);
	this->_ConnectionClosedCallback = callback;
}

template<>
void MiraiClient::On(EventCallback<ClientParseErrorEvent> callback)
{
	std::lock_guard<std::mutex> lk(this->_mtx);
	this->_ParseErrorCallback = callback;
}


using std::string;

string MiraiClient::GetMiraiApiHttpVersion()
{
	json resp = this->_HttpClient->About();
	return Utils::GetValue(resp, "version", "");
}

std::vector<QQ_t> MiraiClient::GetBotList()
{
	json resp = this->_HttpClient->BotList();
	return resp.get<std::vector<QQ_t>>();
}


FriendMessageEvent MiraiClient::GetFriendMessage(MessageId_t id, QQ_t qq)
{
	json resp = this->_HttpClient->MessageFromId(this->_SessionKey, id, qq);
	FriendMessageEvent event;
	if (event.GetType() != Utils::GetValue(resp, "type", ""))
		throw TypeDismatch(string(event.GetType()), Utils::GetValue(resp, "type", ""));
	event.FromJson(resp);
	return event;
}

GroupMessageEvent MiraiClient::GetGroupMessage(MessageId_t id, GID_t GroupId)
{
	json resp = this->_HttpClient->MessageFromId(this->_SessionKey, id, GroupId);
	GroupMessageEvent event;
	if (event.GetType() != Utils::GetValue(resp, "type", ""))
		throw TypeDismatch(string(event.GetType()), Utils::GetValue(resp, "type", ""));
	event.FromJson(resp);
	return event;
}

TempMessageEvent MiraiClient::GetTempMessage(MessageId_t id, GID_t GroupId)
{
	json resp = this->_HttpClient->MessageFromId(this->_SessionKey, id, GroupId);
	TempMessageEvent event;
	if (event.GetType() != Utils::GetValue(resp, "type", ""))
		throw TypeDismatch(string(event.GetType()), Utils::GetValue(resp, "type", ""));
	event.FromJson(resp);
	return event;
}

StrangerMessageEvent MiraiClient::GetStrangerMessage(MessageId_t id, QQ_t qq)
{
	json resp = this->_HttpClient->MessageFromId(this->_SessionKey, id, qq);
	StrangerMessageEvent event;
	if (event.GetType() != Utils::GetValue(resp, "type", ""))
		throw TypeDismatch(string(event.GetType()), Utils::GetValue(resp, "type", ""));
	event.FromJson(resp);
	return event;
}



std::vector<User> MiraiClient::GetFriendList()
{
	json resp = this->_HttpClient->FriendList(this->_SessionKey);
	return resp.get<std::vector<User>>();
}

std::vector<Group> MiraiClient::GetGroupList()
{
	json resp = this->_HttpClient->GroupList(this->_SessionKey);
	return resp.get<std::vector<Group>>();
}

std::vector<GroupMember> MiraiClient::GetMemberList(GID_t GroupId)
{
	json resp = this->_HttpClient->MemberList(this->_SessionKey, GroupId);
	return resp.get<std::vector<GroupMember>>();
}


UserProfile MiraiClient::GetBotProfile()
{
	json resp = this->_HttpClient->BotProfile(this->_SessionKey);
	return resp.get<UserProfile>();
}

UserProfile MiraiClient::GetFriendProfile(QQ_t qq)
{
	json resp = this->_HttpClient->FriendProfile(this->_SessionKey, qq);
	return resp.get<UserProfile>();
}

UserProfile MiraiClient::GetMemberProfile(GID_t GroupId, QQ_t MemberId)
{
	json resp = this->_HttpClient->MemberProfile(this->_SessionKey, GroupId, MemberId);
	return resp.get<UserProfile>();
}

UserProfile MiraiClient::GetUserProfile(QQ_t qq)
{
	json resp = this->_HttpClient->UserProfile(this->_SessionKey, qq);
	return resp.get<UserProfile>();
}

MessageId_t MiraiClient::SendFriendMessage(QQ_t qq, const MessageChain& message, bool ignoreInvalid)
{
	json resp = this->_HttpClient->SendFriendMessage(this->_SessionKey, qq, message.ToJson(ignoreInvalid));
	return Utils::GetValue(resp, "messageId", (MessageId_t)0);
}

MessageId_t MiraiClient::SendGroupMessage(GID_t GroupId, const MessageChain& message, bool ignoreInvalid)
{
	json resp = this->_HttpClient->SendGroupMessage(this->_SessionKey, GroupId, message.ToJson(ignoreInvalid));
	return Utils::GetValue(resp, "messageId", (MessageId_t)0);
}

MessageId_t MiraiClient::SendTempMessage(QQ_t MemberId, GID_t GroupId, const MessageChain& message, bool ignoreInvalid)
{
	json resp = this->_HttpClient->SendTempMessage(this->_SessionKey, MemberId, GroupId, message.ToJson(ignoreInvalid));
	return Utils::GetValue(resp, "messageId", (MessageId_t)0);
}

void MiraiClient::SendNudge(const NudgeTarget& target)
{
	if (target.GetTargetKind() == NudgeTarget::GROUP)
		(void)this->_HttpClient->SendNudge(this->_SessionKey, target.GetTarget(), target.GetGroup(), target.GetTargetKindStr());
	else
		(void)this->_HttpClient->SendNudge(this->_SessionKey, target.GetTarget(), target.GetTarget(), target.GetTargetKindStr());
}

void MiraiClient::NudgeFriend(QQ_t qq)
{
	(void)this->_HttpClient->SendNudge(this->_SessionKey, qq, qq, "Friend");
}

void MiraiClient::NudgeGroup(QQ_t MemberId, GID_t GroupId)
{
	(void)this->_HttpClient->SendNudge(this->_SessionKey, MemberId, GroupId, "Group");
}

void MiraiClient::NudgeStranger(QQ_t qq)
{
	(void)this->_HttpClient->SendNudge(this->_SessionKey, qq, qq, "Stranger");
}

void MiraiClient::RecallFriendMessage(MessageId_t id, QQ_t qq)
{
	(void)this->_HttpClient->Recall(this->_SessionKey, id, qq);
}
void MiraiClient::RecallGroupMessage(MessageId_t id, GID_t GroupId)
{
	(void)this->_HttpClient->Recall(this->_SessionKey, id, GroupId);
}


std::vector<GroupFileInfo> MiraiClient::GetGroupFileList(GID_t GroupId, const FilePath& dir, int64_t offset,
							int64_t size, bool withDownloadInfo)
{
	json resp = this->_HttpClient->FileList(
		this->_SessionKey,
		dir.GetId(),
		dir.GetPath(),
		GroupId,
		offset,
		size,
		withDownloadInfo
	);
	return resp.get<std::vector<GroupFileInfo>>();
}

GroupFileInfo MiraiClient::GetGroupFileInfo(GID_t GroupId, const FilePath& dir, bool withDownloadInfo)
{
	json resp = this->_HttpClient->FileInfo(
		this->_SessionKey,
		dir.GetId(),
		dir.GetPath(),
		GroupId,
		withDownloadInfo
	);
	return resp.get<GroupFileInfo>();
}

void MiraiClient::GetGroupFileInfo(GID_t GroupId, GroupFileInfo& file, bool withDownloadInfo)
{
	json resp = this->_HttpClient->FileInfo(
		this->_SessionKey,
		file.id,
		"",
		GroupId,
		withDownloadInfo
	);
	file = resp.get<GroupFileInfo>();
}

GroupFileInfo MiraiClient::CreateGroupFileDirectory(GID_t GroupId, const string& directory)
{
	json resp = this->_HttpClient->FileMkdir(
		this->_SessionKey,
		"",
		"",
		GroupId,
		directory
	);
	return resp.get<GroupFileInfo>();
}

void MiraiClient::RemoveGroupFile(GID_t GroupId, const FilePath& dir)
{
	(void)this->_HttpClient->FileDelete(
		this->_SessionKey, 
		dir.GetId(), dir.GetPath(), 
		GroupId);
}

void MiraiClient::MoveGroupFile(GID_t GroupId, const FilePath& FileDir, const FilePath& MoveToDir)
{
	(void)this->_HttpClient->FileMove(
		this->_SessionKey, 
		FileDir.GetId(), FileDir.GetPath(), 
		GroupId,
		MoveToDir.GetId(), MoveToDir.GetPath()
	);
}

void MiraiClient::RenameGroupFile(GID_t GroupId, const FilePath& FileDir, const string& NewName)
{
	(void)this->_HttpClient->FileRename(
		this->_SessionKey, 
		FileDir.GetId(), FileDir.GetPath(),
		GroupId,
		NewName
	);
}

GroupFileInfo MiraiClient::UploadGroupFile(GID_t GroupId, const string& UploadPath, const string& name, const string& content)
{
	json resp = this->_HttpClient->FileUpload(
		this->_SessionKey, 
		UploadPath, 
		GroupId, "group", 
		name, content);
	return resp.get<GroupFileInfo>();
}

GroupFileInfo MiraiClient::UploadGroupFile(GID_t GroupId, const string& UploadPath, const string& name, std::istream& file)
{
	string s;
	char buffer[4096];
	while (file.read(buffer, sizeof(buffer)))
		s.append(buffer, sizeof(buffer));
	s.append(buffer, file.gcount());
	return this->UploadGroupFile(GroupId, UploadPath, name, s);
}

GroupFileInfo MiraiClient::UploadGroupFile(GID_t GroupId, const string& UploadPath, const std::filesystem::path& path)
{
	string name = (path.has_filename())? path.filename() : path;
	std::ifstream f(path, std::ios_base::binary);
	return this->UploadGroupFile(GroupId, UploadPath, name, f);
}

FriendImage MiraiClient::UploadFriendImage(const string& content)
{
	json resp = this->_HttpClient->UploadImage(
		this->_SessionKey, 
		"friend", 
		content);
	return resp.get<FriendImage>();
}

FriendImage MiraiClient::UploadFriendImage(std::istream& file)
{
	string s;
	char buffer[4096];
	while (file.read(buffer, sizeof(buffer)))
		s.append(buffer, sizeof(buffer));
	s.append(buffer, file.gcount());
	return this->UploadFriendImage(s);
}

GroupImage MiraiClient::UploadGroupImage(const string& content)
{
	json resp = this->_HttpClient->UploadImage(
		this->_SessionKey, 
		"group", 
		content);
	return resp.get<GroupImage>();
}

GroupImage MiraiClient::UploadGroupImage(std::istream& file)
{
	string s;
	char buffer[4096];
	while (file.read(buffer, sizeof(buffer)))
		s.append(buffer, sizeof(buffer));
	s.append(buffer, file.gcount());
	return this->UploadGroupImage(s);
}

TempImage MiraiClient::UploadTempImage(const string& content)
{
	json resp = this->_HttpClient->UploadImage(
		this->_SessionKey, 
		"temp", 
		content);
	return resp.get<TempImage>();
}

TempImage MiraiClient::UploadTempImage(std::istream& file)
{
	string s;
	char buffer[4096];
	while (file.read(buffer, sizeof(buffer)))
		s.append(buffer, sizeof(buffer));
	s.append(buffer, file.gcount());
	return this->UploadTempImage(s);
}

GroupAudio MiraiClient::UploadGroupAudio(const string& content)
{
	json resp = this->_HttpClient->UploadAudio(
		this->_SessionKey, 
		"group", 
		content);
	return resp.get<GroupAudio>();
}
GroupAudio MiraiClient::UploadGroupAudio(std::istream& file)
{
	string s;
	char buffer[4096];
	while (file.read(buffer, sizeof(buffer)))
		s.append(buffer, sizeof(buffer));
	s.append(buffer, file.gcount());
	return this->UploadGroupAudio(s);
}


void MiraiClient::DeleteFriend(QQ_t qq)
{
	(void)this->_HttpClient->DeleteFriend(this->_SessionKey, qq);
}

void MiraiClient::Mute(GID_t GroupId, QQ_t member, std::chrono::seconds time)
{
	(void)this->_HttpClient->Mute(this->_SessionKey, GroupId, member, time);
}

void MiraiClient::Mute(const GroupMember& member, std::chrono::seconds time)
{
	(void)this->_HttpClient->Mute(this->_SessionKey, member.group.id, member.id, time);
}

void MiraiClient::Unmute(GID_t GroupId, QQ_t member)
{
	(void)this->_HttpClient->Unmute(this->_SessionKey, GroupId, member);
}

void MiraiClient::Unmute(const GroupMember& member)
{
	(void)this->_HttpClient->Unmute(this->_SessionKey, member.group.id, member.id);
}

void MiraiClient::Kick(GID_t GroupId, QQ_t member, const string& message)
{
	(void)this->_HttpClient->Kick(this->_SessionKey, GroupId, member, message);
}

void MiraiClient::Kick(const GroupMember& member, const string& message)
{
	(void)this->_HttpClient->Kick(this->_SessionKey, member.group.id, member.id, message);
}

void MiraiClient::LeaveGroup(GID_t GroupId)
{
	(void)this->_HttpClient->Quit(this->_SessionKey, GroupId);
}

void MiraiClient::MuteAll(GID_t GroupId)
{
	(void)this->_HttpClient->MuteAll(this->_SessionKey, GroupId);
}

void MiraiClient::UnmuteAll(GID_t GroupId)
{
	(void)this->_HttpClient->UnmuteAll(this->_SessionKey, GroupId);
}

void MiraiClient::SetEssence(GID_t GroupId, MessageId_t MessageId)
{
	(void)this->_HttpClient->SetEssence(this->_SessionKey, MessageId, GroupId);
}


GroupConfig MiraiClient::GetGroupConfig(GID_t GroupId)
{
	json resp = this->_HttpClient->GetGroupConfig(this->_SessionKey, GroupId);
	return resp.get<GroupConfig>();
}

void MiraiClient::SetGroupConfig(GID_t GroupId, const string& name, std::optional<bool> AllowMemberInvite)
{
	(void)this->_HttpClient->PostGroupConfig(
		this->_SessionKey, GroupId, 
		name, std::nullopt, AllowMemberInvite
	);
}

GroupMember MiraiClient::GetMemberInfo(GID_t GroupId, QQ_t member)
{
	json resp = this->_HttpClient->GetMemberInfo(this->_SessionKey, GroupId, member);
	return resp.get<GroupMember>();
}

void MiraiClient::SetMemberInfo(GID_t GroupId, QQ_t member, const string& name, const string& title)
{
	(void)this->_HttpClient->PostMemberInfo(
		this->_SessionKey, GroupId, member,
		name, title
	);
}

void MiraiClient::SetGroupAdmin(GID_t GroupId, QQ_t member, bool assign)
{
	(void)this->_HttpClient->MemberAdmin(
		this->_SessionKey, GroupId, member,
		assign
	);
}


std::vector<GroupAnnouncement> MiraiClient::GetAnnouncementList(GID_t GroupId, int64_t offset, int64_t size)
{
	json resp = this->_HttpClient->AnnoList(this->_SessionKey, GroupId, offset, size);
	return resp.get<std::vector<GroupAnnouncement>>();
}

GroupAnnouncement MiraiClient::PublishAnnouncement(GID_t GroupId, const string& content,
						const MiraiImage& cover, bool ToNewMember, bool pinned, 
						bool ShowEditCard, bool ShowPopup, bool RequireConfirm)
{
	json resp = this->_HttpClient->AnnoPublish(
		this->_SessionKey, GroupId, content,
		cover.url, cover.path, cover.base64,
		ToNewMember, pinned, ShowEditCard,
		ShowPopup, RequireConfirm
	);
	return resp.get<GroupAnnouncement>();
}

void MiraiClient::DeleteAnnouncement(GID_t GroupId, const string& fid)
{
	(void)this->_HttpClient->AnnoDelete(this->_SessionKey, GroupId, fid);
}

void MiraiClient::DeleteAnnouncement(const GroupAnnouncement& announcement)
{
	(void)this->_HttpClient->AnnoDelete(this->_SessionKey, announcement.group.id, announcement.fid);
}


void MiraiClient::RespNewFriendRequestEvent(int64_t EventId, QQ_t FromId, GID_t GroupId, NewFriendRequestOp operation, const string& message)
{
	(void)this->_HttpClient->RespNewFriendRequestEvent(
		this->_SessionKey,
		EventId, FromId, GroupId,
		(int)operation, message
	);
}

void MiraiClient::RespNewFriendRequestEvent(const NewFriendRequestEvent& event, NewFriendRequestOp operation, const string& message)
{
	(void)this->_HttpClient->RespNewFriendRequestEvent(
		this->_SessionKey,
		event.GetEventId(), event.GetUserId(), event.GetGroupId(),
		(int)operation, message
	);
}

void MiraiClient::RespMemberJoinRequestEvent(int64_t EventId, QQ_t FromId, GID_t GroupId, MemberJoinRequestOp operation, const string& message)
{
	(void)this->_HttpClient->RespMemberJoinRequestEvent(
		this->_SessionKey,
		EventId, FromId, GroupId,
		(int)operation, message
	);
}

void MiraiClient::RespMemberJoinRequestEvent(const MemberJoinRequestEvent& event, MemberJoinRequestOp operation, const string& message)
{
	(void)this->_HttpClient->RespMemberJoinRequestEvent(
		this->_SessionKey,
		event.GetEventId(), event.GetUserId(), event.GetGroupId(),
		(int)operation, message
	);
}

void MiraiClient::RespBotInvitedJoinGroupRequestEvent(int64_t EventId, QQ_t FromId, GID_t GroupId, BotInvitedJoinGroupRequestOp operation, const string& message)
{
	(void)this->_HttpClient->RespBotInvitedJoinGroupRequestEvent(
		this->_SessionKey,
		EventId, FromId, GroupId,
		(int)operation, message
	);
}

void MiraiClient::RespBotInvitedJoinGroupRequestEvent(const BotInvitedJoinGroupRequestEvent& event, BotInvitedJoinGroupRequestOp operation, const string& message)
{
	(void)this->_HttpClient->RespBotInvitedJoinGroupRequestEvent(
		this->_SessionKey,
		event.GetEventId(), event.GetUserId(), event.GetGroupId(),
		(int)operation, message
	);
}


void MiraiClient::RegisterCommand(const string& name, const std::vector<string>& alias, const string& usage, const string& description)
{
	(void)this->_HttpClient->CmdRegister(this->_SessionKey, name, alias, usage, description);
}
void MiraiClient::ExecuteCommand(const MessageChain& command)
{
	(void)this->_HttpClient->CmdExecute(this->_SessionKey, command.ToJson(false));
}


json MiraiClient::PostRaw(const string& path, const string& content, const string& ContentType)
{
	return this->_HttpClient->PostRaw(path, content, ContentType);
}

json MiraiClient::GetRaw(const string& path, const std::multimap<string, string> params)
{
	return this->_HttpClient->GetRaw(path, params);
}

}