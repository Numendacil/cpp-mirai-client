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
#include <thread>
#include <type_traits>
#include <utility>

#include <nlohmann/json.hpp>

#include <libmirai/Impl/HttpClientImpl.hpp>
#include <libmirai/Impl/MessageClientImpl.hpp>
#include <libmirai/Serialization/Events/Events.hpp>
#include <libmirai/Serialization/Messages/MessageChain.hpp>
#include <libmirai/Serialization/Types/Types.hpp>
#include <libmirai/Utils/Common.hpp>
#include <libmirai/Utils/Logger.hpp>
#include <libmirai/Utils/ThreadPool.hpp>
#include <libmirai/models.hpp>

namespace Mirai
{

using json = nlohmann::json;

MiraiClient::MiraiClient()
{
	this->logger_ = std::make_shared<NullLogger>();
}

MiraiClient::MiraiClient(SessionConfigs config) : config_(std::move(config))
{
	this->logger_ = std::make_shared<NullLogger>();
}

MiraiClient::MiraiClient(MiraiClient&& rhs) noexcept
{
	*this = std::move(rhs);
}

MiraiClient& MiraiClient::operator=(MiraiClient&& rhs) noexcept
{
	if (this != &rhs)
	{
		std::unique_lock<std::shared_mutex> lk(this->mtx_);
		this->config_ = std::move(rhs.config_);
		this->SessionKey_ = std::move(rhs.SessionKey_);
		this->SessionKeySet_ = rhs.SessionKeySet_.load();
		this->connected_ = rhs.connected_;
		this->HttpClients_ = std::move(rhs.HttpClients_);
		this->MessageClient_ = std::move(rhs.MessageClient_);
		this->ThreadPool_ = std::move(rhs.ThreadPool_);
	}
	return *this;
}

MiraiClient::~MiraiClient() = default;

Details::HttpClientImpl* MiraiClient::GetClient_()
{
	auto id = std::this_thread::get_id();

	std::unique_lock<std::shared_mutex> lk(this->mtx_);
	if (!this->HttpClients_[id])
		this->HttpClients_[id] =
			std::make_unique<Details::HttpClientImpl>(this->config_.HttpUrl, this->config_.ConnectionTimeout,
		                                              this->config_.ReadTimeout, this->config_.WriteTimeout);
	return this->HttpClients_[id].get();
}


void MiraiClient::Connect()
{
	std::unique_lock<std::shared_mutex> lk(this->mtx_); // Lock everything

	if (this->connected_) return;

	LOG_DEBUG(*(this->logger_), "Connecting to Mirai-Api-Http, initializing clients and threadpool");

	this->SessionKey_ = "";
	this->SessionKeySet_ = false;

	this->ThreadPool_ = std::make_unique<Utils::ThreadPool>(this->config_.ThreadPoolSize);

	LOG_DEBUG(*(this->logger_), "Threadpool initialized");

	this->HttpClients_.clear();


	std::condition_variable_any cond;

	this->MessageClient_ = std::make_unique<Details::MessageClientImpl>(
		this->config_.HeartbeatInterval, this->config_.HandshakeTimeout, this->config_.EnablePong,
		this->config_.EnableDeflate, this->config_.AutoReconnect, this->config_.MinRetryInterval,
		this->config_.MaxRetryInterval);

	this->MessageClient_->OnOpen(
		[this](const ix::WebSocketOpenInfo& info)
		{
			LOG_DEBUG(
				this->GetLogger_(),
				"MessageClient connected with response header: " + [&]() -> std::string
				{
					std::string header;
					for (const auto& p : info.headers)
						header += "\n" + p.first + ": " + p.second;
					return header;
				}());

			this->HandshakeInfo_ = ClientConnectionEstablishedEvent{{}, info.uri, {info.headers.begin(), info.headers.end()}, info.protocol, {}, {}};
		});

	this->MessageClient_->OnError(
		[this](const ix::WebSocketErrorInfo& info)
		{
			LOG_DEBUG(
				this->GetLogger_(),
				"MessageClient failed to connect with error: " + [&]() -> std::string
				{
					std::string msg;
					msg += "\nHTTP Status: " + std::to_string(info.http_status);
					msg += "\nReason: " + info.reason;
					msg += "\nDecompressionError: ";
					msg += info.decompressionError ? "True" : "False";
					return msg;
				}());

			auto callback = this->GetErrorCallback_();
			if (callback)
			{
				ClientConnectionErrorEvent event{{}, info.retries, info.wait_time, info.http_status, info.reason,
			                                     info.decompressionError};
				callback(std::move(event));
			}
		});

	this->MessageClient_->OnClose(
		[this](const ix::WebSocketCloseInfo& info)
		{
			LOG_DEBUG(
				this->GetLogger_(),
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
				std::unique_lock<std::shared_mutex> lk(this->mtx_);
				this->connected_ = false;
			}
			this->SessionKeySet_ = false;
			auto callback = this->GetClosedCallback_();
			if (callback)
			{
				ClientConnectionClosedEvent event{{}, info.code, info.reason, info.remote};
				callback(std::move(event));
			}
		});

	this->MessageClient_->OnText(
		[this, &cond](const std::string& message)
		{
			LOG_TRACE(this->GetLogger_(), "Received: " + message);

			try
			{
				json msg = json::parse(message);

				if (!msg.contains("data")) return;

				/*std::string id = msg.value("syncId", "");*/
				json data = msg.at("data");
				if (!data.is_object()) return;

				data = Utils::ParseResponse(data);

				if (!this->SessionKeySet_) // check for sessionKey
				{
					if (!(data.contains("session") && data.at("session").is_string())) return;

					std::string session = data.at("session").get<std::string>();

					LOG_DEBUG(this->GetLogger_(), "Session key obtained: " + session);

					// Check http connection
					ClientConnectionEstablishedEvent event;
					{
						LOG_DEBUG(this->GetLogger_(),
					              "Checking HTTP connection, calling /sessionInfo with session key: " + session);

						json resp = this->GetClient_()->SessionInfo(session);

						LOG_DEBUG(this->GetLogger_(), "Checking HTTP connection, received: " + resp.dump());

						std::string validate = resp.at("sessionKey").get<std::string>();
						if (session != validate)
							throw MiraiApiHttpException(-1,
						                                "Dismatched sessionKey: \"" + session + "\" <-> \"" + validate);

						{
							std::unique_lock<std::shared_mutex> lk(this->mtx_);
							this->SessionKey_ = session;
							this->SessionKeySet_ = true;
							this->connected_ = true;
							this->HandshakeInfo_.SessionKey = session;
							resp.at("qq").get_to(this->HandshakeInfo_.BotProfile);
							event = std::move(this->HandshakeInfo_);
						}
					}

					auto callback = this->GetEstablishedCallback_();
					if (callback) callback(std::move(event));

					cond.notify_one();
					return;
				}

				if (!(data.contains("type") && data.at("type").is_string())) return;

				auto type = data.at("type").get<EventTypes>();

				LOG_TRACE(this->GetLogger_(), "Dispatching message, type: " + type);

				EventHandler handler;
				{
					std::shared_lock<std::shared_mutex> lk(this->mtx_);
					if (this->EventHandlers_.count(type)) handler = this->EventHandlers_.at(type);
				}
				std::visit([&data, this](auto&& handler)
				{
					using FuncType = std::decay_t<decltype(handler)>;
					using EventType = typename traits::function_traits<FuncType>::type;

					if (!handler)
					{
						LOG_TRACE(this->GetLogger_(), "No matching handler found");
						return;
					}
					
					LOG_TRACE(this->GetLogger_(), "Found handler");

					if constexpr (
						!std::disjunction_v<
							std::is_same<EventType, ClientConnectionEstablishedEvent>,
							std::is_same<EventType, ClientConnectionErrorEvent>,
							std::is_same<EventType, ClientConnectionClosedEvent>,
							std::is_same<EventType, ClientParseErrorEvent>
						>)
					{
						EventType event;
						event.SetClient_(this);
						from_json(data, event);
						(void)this->ThreadPool_->enqueue(handler, std::move(event));
					}
					
				}, handler);
			}
			catch (const ParseError& e)
			{
				auto callback = this->GetParseErrorCallback_();
				if (callback) callback({{}, e, message});
			}
			catch (const std::exception& e)
			{
				auto callback = this->GetParseErrorCallback_();
				if (callback) callback({{}, ParseError{e.what(), message}, message});
			}
			catch (...)
			{
				auto callback = this->GetParseErrorCallback_();
				if (callback) callback({{}, ParseError{"Unknown error", message}, message});
			}
		});

	LOG_DEBUG(*(this->logger_), "Clients initialized");

	LOG_DEBUG(*(this->logger_),
	          "Calling connect from MessageClient: " + this->config_.WebsocketUrl
	              + "/all?verifyKey=" + this->config_.VerifyKey + "&qq=" + this->config_.BotQQ.to_string());

	this->MessageClient_->Connect(this->config_.WebsocketUrl + "/all?verifyKey=" + this->config_.VerifyKey
	                              + "&qq=" + this->config_.BotQQ.to_string());


	if (!cond.wait_for(lk, std::chrono::seconds(10), [this]() -> bool { return this->connected_; }))
	{
		LOG_DEBUG(*(this->logger_), "Timeout waiting for session key, closing down");

		// failed to get sessionKey
		this->MessageClient_->Disconnect();
		throw NetworkException(-2, "Failed to receive session key from server");
	}


	LOG_DEBUG(*(this->logger_), "Successfully connected");
}

void MiraiClient::Disconnect()
{
	std::unique_lock<std::shared_mutex> lk(this->mtx_); // Lock everything

	if (!this->connected_) return;

	if (this->MessageClient_)
	{
		this->MessageClient_->Disconnect();
		LOG_DEBUG(*(this->logger_), "Successfully disconnected");
	}
	this->ThreadPool_ = nullptr;

	LOG_DEBUG(*(this->logger_), "Threadpool shutdown complete");

	this->HttpClients_.clear();
	this->MessageClient_ = nullptr;

	LOG_DEBUG(*(this->logger_), "Clients shutdown complete");
}

template<>
void MiraiClient::On<ClientConnectionEstablishedEvent>(EventCallback<ClientConnectionEstablishedEvent> callback)
{
	std::unique_lock<std::shared_mutex> lk(this->mtx_);
	this->ConnectionEstablishedCallback_ = callback;
}

template<> void MiraiClient::On<ClientConnectionErrorEvent>(EventCallback<ClientConnectionErrorEvent> callback)
{
	std::unique_lock<std::shared_mutex> lk(this->mtx_);
	this->ConnectionErrorCallback_ = callback;
}

template<> void MiraiClient::On<ClientConnectionClosedEvent>(EventCallback<ClientConnectionClosedEvent> callback)
{
	std::unique_lock<std::shared_mutex> lk(this->mtx_);
	this->ConnectionClosedCallback_ = callback;
}

template<> void MiraiClient::On<ClientParseErrorEvent>(EventCallback<ClientParseErrorEvent> callback)
{
	std::unique_lock<std::shared_mutex> lk(this->mtx_);
	this->ParseErrorCallback_ = callback;
}


using std::string;

string MiraiClient::GetMiraiApiHttpVersion()
{
	json resp = this->GetClient_()->About();

	LOG_TRACE(this->GetLogger_(), "Calling About received " + resp.dump());

	return Utils::GetValue(resp, "version", "");
}

std::vector<QQ_t> MiraiClient::GetBotList()
{
	json resp = this->GetClient_()->BotList();

	LOG_TRACE(this->GetLogger_(), "Calling BotList received " + resp.dump());

	return resp.get<std::vector<QQ_t>>();
}


FriendMessageEvent MiraiClient::GetFriendMessage(MessageId_t id, QQ_t qq)
{
	json resp = this->GetClient_()->MessageFromId(this->GetSessionKeyCopy_(), id, qq);

	LOG_TRACE(this->GetLogger_(), "Calling MessageFromId received " + resp.dump());

	FriendMessageEvent event;
	if (FriendMessageEvent::GetType() != resp.at("type").get<EventTypes>())
		throw TypeDismatch(to_string(FriendMessageEvent::GetType()), Utils::GetValue(resp, "type", ""));
	resp.get_to(event);
	return event;
}

GroupMessageEvent MiraiClient::GetGroupMessage(MessageId_t id, GID_t GroupId)
{
	json resp = this->GetClient_()->MessageFromId(this->GetSessionKeyCopy_(), id, GroupId);

	LOG_TRACE(this->GetLogger_(), "Calling MessageFromId received " + resp.dump());

	GroupMessageEvent event;
	if (GroupMessageEvent::GetType() != resp.at("type").get<EventTypes>())
		throw TypeDismatch(to_string(GroupMessageEvent::GetType()), Utils::GetValue(resp, "type", ""));
	resp.get_to(event);
	return event;
}

TempMessageEvent MiraiClient::GetTempMessage(MessageId_t id, GID_t GroupId)
{
	json resp = this->GetClient_()->MessageFromId(this->GetSessionKeyCopy_(), id, GroupId);

	LOG_TRACE(this->GetLogger_(), "Calling MessageFromId received " + resp.dump());

	TempMessageEvent event;
	if (TempMessageEvent::GetType() != resp.at("type").get<EventTypes>())
		throw TypeDismatch(to_string(TempMessageEvent::GetType()), Utils::GetValue(resp, "type", ""));
	resp.get_to(event);
	return event;
}

StrangerMessageEvent MiraiClient::GetStrangerMessage(MessageId_t id, QQ_t qq)
{
	json resp = this->GetClient_()->MessageFromId(this->GetSessionKeyCopy_(), id, qq);

	LOG_TRACE(this->GetLogger_(), "Calling MessageFromId received " + resp.dump());

	StrangerMessageEvent event;
	if (StrangerMessageEvent::GetType() != resp.at("type").get<EventTypes>())
		throw TypeDismatch(to_string(StrangerMessageEvent::GetType()), Utils::GetValue(resp, "type", ""));
	resp.get_to(event);
	return event;
}


std::vector<User> MiraiClient::GetFriendList()
{
	json resp = this->GetClient_()->FriendList(this->GetSessionKeyCopy_());

	LOG_TRACE(this->GetLogger_(), "Calling FriendList received " + resp.dump());

	return resp.get<std::vector<User>>();
}

std::vector<Group> MiraiClient::GetGroupList()
{
	json resp = this->GetClient_()->GroupList(this->GetSessionKeyCopy_());

	LOG_TRACE(this->GetLogger_(), "Calling GroupList received " + resp.dump());

	return resp.get<std::vector<Group>>();
}

std::vector<GroupMember> MiraiClient::GetMemberList(GID_t GroupId)
{
	json resp = this->GetClient_()->MemberList(this->GetSessionKeyCopy_(), GroupId);

	LOG_TRACE(this->GetLogger_(), "Calling MemberList received " + resp.dump());

	return resp.get<std::vector<GroupMember>>();
}


UserProfile MiraiClient::GetBotProfile()
{
	json resp = this->GetClient_()->BotProfile(this->GetSessionKeyCopy_());

	LOG_TRACE(this->GetLogger_(), "Calling BotProfile received " + resp.dump());

	return resp.get<UserProfile>();
}

UserProfile MiraiClient::GetFriendProfile(QQ_t qq)
{
	json resp = this->GetClient_()->FriendProfile(this->GetSessionKeyCopy_(), qq);

	LOG_TRACE(this->GetLogger_(), "Calling FriendProfile received " + resp.dump());

	return resp.get<UserProfile>();
}

UserProfile MiraiClient::GetMemberProfile(GID_t GroupId, QQ_t MemberId)
{
	json resp = this->GetClient_()->MemberProfile(this->GetSessionKeyCopy_(), GroupId, MemberId);

	LOG_TRACE(this->GetLogger_(), "Calling MemberProfile received " + resp.dump());

	return resp.get<UserProfile>();
}

UserProfile MiraiClient::GetUserProfile(QQ_t qq)
{
	json resp = this->GetClient_()->UserProfile(this->GetSessionKeyCopy_(), qq);

	LOG_TRACE(this->GetLogger_(), "Calling UserProfile received " + resp.dump());

	return resp.get<UserProfile>();
}

MessageId_t MiraiClient::SendFriendMessage(QQ_t qq, const MessageChain& message, std::optional<MessageId_t> QuoteId)
{
	json resp = this->GetClient_()->SendFriendMessage(this->GetSessionKeyCopy_(), qq, message, QuoteId);

	LOG_TRACE(this->GetLogger_(), "Calling SendFriendMessage received " + resp.dump());

	return Utils::GetValue(resp, "messageId", (MessageId_t)0);
}

MessageId_t MiraiClient::SendGroupMessage(GID_t GroupId, const MessageChain& message,
                                          std::optional<MessageId_t> QuoteId)
{
	json resp = this->GetClient_()->SendGroupMessage(this->GetSessionKeyCopy_(), GroupId, message, QuoteId);

	LOG_TRACE(this->GetLogger_(), "Calling SendGroupMessage received " + resp.dump());

	return Utils::GetValue(resp, "messageId", (MessageId_t)0);
}

MessageId_t MiraiClient::SendTempMessage(QQ_t MemberId, GID_t GroupId, const MessageChain& message,
                                         std::optional<MessageId_t> QuoteId)
{
	json resp = this->GetClient_()->SendTempMessage(this->GetSessionKeyCopy_(), MemberId, GroupId, message, QuoteId);

	LOG_TRACE(this->GetLogger_(), "Calling SendTempMessage received " + resp.dump());

	return Utils::GetValue(resp, "messageId", (MessageId_t)0);
}

void MiraiClient::SendNudge(const NudgeTarget& target)
{
	json resp;
	if (target.GetNudgeType() == NudgeType::GROUP)
		resp = this->GetClient_()->SendNudge(this->GetSessionKeyCopy_(), target.GetTarget(), target.GetGroup(),
		                                     target.GetNudgeType());
	else
		resp = this->GetClient_()->SendNudge(this->GetSessionKeyCopy_(), target.GetTarget(), target.GetTarget(),
		                                     target.GetNudgeType());

	LOG_TRACE(this->GetLogger_(), "Calling SendNudge received " + resp.dump());
}

void MiraiClient::NudgeFriend(QQ_t qq)
{
	json resp = this->GetClient_()->SendNudge(this->GetSessionKeyCopy_(), qq, qq, NudgeType::FRIEND);

	LOG_TRACE(this->GetLogger_(), "Calling SendNudge received " + resp.dump());
}

void MiraiClient::NudgeGroup(QQ_t MemberId, GID_t GroupId)
{
	json resp = this->GetClient_()->SendNudge(this->GetSessionKeyCopy_(), MemberId, GroupId, NudgeType::GROUP);

	LOG_TRACE(this->GetLogger_(), "Calling SendNudge received " + resp.dump());
}

void MiraiClient::NudgeStranger(QQ_t qq)
{
	json resp = this->GetClient_()->SendNudge(this->GetSessionKeyCopy_(), qq, qq, NudgeType::STRANGER);

	LOG_TRACE(this->GetLogger_(), "Calling SendNudge received " + resp.dump());
}

void MiraiClient::RecallFriendMessage(MessageId_t id, QQ_t qq)
{
	json resp = this->GetClient_()->Recall(this->GetSessionKeyCopy_(), id, qq);

	LOG_TRACE(this->GetLogger_(), "Calling Recall received " + resp.dump());
}

void MiraiClient::RecallGroupMessage(MessageId_t id, GID_t GroupId)
{
	json resp = this->GetClient_()->Recall(this->GetSessionKeyCopy_(), id, GroupId);

	LOG_TRACE(this->GetLogger_(), "Calling Recall received " + resp.dump());
}

std::vector<MessageChain> MiraiClient::GetRoamingFriendMessage(QQ_t qq, std::time_t TimeStart, std::time_t TimeEnd)
{
	json resp = this->GetClient_()->RoamingMessages(this->GetSessionKeyCopy_(), TimeStart, TimeEnd, qq);

	LOG_TRACE(this->GetLogger_(), "Calling RoamingMessages received " + resp.dump());

	return resp.get<std::vector<MessageChain>>();
}


std::vector<GroupFileInfo> MiraiClient::GetGroupFileList(GID_t GroupId, const FilePath& dir, int64_t offset,
                                                         int64_t size, bool withDownloadInfo)
{
	json resp = this->GetClient_()->FileList(this->GetSessionKeyCopy_(), dir.GetId(), dir.GetPath(), GroupId, offset,
	                                         size, withDownloadInfo);

	LOG_TRACE(this->GetLogger_(), "Calling FileList received " + resp.dump());

	return resp.get<std::vector<GroupFileInfo>>();
}

GroupFileInfo MiraiClient::GetGroupFileInfo(GID_t GroupId, const FilePath& dir, bool withDownloadInfo)
{
	json resp =
		this->GetClient_()->FileInfo(this->GetSessionKeyCopy_(), dir.GetId(), dir.GetPath(), GroupId, withDownloadInfo);

	LOG_TRACE(this->GetLogger_(), "Calling FileInfo received " + resp.dump());

	return resp.get<GroupFileInfo>();
}

void MiraiClient::GetGroupFileInfo(GID_t GroupId, GroupFileInfo& file, bool withDownloadInfo)
{
	json resp = this->GetClient_()->FileInfo(this->GetSessionKeyCopy_(), file.id, "", GroupId, withDownloadInfo);

	LOG_TRACE(this->GetLogger_(), "Calling FileInfo received " + resp.dump());

	file = resp.get<GroupFileInfo>();
}

GroupFileInfo MiraiClient::CreateGroupFileDirectory(GID_t GroupId, const string& directory)
{
	json resp = this->GetClient_()->FileMkdir(this->GetSessionKeyCopy_(), "", "", GroupId, directory);

	LOG_TRACE(this->GetLogger_(), "Calling FileMkdir received " + resp.dump());

	return resp.get<GroupFileInfo>();
}

void MiraiClient::RemoveGroupFile(GID_t GroupId, const FilePath& dir)
{
	json resp = this->GetClient_()->FileDelete(this->GetSessionKeyCopy_(), dir.GetId(), dir.GetPath(), GroupId);

	LOG_TRACE(this->GetLogger_(), "Calling RemoveGroupFile received " + resp.dump());
}

void MiraiClient::MoveGroupFile(GID_t GroupId, const FilePath& FileDir, const FilePath& MoveToDir)
{
	json resp = this->GetClient_()->FileMove(this->GetSessionKeyCopy_(), FileDir.GetId(), FileDir.GetPath(), GroupId,
	                                         MoveToDir.GetId(), MoveToDir.GetPath());

	LOG_TRACE(this->GetLogger_(), "Calling FileMove received " + resp.dump());
}

void MiraiClient::RenameGroupFile(GID_t GroupId, const FilePath& FileDir, const string& NewName)
{
	json resp = this->GetClient_()->FileRename(this->GetSessionKeyCopy_(), FileDir.GetId(), FileDir.GetPath(), GroupId,
	                                           NewName);

	LOG_TRACE(this->GetLogger_(), "Calling FileRename received " + resp.dump());
}


GroupFileInfo MiraiClient::UploadGroupFile(GID_t GroupId, const string& UploadPath, const string& name, string content)
{
	json resp = this->GetClient_()->FileUpload(this->GetSessionKeyCopy_(), UploadPath, GroupId, "group", name,
	                                           std::move(content));

	LOG_TRACE(this->GetLogger_(), "Calling FileUpload received " + resp.dump());

	return resp.get<GroupFileInfo>();
}

GroupFileInfo MiraiClient::UploadGroupFile(GID_t GroupId, const string& UploadPath, const string& name,
                                           std::istream& file)
{
	string s;
	char buffer[4096];                        // NOLINT(*-avoid-c-arrays)
	while (file.read(buffer, sizeof(buffer))) // NOLINT(*-array-to-pointer-decay)
		s.append(buffer, sizeof(buffer));     // NOLINT(*-array-to-pointer-decay)
	s.append(buffer, file.gcount());          // NOLINT(*-array-to-pointer-decay)
	return this->UploadGroupFile(GroupId, UploadPath, name, std::move(s));
}

GroupFileInfo
MiraiClient::UploadGroupFile(GID_t GroupId, const string& UploadPath, const string& name,
                             std::function<bool(size_t offset, std::ostream& sink, bool& finish)> ContentProvider)
{
	json resp = this->GetClient_()->FileUploadChunked(this->GetSessionKeyCopy_(), UploadPath, GroupId, "group", name,
	                                                  std::move(ContentProvider));

	LOG_TRACE(this->GetLogger_(), "Calling FileUploadChunked received " + resp.dump());

	return resp.get<GroupFileInfo>();
}


FriendImage MiraiClient::UploadFriendImage(string content)
{
	json resp = this->GetClient_()->UploadImage(this->GetSessionKeyCopy_(), "friend", std::move(content));

	LOG_TRACE(this->GetLogger_(), "Calling UploadImage received " + resp.dump());

	return resp.get<FriendImage>();
}

FriendImage MiraiClient::UploadFriendImage(std::istream& file)
{
	string s;
	char buffer[4096];                        // NOLINT(*-avoid-c-arrays)
	while (file.read(buffer, sizeof(buffer))) // NOLINT(*-array-to-pointer-decay)
		s.append(buffer, sizeof(buffer));     // NOLINT(*-array-to-pointer-decay)
	s.append(buffer, file.gcount());          // NOLINT(*-array-to-pointer-decay)
	return this->UploadFriendImage(std::move(s));
}

FriendImage
MiraiClient::UploadFriendImage(std::function<bool(size_t offset, std::ostream& sink, bool& finish)> ContentProvider)
{
	json resp =
		this->GetClient_()->UploadImageChunked(this->GetSessionKeyCopy_(), "friend", std::move(ContentProvider));

	LOG_TRACE(this->GetLogger_(), "Calling UploadImageChunked received " + resp.dump());

	return resp.get<FriendImage>();
}


GroupImage MiraiClient::UploadGroupImage(string content)
{
	json resp = this->GetClient_()->UploadImage(this->GetSessionKeyCopy_(), "group", std::move(content));

	LOG_TRACE(this->GetLogger_(), "Calling UploadImage received " + resp.dump());

	return resp.get<GroupImage>();
}

GroupImage MiraiClient::UploadGroupImage(std::istream& file)
{
	string s;
	char buffer[4096];                        // NOLINT(*-avoid-c-arrays)
	while (file.read(buffer, sizeof(buffer))) // NOLINT(*-array-to-pointer-decay)
		s.append(buffer, sizeof(buffer));     // NOLINT(*-array-to-pointer-decay)
	s.append(buffer, file.gcount());          // NOLINT(*-array-to-pointer-decay)
	return this->UploadGroupImage(std::move(s));
}

GroupImage
MiraiClient::UploadGroupImage(std::function<bool(size_t offset, std::ostream& sink, bool& finish)> ContentProvider)
{
	json resp = this->GetClient_()->UploadImageChunked(this->GetSessionKeyCopy_(), "group", std::move(ContentProvider));

	LOG_TRACE(this->GetLogger_(), "Calling UploadImageChunked received " + resp.dump());

	return resp.get<GroupImage>();
}


TempImage MiraiClient::UploadTempImage(const string& content)
{
	json resp = this->GetClient_()->UploadImage(this->GetSessionKeyCopy_(), "temp", content);

	LOG_TRACE(this->GetLogger_(), "Calling UploadImage received " + resp.dump());

	return resp.get<TempImage>();
}

TempImage MiraiClient::UploadTempImage(std::istream& file)
{
	string s;
	char buffer[4096];                        // NOLINT(*-avoid-c-arrays)
	while (file.read(buffer, sizeof(buffer))) // NOLINT(*-array-to-pointer-decay)
		s.append(buffer, sizeof(buffer));     // NOLINT(*-array-to-pointer-decay)
	s.append(buffer, file.gcount());          // NOLINT(*-array-to-pointer-decay)
	return this->UploadTempImage(s);
}

TempImage
MiraiClient::UploadTempImage(std::function<bool(size_t offset, std::ostream& sink, bool& finish)> ContentProvider)
{
	json resp = this->GetClient_()->UploadImageChunked(this->GetSessionKeyCopy_(), "temp", std::move(ContentProvider));

	LOG_TRACE(this->GetLogger_(), "Calling UploadImageChunked received " + resp.dump());

	return resp.get<TempImage>();
}


GroupAudio MiraiClient::UploadGroupAudio(const string& content)
{
	json resp = this->GetClient_()->UploadAudio(this->GetSessionKeyCopy_(), "group", content);

	LOG_TRACE(this->GetLogger_(), "Calling UploadAudio received " + resp.dump());

	return resp.get<GroupAudio>();
}

GroupAudio MiraiClient::UploadGroupAudio(std::istream& file)
{
	string s;
	char buffer[4096];                        // NOLINT(*-avoid-c-arrays)
	while (file.read(buffer, sizeof(buffer))) // NOLINT(*-array-to-pointer-decay)
		s.append(buffer, sizeof(buffer));     // NOLINT(*-array-to-pointer-decay)
	s.append(buffer, file.gcount());          // NOLINT(*-array-to-pointer-decay)
	return this->UploadGroupAudio(s);
}

GroupAudio
MiraiClient::UploadGroupAudio(std::function<bool(size_t offset, std::ostream& sink, bool& finish)> ContentProvider)
{
	json resp = this->GetClient_()->UploadAudioChunked(this->GetSessionKeyCopy_(), "group", std::move(ContentProvider));

	LOG_TRACE(this->GetLogger_(), "Calling UploadAudioChunked received " + resp.dump());

	return resp.get<GroupAudio>();
}


void MiraiClient::DeleteFriend(QQ_t qq)
{
	json resp = this->GetClient_()->DeleteFriend(this->GetSessionKeyCopy_(), qq);

	LOG_TRACE(this->GetLogger_(), "Calling DeleteFriend received " + resp.dump());
}

void MiraiClient::Mute(GID_t GroupId, QQ_t member, std::chrono::seconds time)
{
	json resp = this->GetClient_()->Mute(this->GetSessionKeyCopy_(), GroupId, member, time);

	LOG_TRACE(this->GetLogger_(), "Calling Mute received " + resp.dump());
}

void MiraiClient::Mute(const GroupMember& member, std::chrono::seconds time)
{
	json resp = this->GetClient_()->Mute(this->GetSessionKeyCopy_(), member.group.id, member.id, time);

	LOG_TRACE(this->GetLogger_(), "Calling Mute received " + resp.dump());
}

void MiraiClient::Unmute(GID_t GroupId, QQ_t member)
{
	json resp = this->GetClient_()->Unmute(this->GetSessionKeyCopy_(), GroupId, member);

	LOG_TRACE(this->GetLogger_(), "Calling Unmute received " + resp.dump());
}

void MiraiClient::Unmute(const GroupMember& member)
{
	json resp = this->GetClient_()->Unmute(this->GetSessionKeyCopy_(), member.group.id, member.id);

	LOG_TRACE(this->GetLogger_(), "Calling Unmute received " + resp.dump());
}

void MiraiClient::Kick(GID_t GroupId, QQ_t member, const string& message)
{
	json resp = this->GetClient_()->Kick(this->GetSessionKeyCopy_(), GroupId, member, message);

	LOG_TRACE(this->GetLogger_(), "Calling Kick received " + resp.dump());
}

void MiraiClient::Kick(const GroupMember& member, const string& message)
{
	json resp = this->GetClient_()->Kick(this->GetSessionKeyCopy_(), member.group.id, member.id, message);

	LOG_TRACE(this->GetLogger_(), "Calling Kick received " + resp.dump());
}

void MiraiClient::LeaveGroup(GID_t GroupId)
{
	json resp = this->GetClient_()->Quit(this->GetSessionKeyCopy_(), GroupId);

	LOG_TRACE(this->GetLogger_(), "Calling Quit received " + resp.dump());
}

void MiraiClient::MuteAll(GID_t GroupId)
{
	json resp = this->GetClient_()->MuteAll(this->GetSessionKeyCopy_(), GroupId);

	LOG_TRACE(this->GetLogger_(), "Calling MuteAll received " + resp.dump());
}

void MiraiClient::UnmuteAll(GID_t GroupId)
{
	json resp = this->GetClient_()->UnmuteAll(this->GetSessionKeyCopy_(), GroupId);

	LOG_TRACE(this->GetLogger_(), "Calling UnmuteAll received " + resp.dump());
}

void MiraiClient::SetEssence(GID_t GroupId, MessageId_t MessageId)
{
	json resp = this->GetClient_()->SetEssence(this->GetSessionKeyCopy_(), MessageId, GroupId);

	LOG_TRACE(this->GetLogger_(), "Calling SetEssence received " + resp.dump());
}


GroupConfig MiraiClient::GetGroupConfig(GID_t GroupId)
{
	json resp = this->GetClient_()->GetGroupConfig(this->GetSessionKeyCopy_(), GroupId);

	LOG_TRACE(this->GetLogger_(), "Calling GetGroupConfig received " + resp.dump());

	return resp.get<GroupConfig>();
}

void MiraiClient::SetGroupConfig(GID_t GroupId, const string& name, std::optional<bool> AllowMemberInvite)
{
	json resp =
		this->GetClient_()->PostGroupConfig(this->GetSessionKeyCopy_(), GroupId, name, std::nullopt, AllowMemberInvite);

	LOG_TRACE(this->GetLogger_(), "Calling PostGroupConfig received " + resp.dump());
}

GroupMember MiraiClient::GetMemberInfo(GID_t GroupId, QQ_t member)
{
	json resp = this->GetClient_()->GetMemberInfo(this->GetSessionKeyCopy_(), GroupId, member);

	LOG_TRACE(this->GetLogger_(), "Calling GetMemberInfo received " + resp.dump());

	return resp.get<GroupMember>();
}

void MiraiClient::SetMemberInfo(GID_t GroupId, QQ_t member, const string& name, const string& title)
{
	json resp = this->GetClient_()->PostMemberInfo(this->GetSessionKeyCopy_(), GroupId, member, name, title);

	LOG_TRACE(this->GetLogger_(), "Calling PostMemberInfo received " + resp.dump());
}

void MiraiClient::SetGroupAdmin(GID_t GroupId, QQ_t member, bool assign)
{
	json resp = this->GetClient_()->MemberAdmin(this->GetSessionKeyCopy_(), GroupId, member, assign);

	LOG_TRACE(this->GetLogger_(), "Calling MemberAdmin received " + resp.dump());
}


std::vector<GroupAnnouncement> MiraiClient::GetAnnouncementList(GID_t GroupId, int64_t offset, int64_t size)
{
	json resp = this->GetClient_()->AnnoList(this->GetSessionKeyCopy_(), GroupId, offset, size);

	LOG_TRACE(this->GetLogger_(), "Calling AnnoList received " + resp.dump());

	return resp.get<std::vector<GroupAnnouncement>>();
}

GroupAnnouncement MiraiClient::PublishAnnouncement(GID_t GroupId, const string& content, const MiraiImage& cover,
                                                   bool ToNewMember, bool pinned, bool ShowEditCard, bool ShowPopup,
                                                   bool RequireConfirm)
{
	json resp =
		this->GetClient_()->AnnoPublish(this->GetSessionKeyCopy_(), GroupId, content, cover.url, cover.path,
	                                    cover.base64, ToNewMember, pinned, ShowEditCard, ShowPopup, RequireConfirm);

	LOG_TRACE(this->GetLogger_(), "Calling AnnoPublish received " + resp.dump());

	return resp.get<GroupAnnouncement>();
}

void MiraiClient::DeleteAnnouncement(GID_t GroupId, const string& fid)
{
	json resp = this->GetClient_()->AnnoDelete(this->GetSessionKeyCopy_(), GroupId, fid);

	LOG_TRACE(this->GetLogger_(), "Calling AnnoDelete received " + resp.dump());
}

void MiraiClient::DeleteAnnouncement(const GroupAnnouncement& announcement)
{
	json resp = this->GetClient_()->AnnoDelete(this->GetSessionKeyCopy_(), announcement.group.id, announcement.fid);

	LOG_TRACE(this->GetLogger_(), "Calling AnnoDelete received " + resp.dump());
}


void MiraiClient::RespNewFriendRequestEvent(int64_t EventId, QQ_t FromId, GID_t GroupId, NewFriendRequestOp operation,
                                            const string& message)
{
	json resp = this->GetClient_()->RespNewFriendRequestEvent(this->GetSessionKeyCopy_(), EventId, FromId, GroupId,
	                                                          (int)operation, message);

	LOG_TRACE(this->GetLogger_(), "Calling RespNewFriendRequestEvent received " + resp.dump());
}

void MiraiClient::RespNewFriendRequestEvent(const NewFriendRequestEvent& event, NewFriendRequestOp operation,
                                            const string& message)
{
	json resp = this->GetClient_()->RespNewFriendRequestEvent(
		this->GetSessionKeyCopy_(), event.GetEventId(), event.GetUserId(), event.GetGroupId(), (int)operation, message);


	LOG_TRACE(this->GetLogger_(), "Calling RespNewFriendRequestEvent received " + resp.dump());
}

void MiraiClient::RespMemberJoinRequestEvent(int64_t EventId, QQ_t FromId, GID_t GroupId, MemberJoinRequestOp operation,
                                             const string& message)
{
	json resp = this->GetClient_()->RespMemberJoinRequestEvent(this->GetSessionKeyCopy_(), EventId, FromId, GroupId,
	                                                           (int)operation, message);

	LOG_TRACE(this->GetLogger_(), "Calling RespMemberJoinRequestEvent received " + resp.dump());
}

void MiraiClient::RespMemberJoinRequestEvent(const MemberJoinRequestEvent& event, MemberJoinRequestOp operation,
                                             const string& message)
{
	json resp = this->GetClient_()->RespMemberJoinRequestEvent(
		this->GetSessionKeyCopy_(), event.GetEventId(), event.GetUserId(), event.GetGroupId(), (int)operation, message);

	LOG_TRACE(this->GetLogger_(), "Calling RespMemberJoinRequestEvent received " + resp.dump());
}

void MiraiClient::RespBotInvitedJoinGroupRequestEvent(int64_t EventId, QQ_t FromId, GID_t GroupId,
                                                      BotInvitedJoinGroupRequestOp operation, const string& message)
{
	json resp = this->GetClient_()->RespBotInvitedJoinGroupRequestEvent(this->GetSessionKeyCopy_(), EventId, FromId,
	                                                                    GroupId, (int)operation, message);

	LOG_TRACE(this->GetLogger_(), "Calling RespBotInvitedJoinGroupRequestEvent received " + resp.dump());
}

void MiraiClient::RespBotInvitedJoinGroupRequestEvent(const BotInvitedJoinGroupRequestEvent& event,
                                                      BotInvitedJoinGroupRequestOp operation, const string& message)
{
	json resp = this->GetClient_()->RespBotInvitedJoinGroupRequestEvent(
		this->GetSessionKeyCopy_(), event.GetEventId(), event.GetUserId(), event.GetGroupId(), (int)operation, message);

	LOG_TRACE(this->GetLogger_(), "Calling RespBotInvitedJoinGroupRequestEvent received " + resp.dump());
}


void MiraiClient::RegisterCommand(const string& name, const std::vector<string>& alias, const string& usage,
                                  const string& description)
{
	json resp = this->GetClient_()->CmdRegister(this->GetSessionKeyCopy_(), name, alias, usage, description);

	LOG_TRACE(this->GetLogger_(), "Calling CmdRegister received " + resp.dump());
}
void MiraiClient::ExecuteCommand(const MessageChain& command)
{
	json resp = this->GetClient_()->CmdExecute(this->GetSessionKeyCopy_(), command);

	LOG_TRACE(this->GetLogger_(), "Calling CmdExecute received " + resp.dump());
}


string MiraiClient::PostRaw(const string& path, const string& content, const string& ContentType)
{
	return this->GetClient_()->PostRaw(path, content, ContentType);
}

string MiraiClient::GetRaw(const string& path, const std::multimap<string, string> params)
{
	return this->GetClient_()->GetRaw(path, params);
}

} // namespace Mirai