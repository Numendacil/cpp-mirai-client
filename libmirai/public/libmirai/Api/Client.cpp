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

#include "Client.hpp"

#include <condition_variable>
#include <exception>
#include <fstream>
#include <memory>
#include <mutex>
#include <string>

#include <nlohmann/json.hpp>

#include <libmirai/Events/BotInvitedJoinGroupRequestEvent.hpp>
#include <libmirai/Events/FriendMessageEvent.hpp>
#include <libmirai/Events/GroupMessageEvent.hpp>
#include <libmirai/Events/MemberJoinRequestEvent.hpp>
#include <libmirai/Events/MiraiClientEvents.hpp>
#include <libmirai/Events/NewFriendRequestEvent.hpp>
#include <libmirai/Events/StrangerMessageEvent.hpp>
#include <libmirai/Events/TempMessageEvent.hpp>
#include <libmirai/Exceptions/Exceptions.hpp>
#include <libmirai/Impl/HttpClientImpl.hpp>
#include <libmirai/Impl/MessageClientImpl.hpp>
#include <libmirai/Messages/MessageChain.hpp>
#include <libmirai/Types/Types.hpp>
#include <libmirai/Utils/Common.hpp>
#include <libmirai/Utils/ThreadPool.hpp>

#include "libmirai/Utils/Logger.hpp"

namespace Mirai
{

using json = nlohmann::json;

MiraiClient::MiraiClient()
{
	this->_logger = std::make_shared<NullLogger>();
}

MiraiClient::MiraiClient(SessionConfigs config) : _config(std::move(config)) {}

MiraiClient::MiraiClient(MiraiClient&& rhs) noexcept
{
	*this = std::move(rhs);
}

MiraiClient& MiraiClient::operator=(MiraiClient&& rhs) noexcept
{
	if (this != &rhs)
	{
		std::lock_guard<std::mutex> lk(this->_mtx);
		this->_config = std::move(rhs._config);
		this->_SessionKey = std::move(rhs._SessionKey);
		this->_SessionKeySet = rhs._SessionKeySet.load();
		this->_connected = rhs._connected;
		this->_HttpClient = std::move(rhs._HttpClient);
		this->_MessageClient = std::move(rhs._MessageClient);
		this->_ThreadPool = std::move(rhs._ThreadPool);
	}
	return *this;
}

MiraiClient::~MiraiClient() = default;

void MiraiClient::Connect()
{
	std::unique_lock<std::mutex> lk(this->_mtx);
	if (this->_connected) return;

	LOG_DEBUG(this->_GetLogger(), "Connecting to Mirai-Api-Http, initializing clients and threadpool");

	this->_SessionKey = "";
	this->_SessionKeySet = false;

	this->_ThreadPool = std::make_unique<Utils::ThreadPool>(this->_config.ThreadPoolSize);

	LOG_DEBUG(this->_GetLogger(), "Threadpool initialized");

	this->_HttpClient = std::make_unique<Details::HttpClientImpl>(
		this->_config.HttpUrl, this->_config.ConnectionTimeout, this->_config.ReadTimeout, this->_config.WriteTimeout);

	this->_MessageClient = std::make_unique<Details::MessageClientImpl>(
		this->_config.HeartbeatInterval, this->_config.HandshakeTimeout, this->_config.EnablePong,
		this->_config.EnableDeflate, this->_config.AutoReconnect, this->_config.MinRetryInterval,
		this->_config.MaxRetryInterval);

	this->_MessageClient->OnOpen(
		[this](const ix::WebSocketOpenInfo& info)
		{
			LOG_DEBUG(
				this->_GetLogger(),
				"MessageClient connected with response header: " + [&]() -> std::string
				{
					std::string header;
					for (const auto& p : info.headers)
						header += "\n" + p.first + ": " + p.second;
					return header;
				}());

			this->_HandshakeInfo = {info.uri, {info.headers.begin(), info.headers.end()}, info.protocol};
		});

	this->_MessageClient->OnError(
		[this](const ix::WebSocketErrorInfo& info)
		{
			LOG_DEBUG(
				this->_GetLogger(),
				"MessageClient failed to connect with error: " + [&]() -> std::string
				{
					std::string msg;
					msg += "\nHTTP Status: " + std::to_string(info.http_status);
					msg += "\nReason: " + info.reason;
					msg += "\nDecompressionError: ";
					msg += info.decompressionError ? "True" : "False";
					return msg;
				}());

			auto callback = this->_GetErrorCallback();
			if (callback)
			{
				ClientConnectionErrorEvent event{info.retries, info.wait_time, info.http_status, info.reason,
			                                     info.decompressionError};
				callback(event);
			}
		});

	this->_MessageClient->OnClose(
		[this](const ix::WebSocketCloseInfo& info)
		{
			LOG_DEBUG(
				this->_GetLogger(),
				"MessageClient connection closed: " + [&]() -> std::string
				{
					std::string msg;
					msg += "\nCode: " + std::to_string(info.code);
					msg += "\nReason: " + info.reason;
					msg += "\nClosedByRemote: ";
					msg += info.remote ? "True" : "False";
					return msg;
				}());

			{
				std::lock_guard<std::mutex> lk(this->_mtx);
				this->_connected = false;
			}
			this->_SessionKeySet = false;
			auto callback = this->_GetClosedCallback();
			if (callback)
			{
				ClientConnectionClosedEvent event{info.code, info.reason, info.remote};
				callback(event);
			}
		});

	this->_MessageClient->OnText(
		[this](const std::string& message)
		{
			LOG_TRACE(this->_GetLogger(), "Received: " + message);

			try
			{
				json msg = json::parse(message);

				if (!msg.contains("data")) return;

				/*std::string id = msg.value("syncId", "");*/
				json data = msg.at("data");
				if (!data.is_object()) return;

				json resp = Utils::ParseResponse(data);

				if (!this->_SessionKeySet) // check for sessionKey
				{
					if (this->_ReadSessionKey(data))
					{
						return;
					}
				}

				this->_DispatchEvent(data);
			}
			catch (const ParseError& e)
			{
				auto callback = this->_GetParseErrorCallback();
				if (this->_ParseErrorCallback) this->_ParseErrorCallback({e, message});
			}
			catch (const std::exception& e)
			{
				auto callback = this->_GetParseErrorCallback();
				if (this->_ParseErrorCallback) this->_ParseErrorCallback({ParseError{e.what(), message}, message});
			}
			catch (...)
			{
				auto callback = this->_GetParseErrorCallback();
				if (this->_ParseErrorCallback)
					this->_ParseErrorCallback({ParseError{"Unknown error", message}, message});
			}
		});

	LOG_DEBUG(this->_GetLogger(), "Clients initialized");

	LOG_DEBUG(this->_GetLogger(),
	          "Calling connect from MessageClient: " + this->_config.WebsocketUrl
	              + "/all?verifyKey=" + this->_config.VerifyKey + "&qq=" + this->_config.BotQQ.to_string());

	this->_MessageClient->Connect(this->_config.WebsocketUrl + "/all?verifyKey=" + this->_config.VerifyKey
	                              + "&qq=" + this->_config.BotQQ.to_string());

	if (!this->_cv.wait_for(lk, std::chrono::seconds(10), [this]() -> bool { return this->_connected; }))
	{
		LOG_DEBUG(this->_GetLogger(), "Timeout waiting for session key, closing down");

		// failed to get sessionKey
		this->_MessageClient->Disconnect();
		throw NetworkException(-2, "Failed to receive session key from server");
	}

	LOG_DEBUG(this->_GetLogger(), "Successfully connected");
}

void MiraiClient::Disconnect()
{
	std::unique_lock<std::mutex> lk(this->_mtx);
	if (this->_MessageClient)
	{
		this->_MessageClient->Disconnect();
		LOG_DEBUG(this->_GetLogger(), "Successfully disconnected");
	}
	this->_ThreadPool = nullptr;

	LOG_DEBUG(this->_GetLogger(), "Threadpool shutdown complete");

	this->_HttpClient = nullptr;
	this->_MessageClient = nullptr;

	LOG_DEBUG(this->_GetLogger(), "Clients shutdown complete");
}

bool MiraiClient::_ReadSessionKey(const json& data)
{
	if (!(data.contains("session") && data.at("session").is_string())) return false;

	std::string session = data.at("session").get<std::string>();

	LOG_DEBUG(this->_GetLogger(), "Session key obtained: " + session);

	// Check http connection
	LOG_DEBUG(this->_GetLogger(), "Checking HTTP connection, calling /sessionInfo with session key: " + session);

	json resp = this->_HttpClient->SessionInfo(session);

	LOG_DEBUG(this->_GetLogger(), "Checking HTTP connection, received: " + resp.dump());

	std::string validate = Utils::GetValue(resp, "sessionKey", "");
	if (session != validate)
		throw MiraiApiHttpException(-1, "Dismatched sessionKey: \"" + session + "\" <-> \"" + validate);
	
	ClientConnectionEstablishedEvent event;
	{
		std::unique_lock<std::mutex> lk(this->_mtx);
		this->_SessionKey = session;
		this->_SessionKeySet = true;
		this->_connected = true;
		this->_HandshakeInfo.SessionKey = session;
		this->_HandshakeInfo.BotProfile = Utils::GetValue(resp, "qq", User{});
		event = this->_HandshakeInfo;
	}

	auto callback = this->_GetEstablishedCallback();
	if (callback) callback(event);

	this->_cv.notify_one();
	return true;
}

void MiraiClient::_DispatchEvent(const json& data)
{
	if (!(data.contains("type") && data.at("type").is_string())) return;

	std::string type = data.at("type").get<std::string>();

	LOG_DEBUG(this->_GetLogger(), "Dispatching message, type: " + type);

	EventHandler handler;
	{
		std::lock_guard<std::mutex> lk(this->_mtx);
		if (this->_EventHandlers.count(type)) handler = this->_EventHandlers.at(type);
	}
	if (handler)
	{
		LOG_DEBUG(this->_GetLogger(), "Found handler");
		(void)this->_ThreadPool->enqueue(
			[data, handler, this]()
			{
				try
				{
					handler(data);
				}
				catch (const ParseError& e)
				{
					auto callback = this->_GetParseErrorCallback();
					if (this->_ParseErrorCallback) this->_ParseErrorCallback({e, data.dump()});
				}
			});
		return;
	}
	LOG_DEBUG(this->_GetLogger(), "No matching handler found");
}


template<>
void MiraiClient::On<ClientConnectionEstablishedEvent>(EventCallback<ClientConnectionEstablishedEvent> callback)
{
	std::lock_guard<std::mutex> lk(this->_mtx);
	this->_ConnectionEstablishedCallback = callback;
}

template<> void MiraiClient::On<ClientConnectionErrorEvent>(EventCallback<ClientConnectionErrorEvent> callback)
{
	std::lock_guard<std::mutex> lk(this->_mtx);
	this->_ConnectionErrorCallback = callback;
}

template<> void MiraiClient::On<ClientConnectionClosedEvent>(EventCallback<ClientConnectionClosedEvent> callback)
{
	std::lock_guard<std::mutex> lk(this->_mtx);
	this->_ConnectionClosedCallback = callback;
}

template<> void MiraiClient::On<ClientParseErrorEvent>(EventCallback<ClientParseErrorEvent> callback)
{
	std::lock_guard<std::mutex> lk(this->_mtx);
	this->_ParseErrorCallback = callback;
}


using std::string;

string MiraiClient::GetMiraiApiHttpVersion()
{
	json resp = this->_HttpClient->About();

	LOG_TRACE(this->_GetLogger(), "Calling About received " + resp.dump());

	return Utils::GetValue(resp, "version", "");
}

std::vector<QQ_t> MiraiClient::GetBotList()
{
	json resp = this->_HttpClient->BotList();

	LOG_TRACE(this->_GetLogger(), "Calling BotList received " + resp.dump());

	return resp.get<std::vector<QQ_t>>();
}


FriendMessageEvent MiraiClient::GetFriendMessage(MessageId_t id, QQ_t qq)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->MessageFromId(SessionKey, id, qq);

	LOG_TRACE(this->_GetLogger(), "Calling MessageFromId received " + resp.dump());

	FriendMessageEvent event(nullptr);
	if (event.GetType() != Utils::GetValue(resp, "type", ""))
		throw TypeDismatch(string(event.GetType()), Utils::GetValue(resp, "type", ""));
	event.FromJson(resp);
	return event;
}

GroupMessageEvent MiraiClient::GetGroupMessage(MessageId_t id, GID_t GroupId)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->MessageFromId(SessionKey, id, GroupId);

	LOG_TRACE(this->_GetLogger(), "Calling MessageFromId received " + resp.dump());

	GroupMessageEvent event(nullptr);
	if (event.GetType() != Utils::GetValue(resp, "type", ""))
		throw TypeDismatch(string(event.GetType()), Utils::GetValue(resp, "type", ""));
	event.FromJson(resp);
	return event;
}

TempMessageEvent MiraiClient::GetTempMessage(MessageId_t id, GID_t GroupId)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->MessageFromId(SessionKey, id, GroupId);

	LOG_TRACE(this->_GetLogger(), "Calling MessageFromId received " + resp.dump());

	TempMessageEvent event(nullptr);
	if (event.GetType() != Utils::GetValue(resp, "type", ""))
		throw TypeDismatch(string(event.GetType()), Utils::GetValue(resp, "type", ""));
	event.FromJson(resp);
	return event;
}

StrangerMessageEvent MiraiClient::GetStrangerMessage(MessageId_t id, QQ_t qq)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->MessageFromId(SessionKey, id, qq);

	LOG_TRACE(this->_GetLogger(), "Calling MessageFromId received " + resp.dump());

	StrangerMessageEvent event(nullptr);
	if (event.GetType() != Utils::GetValue(resp, "type", ""))
		throw TypeDismatch(string(event.GetType()), Utils::GetValue(resp, "type", ""));
	event.FromJson(resp);
	return event;
}


std::vector<User> MiraiClient::GetFriendList()
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->FriendList(SessionKey);

	LOG_TRACE(this->_GetLogger(), "Calling FriendList received " + resp.dump());

	return resp.get<std::vector<User>>();
}

std::vector<Group> MiraiClient::GetGroupList()
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->GroupList(SessionKey);

	LOG_TRACE(this->_GetLogger(), "Calling GroupList received " + resp.dump());

	return resp.get<std::vector<Group>>();
}

std::vector<GroupMember> MiraiClient::GetMemberList(GID_t GroupId)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->MemberList(SessionKey, GroupId);

	LOG_TRACE(this->_GetLogger(), "Calling MemberList received " + resp.dump());

	return resp.get<std::vector<GroupMember>>();
}


UserProfile MiraiClient::GetBotProfile()
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->BotProfile(SessionKey);

	LOG_TRACE(this->_GetLogger(), "Calling BotProfile received " + resp.dump());

	return resp.get<UserProfile>();
}

UserProfile MiraiClient::GetFriendProfile(QQ_t qq)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->FriendProfile(SessionKey, qq);

	LOG_TRACE(this->_GetLogger(), "Calling FriendProfile received " + resp.dump());

	return resp.get<UserProfile>();
}

UserProfile MiraiClient::GetMemberProfile(GID_t GroupId, QQ_t MemberId)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->MemberProfile(SessionKey, GroupId, MemberId);

	LOG_TRACE(this->_GetLogger(), "Calling MemberProfile received " + resp.dump());

	return resp.get<UserProfile>();
}

UserProfile MiraiClient::GetUserProfile(QQ_t qq)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->UserProfile(SessionKey, qq);

	LOG_TRACE(this->_GetLogger(), "Calling UserProfile received " + resp.dump());

	return resp.get<UserProfile>();
}

MessageId_t MiraiClient::SendFriendMessage(QQ_t qq, const MessageChain& message, std::optional<MessageId_t> QuoteId,
                                           bool ignoreInvalid)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->SendFriendMessage(SessionKey, qq, message.ToJson(ignoreInvalid), QuoteId);

	LOG_TRACE(this->_GetLogger(), "Calling SendFriendMessage received " + resp.dump());

	return Utils::GetValue(resp, "messageId", (MessageId_t)0);
}

MessageId_t MiraiClient::SendGroupMessage(GID_t GroupId, const MessageChain& message,
                                          std::optional<MessageId_t> QuoteId, bool ignoreInvalid)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->SendGroupMessage(SessionKey, GroupId, message.ToJson(ignoreInvalid), QuoteId);

	LOG_TRACE(this->_GetLogger(), "Calling SendGroupMessage received " + resp.dump());

	return Utils::GetValue(resp, "messageId", (MessageId_t)0);
}

MessageId_t MiraiClient::SendTempMessage(QQ_t MemberId, GID_t GroupId, const MessageChain& message,
                                         std::optional<MessageId_t> QuoteId, bool ignoreInvalid)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->SendTempMessage(SessionKey, MemberId, GroupId, message.ToJson(ignoreInvalid),
	                                               QuoteId);

	LOG_TRACE(this->_GetLogger(), "Calling SendTempMessage received " + resp.dump());

	return Utils::GetValue(resp, "messageId", (MessageId_t)0);
}

void MiraiClient::SendNudge(const NudgeTarget& target)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp;
	if (target.GetTargetKind() == NudgeTarget::GROUP)
		resp = this->_HttpClient->SendNudge(SessionKey, target.GetTarget(), target.GetGroup(),
		                                    target.GetTargetKindStr());
	else
		resp = this->_HttpClient->SendNudge(SessionKey, target.GetTarget(), target.GetTarget(),
		                                    target.GetTargetKindStr());

	LOG_TRACE(this->_GetLogger(), "Calling SendNudge received " + resp.dump());
}

void MiraiClient::NudgeFriend(QQ_t qq)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->SendNudge(SessionKey, qq, qq, "Friend");

	LOG_TRACE(this->_GetLogger(), "Calling SendNudge received " + resp.dump());
}

void MiraiClient::NudgeGroup(QQ_t MemberId, GID_t GroupId)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->SendNudge(SessionKey, MemberId, GroupId, "Group");

	LOG_TRACE(this->_GetLogger(), "Calling SendNudge received " + resp.dump());
}

void MiraiClient::NudgeStranger(QQ_t qq)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->SendNudge(SessionKey, qq, qq, "Stranger");

	LOG_TRACE(this->_GetLogger(), "Calling SendNudge received " + resp.dump());
}

void MiraiClient::RecallFriendMessage(MessageId_t id, QQ_t qq)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->Recall(SessionKey, id, qq);

	LOG_TRACE(this->_GetLogger(), "Calling Recall received " + resp.dump());
}

void MiraiClient::RecallGroupMessage(MessageId_t id, GID_t GroupId)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->Recall(SessionKey, id, GroupId);

	LOG_TRACE(this->_GetLogger(), "Calling Recall received " + resp.dump());
}

std::vector<MessageChain> MiraiClient::GetRoamingFriendMessage(QQ_t qq, std::time_t TimeStart, std::time_t TimeEnd)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->RoamingMessages(SessionKey, TimeStart, TimeEnd, qq);

	LOG_TRACE(this->_GetLogger(), "Calling RoamingMessages received " + resp.dump());

	return resp.get<std::vector<MessageChain>>();
}


std::vector<GroupFileInfo> MiraiClient::GetGroupFileList(GID_t GroupId, const FilePath& dir, int64_t offset,
                                                         int64_t size, bool withDownloadInfo)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->FileList(SessionKey, dir.GetId(), dir.GetPath(), GroupId, offset, size,
	                                        withDownloadInfo);

	LOG_TRACE(this->_GetLogger(), "Calling FileList received " + resp.dump());

	return resp.get<std::vector<GroupFileInfo>>();
}

GroupFileInfo MiraiClient::GetGroupFileInfo(GID_t GroupId, const FilePath& dir, bool withDownloadInfo)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->FileInfo(SessionKey, dir.GetId(), dir.GetPath(), GroupId, withDownloadInfo);

	LOG_TRACE(this->_GetLogger(), "Calling FileInfo received " + resp.dump());

	return resp.get<GroupFileInfo>();
}

void MiraiClient::GetGroupFileInfo(GID_t GroupId, GroupFileInfo& file, bool withDownloadInfo)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->FileInfo(SessionKey, file.id, "", GroupId, withDownloadInfo);

	LOG_TRACE(this->_GetLogger(), "Calling FileInfo received " + resp.dump());

	file = resp.get<GroupFileInfo>();
}

GroupFileInfo MiraiClient::CreateGroupFileDirectory(GID_t GroupId, const string& directory)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->FileMkdir(SessionKey, "", "", GroupId, directory);

	LOG_TRACE(this->_GetLogger(), "Calling FileMkdir received " + resp.dump());

	return resp.get<GroupFileInfo>();
}

void MiraiClient::RemoveGroupFile(GID_t GroupId, const FilePath& dir)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->FileDelete(SessionKey, dir.GetId(), dir.GetPath(), GroupId);

	LOG_TRACE(this->_GetLogger(), "Calling RemoveGroupFile received " + resp.dump());
}

void MiraiClient::MoveGroupFile(GID_t GroupId, const FilePath& FileDir, const FilePath& MoveToDir)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->FileMove(SessionKey, FileDir.GetId(), FileDir.GetPath(), GroupId,
	                                        MoveToDir.GetId(), MoveToDir.GetPath());

	LOG_TRACE(this->_GetLogger(), "Calling FileMove received " + resp.dump());
}

void MiraiClient::RenameGroupFile(GID_t GroupId, const FilePath& FileDir, const string& NewName)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->FileRename(SessionKey, FileDir.GetId(), FileDir.GetPath(), GroupId, NewName);

	LOG_TRACE(this->_GetLogger(), "Calling FileRename received " + resp.dump());
}

GroupFileInfo MiraiClient::UploadGroupFile(GID_t GroupId, const string& UploadPath, const string& name,
                                           const string& content)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->FileUpload(SessionKey, UploadPath, GroupId, "group", name, content);

	LOG_TRACE(this->_GetLogger(), "Calling FileUpload received " + resp.dump());

	return resp.get<GroupFileInfo>();
}

GroupFileInfo MiraiClient::UploadGroupFile(GID_t GroupId, const string& UploadPath, const string& name,
                                           std::istream& file)
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
	string name = (path.has_filename()) ? path.filename().u8string() : path.u8string();
	std::ifstream f(path, std::ios_base::binary);
	return this->UploadGroupFile(GroupId, UploadPath, name, f);
}

FriendImage MiraiClient::UploadFriendImage(const string& content)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->UploadImage(SessionKey, "friend", content);

	LOG_TRACE(this->_GetLogger(), "Calling UploadImage received " + resp.dump());

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
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->UploadImage(SessionKey, "group", content);

	LOG_TRACE(this->_GetLogger(), "Calling UploadImage received " + resp.dump());

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
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->UploadImage(SessionKey, "temp", content);

	LOG_TRACE(this->_GetLogger(), "Calling UploadImage received " + resp.dump());

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
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->UploadAudio(SessionKey, "group", content);

	LOG_TRACE(this->_GetLogger(), "Calling UploadAudio received " + resp.dump());

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
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->DeleteFriend(SessionKey, qq);

	LOG_TRACE(this->_GetLogger(), "Calling DeleteFriend received " + resp.dump());
}

void MiraiClient::Mute(GID_t GroupId, QQ_t member, std::chrono::seconds time)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->Mute(SessionKey, GroupId, member, time);

	LOG_TRACE(this->_GetLogger(), "Calling Mute received " + resp.dump());
}

void MiraiClient::Mute(const GroupMember& member, std::chrono::seconds time)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->Mute(SessionKey, member.group.id, member.id, time);

	LOG_TRACE(this->_GetLogger(), "Calling Mute received " + resp.dump());
}

void MiraiClient::Unmute(GID_t GroupId, QQ_t member)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->Unmute(SessionKey, GroupId, member);

	LOG_TRACE(this->_GetLogger(), "Calling Unmute received " + resp.dump());
}

void MiraiClient::Unmute(const GroupMember& member)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->Unmute(SessionKey, member.group.id, member.id);

	LOG_TRACE(this->_GetLogger(), "Calling Unmute received " + resp.dump());
}

void MiraiClient::Kick(GID_t GroupId, QQ_t member, const string& message)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->Kick(SessionKey, GroupId, member, message);

	LOG_TRACE(this->_GetLogger(), "Calling Kick received " + resp.dump());
}

void MiraiClient::Kick(const GroupMember& member, const string& message)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->Kick(SessionKey, member.group.id, member.id, message);

	LOG_TRACE(this->_GetLogger(), "Calling Kick received " + resp.dump());
}

void MiraiClient::LeaveGroup(GID_t GroupId)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->Quit(SessionKey, GroupId);

	LOG_TRACE(this->_GetLogger(), "Calling Quit received " + resp.dump());
}

void MiraiClient::MuteAll(GID_t GroupId)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->MuteAll(SessionKey, GroupId);

	LOG_TRACE(this->_GetLogger(), "Calling MuteAll received " + resp.dump());
}

void MiraiClient::UnmuteAll(GID_t GroupId)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->UnmuteAll(SessionKey, GroupId);

	LOG_TRACE(this->_GetLogger(), "Calling UnmuteAll received " + resp.dump());
}

void MiraiClient::SetEssence(GID_t GroupId, MessageId_t MessageId)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->SetEssence(SessionKey, MessageId, GroupId);

	LOG_TRACE(this->_GetLogger(), "Calling SetEssence received " + resp.dump());
}


GroupConfig MiraiClient::GetGroupConfig(GID_t GroupId)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->GetGroupConfig(SessionKey, GroupId);

	LOG_TRACE(this->_GetLogger(), "Calling GetGroupConfig received " + resp.dump());

	return resp.get<GroupConfig>();
}

void MiraiClient::SetGroupConfig(GID_t GroupId, const string& name, std::optional<bool> AllowMemberInvite)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->PostGroupConfig(SessionKey, GroupId, name, std::nullopt, AllowMemberInvite);

	LOG_TRACE(this->_GetLogger(), "Calling PostGroupConfig received " + resp.dump());
}

GroupMember MiraiClient::GetMemberInfo(GID_t GroupId, QQ_t member)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->GetMemberInfo(SessionKey, GroupId, member);

	LOG_TRACE(this->_GetLogger(), "Calling GetMemberInfo received " + resp.dump());

	return resp.get<GroupMember>();
}

void MiraiClient::SetMemberInfo(GID_t GroupId, QQ_t member, const string& name, const string& title)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->PostMemberInfo(SessionKey, GroupId, member, name, title);

	LOG_TRACE(this->_GetLogger(), "Calling PostMemberInfo received " + resp.dump());
}

void MiraiClient::SetGroupAdmin(GID_t GroupId, QQ_t member, bool assign)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->MemberAdmin(SessionKey, GroupId, member, assign);

	LOG_TRACE(this->_GetLogger(), "Calling MemberAdmin received " + resp.dump());
}


std::vector<GroupAnnouncement> MiraiClient::GetAnnouncementList(GID_t GroupId, int64_t offset, int64_t size)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->AnnoList(SessionKey, GroupId, offset, size);

	LOG_TRACE(this->_GetLogger(), "Calling AnnoList received " + resp.dump());

	return resp.get<std::vector<GroupAnnouncement>>();
}

GroupAnnouncement MiraiClient::PublishAnnouncement(GID_t GroupId, const string& content, const MiraiImage& cover,
                                                   bool ToNewMember, bool pinned, bool ShowEditCard, bool ShowPopup,
                                                   bool RequireConfirm)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->AnnoPublish(SessionKey, GroupId, content, cover.url, cover.path, cover.base64,
	                                           ToNewMember, pinned, ShowEditCard, ShowPopup, RequireConfirm);

	LOG_TRACE(this->_GetLogger(), "Calling AnnoPublish received " + resp.dump());

	return resp.get<GroupAnnouncement>();
}

void MiraiClient::DeleteAnnouncement(GID_t GroupId, const string& fid)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->AnnoDelete(SessionKey, GroupId, fid);

	LOG_TRACE(this->_GetLogger(), "Calling AnnoDelete received " + resp.dump());
}

void MiraiClient::DeleteAnnouncement(const GroupAnnouncement& announcement)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->AnnoDelete(SessionKey, announcement.group.id, announcement.fid);

	LOG_TRACE(this->_GetLogger(), "Calling AnnoDelete received " + resp.dump());
}


void MiraiClient::RespNewFriendRequestEvent(int64_t EventId, QQ_t FromId, GID_t GroupId, NewFriendRequestOp operation,
                                            const string& message)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->RespNewFriendRequestEvent(SessionKey, EventId, FromId, GroupId,
	                                                         (int)operation, message);

	LOG_TRACE(this->_GetLogger(), "Calling RespNewFriendRequestEvent received " + resp.dump());
}

void MiraiClient::RespNewFriendRequestEvent(const NewFriendRequestEvent& event, NewFriendRequestOp operation,
                                            const string& message)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->RespNewFriendRequestEvent(SessionKey, event.GetEventId(), event.GetUserId(),
	                                                         event.GetGroupId(), (int)operation, message);


	LOG_TRACE(this->_GetLogger(), "Calling RespNewFriendRequestEvent received " + resp.dump());
}

void MiraiClient::RespMemberJoinRequestEvent(int64_t EventId, QQ_t FromId, GID_t GroupId, MemberJoinRequestOp operation,
                                             const string& message)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->RespMemberJoinRequestEvent(SessionKey, EventId, FromId, GroupId,
	                                                          (int)operation, message);

	LOG_TRACE(this->_GetLogger(), "Calling RespMemberJoinRequestEvent received " + resp.dump());
}

void MiraiClient::RespMemberJoinRequestEvent(const MemberJoinRequestEvent& event, MemberJoinRequestOp operation,
                                             const string& message)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->RespMemberJoinRequestEvent(SessionKey, event.GetEventId(), event.GetUserId(),
	                                                          event.GetGroupId(), (int)operation, message);

	LOG_TRACE(this->_GetLogger(), "Calling RespMemberJoinRequestEvent received " + resp.dump());
}

void MiraiClient::RespBotInvitedJoinGroupRequestEvent(int64_t EventId, QQ_t FromId, GID_t GroupId,
                                                      BotInvitedJoinGroupRequestOp operation, const string& message)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->RespBotInvitedJoinGroupRequestEvent(SessionKey, EventId, FromId, GroupId,
	                                                                   (int)operation, message);

	LOG_TRACE(this->_GetLogger(), "Calling RespBotInvitedJoinGroupRequestEvent received " + resp.dump());
}

void MiraiClient::RespBotInvitedJoinGroupRequestEvent(const BotInvitedJoinGroupRequestEvent& event,
                                                      BotInvitedJoinGroupRequestOp operation, const string& message)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->RespBotInvitedJoinGroupRequestEvent(
		SessionKey, event.GetEventId(), event.GetUserId(), event.GetGroupId(), (int)operation, message);

	LOG_TRACE(this->_GetLogger(), "Calling RespBotInvitedJoinGroupRequestEvent received " + resp.dump());
}


void MiraiClient::RegisterCommand(const string& name, const std::vector<string>& alias, const string& usage,
                                  const string& description)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->CmdRegister(SessionKey, name, alias, usage, description);

	LOG_TRACE(this->_GetLogger(), "Calling CmdRegister received " + resp.dump());
}
void MiraiClient::ExecuteCommand(const MessageChain& command)
{
	string SessionKey = this->_GetSessionKeyCopy();
	json resp = this->_HttpClient->CmdExecute(SessionKey, command.ToJson(false));

	LOG_TRACE(this->_GetLogger(), "Calling CmdExecute received " + resp.dump());
}


json MiraiClient::PostRaw(const string& path, const string& content, const string& ContentType)
{
	return this->_HttpClient->PostRaw(path, content, ContentType);
}

json MiraiClient::GetRaw(const string& path, const std::multimap<string, string> params)
{
	return this->_HttpClient->GetRaw(path, params);
}

} // namespace Mirai