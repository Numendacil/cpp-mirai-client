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
#include <variant>

#include <nlohmann/json.hpp>
#include <httplib.h>

#include <libmirai/Serialization/Events/Events.hpp>
#include <libmirai/Serialization/Types/Types.hpp>
#include <libmirai/Utils/Common.hpp>
#include <libmirai/Utils/Logger.hpp>
#include <libmirai/Utils/ThreadPool.hpp>
#include <libmirai/Events/MiraiClientEvents.hpp>

namespace Mirai
{

using json = nlohmann::json;

MiraiClient::MiraiClient() = default;
MiraiClient::MiraiClient(std::unique_ptr<IAdaptor> adaptor) : adaptor_(std::move(adaptor)) {}
MiraiClient::MiraiClient(size_t PoolSize, std::unique_ptr<IAdaptor> adaptor) : PoolSize_(PoolSize), adaptor_(std::move(adaptor)) {}

MiraiClient::MiraiClient(MiraiClient&& rhs) noexcept
{
	std::unique_lock<std::shared_mutex> lk(rhs.mtx_);

	#define MIRAI_MOVE_ASSIGN(m_var) this->m_var = std::move(rhs.m_var)
	#define MIRAI_COPY_ASSIGN(m_var) this->m_var = rhs.m_var

	// NOLINTBEGIN(*-prefer-member-initializer)
	MIRAI_MOVE_ASSIGN(SessionKey_);
	MIRAI_MOVE_ASSIGN(logger_);
	MIRAI_MOVE_ASSIGN(adaptor_);
	MIRAI_MOVE_ASSIGN(pool_);

	MIRAI_MOVE_ASSIGN(ConnectionEstablishedCallback_);
	MIRAI_MOVE_ASSIGN(ConnectionClosedCallback_);
	MIRAI_MOVE_ASSIGN(ConnectionErrorCallback_);
	MIRAI_MOVE_ASSIGN(ParseErrorCallback_);

	MIRAI_MOVE_ASSIGN(EventHandlers_);

	MIRAI_COPY_ASSIGN(connected_);
	MIRAI_COPY_ASSIGN(PoolSize_);
	// NOLINTEND(*-prefer-member-initializer)

	#undef MIRAI_MOVE_ASSIGN
	#undef MIRAI_COPY_ASSIGN
}

MiraiClient& MiraiClient::operator=(MiraiClient&& rhs) noexcept
{
	if (this != &rhs)
	{
		std::unique_lock<std::shared_mutex> lk_this(this->mtx_, std::defer_lock);
		std::unique_lock<std::shared_mutex> lk_rhs(rhs.mtx_, std::defer_lock);

		std::scoped_lock lk(lk_rhs, lk_this);

		#define MIRAI_MOVE_ASSIGN(m_var) this->m_var = std::move(rhs.m_var)
		#define MIRAI_COPY_ASSIGN(m_var) this->m_var = rhs.m_var

		MIRAI_MOVE_ASSIGN(SessionKey_);
		MIRAI_MOVE_ASSIGN(logger_);
		MIRAI_MOVE_ASSIGN(adaptor_);
		MIRAI_MOVE_ASSIGN(pool_);

		MIRAI_MOVE_ASSIGN(ConnectionEstablishedCallback_);
		MIRAI_MOVE_ASSIGN(ConnectionClosedCallback_);
		MIRAI_MOVE_ASSIGN(ConnectionErrorCallback_);
		MIRAI_MOVE_ASSIGN(ParseErrorCallback_);

		MIRAI_MOVE_ASSIGN(EventHandlers_);

		MIRAI_COPY_ASSIGN(connected_);
		MIRAI_COPY_ASSIGN(PoolSize_);

		#undef MIRAI_MOVE_ASSIGN
		#undef MIRAI_COPY_ASSIGN
	}
	return *this;
}

MiraiClient::~MiraiClient() { this->Disconnect(); }


void MiraiClient::Connect()
{
	if (!this->adaptor_)
		throw std::runtime_error("Null pointer: No adaptor is set");

	if (this->connected_) 
		return;

	LOG_DEBUG(*(this->logger_), "Connecting to Mirai-Api-Http, initializing threadpool");

	this->pool_ = std::make_unique<Utils::ThreadPool>(this->PoolSize_);

	LOG_DEBUG(*(this->logger_), "Threadpool initialized");

	this->adaptor_->OnConnectionEstablished(
		[this](ClientConnectionEstablishedEvent event)
		{
			LOG_DEBUG(
				*(this->logger_),
				"MessageClient connected with response header: " + [&]() -> std::string
				{
					std::string header;
					for (const auto& p : event.headers)
						header += "\n" + p.first + ": " + p.second;
					return header;
				}()
			);
			{
				std::unique_lock<std::shared_mutex> lk(this->mtx_);
				this->connected_ = true;
				this->SessionKey_ = event.SessionKey;
			}
			if (this->ConnectionEstablishedCallback_)
				this->ConnectionEstablishedCallback_(std::move(event));
		}
	);

	this->adaptor_->OnConnectionClosed(
		[this](ClientConnectionClosedEvent event)
		{
			LOG_DEBUG(
				*(this->logger_),
				"MessageClient connection closed: " + [&]() -> std::string
				{
					std::string msg;
					msg += "\nCode: " + std::to_string(event.code);
					msg += "\nReason: " + event.reason;
					msg += "\nClosedByRemote: ";
					msg += event.remote ? "True" : "False";
					return msg;
				}()
			);
			{
				std::unique_lock<std::shared_mutex> lk(this->mtx_);
				this->connected_ = false;
			}
			if (this->ConnectionClosedCallback_)
				this->ConnectionClosedCallback_(std::move(event));
		}
	);

	this->adaptor_->OnConnectionError(
		[this](ClientConnectionErrorEvent event)
		{
			LOG_DEBUG(
				*(this->logger_),
				"MessageClient failed to connect with error: " + [&event]() -> std::string
				{
					std::string msg;
					msg += "\nHTTP Status: " + std::to_string(event.HttpStatus);
					msg += "\nReason: " + event.reason;
					msg += "\nDecompressionError: ";
					msg += event.DecompressionError ? "True" : "False";
					return msg;
				}()
			);
			if (this->ConnectionErrorCallback_)
				this->ConnectionErrorCallback_(std::move(event));
		}
	);

	this->adaptor_->OnRecv(
		[this](const std::string& message)
		{
			LOG_TRACE(*(this->logger_), "Received: " + message);

			try
			{
				json msg = json::parse(message);
				if (!msg.contains("data")) 
					return;

				/*std::string id = msg.value("syncId", "");*/
				json data = msg.at("data");
				if (!data.is_object()) return;
				data = Utils::ParseResponse(data);
				if (!(data.contains("type") && data.at("type").is_string())) 
					return;

				auto type = data.at("type").get<EventTypes>();
				LOG_TRACE(*(this->logger_), "Dispatching message, type: " + to_string(type));

				EventHandler handler;
				{
					if (this->EventHandlers_.count(type)) handler = this->EventHandlers_.at(type);
				}
				std::visit([&data, this](auto&& handler)
				{
					using FuncType = std::decay_t<decltype(handler)>;
					using EventType = typename traits::function_traits<FuncType>::type;

					if (!handler)
					{
						LOG_TRACE(*(this->logger_), "No matching handler found");
						return;
					}
					
					LOG_TRACE(*(this->logger_), "Found handler");

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
						(void)this->pool_->enqueue(handler, std::move(event));
					}
					
				}, handler);
			}
			catch (const ParseError& e)
			{
				if (this->ParseErrorCallback_) 
					this->ParseErrorCallback_({{}, e, message});
			}
			catch (const std::exception& e)
			{
				if (this->ParseErrorCallback_) 
					this->ParseErrorCallback_({{}, ParseError{e.what(), message}, message});
			}
			catch (...)
			{
				if (this->ParseErrorCallback_) 
					this->ParseErrorCallback_({{}, ParseError{"Unknown error", message}, message});
			}
		});

	LOG_DEBUG(*(this->logger_), "Calling connect");

	(void)this->adaptor_->Connect();

	LOG_DEBUG(*(this->logger_), "Successfully connected");
}

void MiraiClient::Disconnect()
{
	this->adaptor_->Disconnect(this->GetSessionKey_());
	LOG_DEBUG(*(this->logger_), "Successfully disconnected");

	this->pool_ = nullptr;
	LOG_DEBUG(*(this->logger_), "Threadpool shutdown complete");
}


using std::string;

string MiraiClient::GetMiraiApiHttpVersion() const
{
	return this->adaptor_->Version();
}

QQ_t MiraiClient::GetBotQQ() const
{
	return this->adaptor_->GetBotQQ();
}

std::vector<QQ_t> MiraiClient::GetBotList() const
{
	return this->adaptor_->BotList();
}


FriendMessageEvent MiraiClient::GetFriendMessage(MessageId_t id, QQ_t qq) const
{
	auto msg = this->adaptor_->MessageFromId(this->GetSessionKey_(), id, qq);

	if (!std::holds_alternative<FriendMessageEvent>(msg))
		throw TypeDismatchError(to_string(FriendMessageEvent::GetType()), to_string(std::visit([](auto&& m) -> EventTypes { return m.GetType(); }, msg)));

	return std::get<FriendMessageEvent>(msg);
}

GroupMessageEvent MiraiClient::GetGroupMessage(MessageId_t id, GID_t GroupId) const
{
	auto msg = this->adaptor_->MessageFromId(this->GetSessionKey_(), id, GroupId);

	if (!std::holds_alternative<GroupMessageEvent>(msg))
		throw TypeDismatchError(to_string(GroupMessageEvent::GetType()), to_string(std::visit([](auto&& m) -> EventTypes { return m.GetType(); }, msg)));

	return std::get<GroupMessageEvent>(msg);
}

TempMessageEvent MiraiClient::GetTempMessage(MessageId_t id, GID_t GroupId) const
{
	auto msg = this->adaptor_->MessageFromId(this->GetSessionKey_(), id, GroupId);

	if (!std::holds_alternative<TempMessageEvent>(msg))
		throw TypeDismatchError(to_string(TempMessageEvent::GetType()), to_string(std::visit([](auto&& m) -> EventTypes { return m.GetType(); }, msg)));

	return std::get<TempMessageEvent>(msg);
}

StrangerMessageEvent MiraiClient::GetStrangerMessage(MessageId_t id, QQ_t qq) const
{
	auto msg = this->adaptor_->MessageFromId(this->GetSessionKey_(), id, qq);

	if (!std::holds_alternative<StrangerMessageEvent>(msg))
		throw TypeDismatchError(to_string(StrangerMessageEvent::GetType()), to_string(std::visit([](auto&& m) -> EventTypes { return m.GetType(); }, msg)));

	return std::get<StrangerMessageEvent>(msg);
}


std::vector<User> MiraiClient::GetFriendList() const
{
	return this->adaptor_->FriendList(this->GetSessionKey_());
}

std::vector<Group> MiraiClient::GetGroupList() const
{
	return this->adaptor_->GroupList(this->GetSessionKey_());
}

std::vector<GroupMember> MiraiClient::GetMemberList(GID_t GroupId) const
{
	return this->adaptor_->MemberList(this->GetSessionKey_(), GroupId);
}


UserProfile MiraiClient::GetBotProfile() const
{
	return this->adaptor_->GetBotProfile(this->GetSessionKey_());
}

UserProfile MiraiClient::GetFriendProfile(QQ_t qq) const
{
	return this->adaptor_->GetFriendProfile(this->GetSessionKey_(), qq);
}

UserProfile MiraiClient::GetMemberProfile(GID_t GroupId, QQ_t MemberId) const
{
	return this->adaptor_->GetMemberProfile(this->GetSessionKey_(), GroupId, MemberId);
}

UserProfile MiraiClient::GetUserProfile(QQ_t qq) const
{
	return this->adaptor_->GetUserProfile(this->GetSessionKey_(), qq);
}

MessageId_t MiraiClient::SendFriendMessage(QQ_t qq, const MessageChain& message, std::optional<MessageId_t> QuoteId) const
{
	return this->adaptor_->SendFriendMessage(this->GetSessionKey_(), qq, message, QuoteId);
}

MessageId_t MiraiClient::SendGroupMessage(GID_t GroupId, const MessageChain& message,
                                          std::optional<MessageId_t> QuoteId) const
{
	return this->adaptor_->SendGroupMessage(this->GetSessionKey_(), GroupId, message, QuoteId);
}

MessageId_t MiraiClient::SendTempMessage(QQ_t MemberId, GID_t GroupId, const MessageChain& message,
                                         std::optional<MessageId_t> QuoteId) const
{
	return this->adaptor_->SendTempMessage(this->GetSessionKey_(), MemberId, GroupId, message, QuoteId);
}

void MiraiClient::SendNudge(const NudgeTarget& target) const
{
	if (target.GetNudgeType() == NudgeType::GROUP)
		this->adaptor_->SendNudge(this->GetSessionKey_(), target.GetTarget(), target.GetGroup(),
		                                     target.GetNudgeType());
	else
		this->adaptor_->SendNudge(this->GetSessionKey_(), target.GetTarget(), target.GetTarget(),
		                                     target.GetNudgeType());
}

void MiraiClient::NudgeFriend(QQ_t qq) const
{
	this->adaptor_->SendNudge(this->GetSessionKey_(), qq, qq, NudgeType::FRIEND);
}

void MiraiClient::NudgeGroup(QQ_t MemberId, GID_t GroupId) const
{
	this->adaptor_->SendNudge(this->GetSessionKey_(), MemberId, GroupId, NudgeType::GROUP);
}

void MiraiClient::NudgeStranger(QQ_t qq) const
{
	this->adaptor_->SendNudge(this->GetSessionKey_(), qq, qq, NudgeType::STRANGER);
}

void MiraiClient::RecallFriendMessage(MessageId_t id, QQ_t qq) const
{
	this->adaptor_->Recall(this->GetSessionKey_(), id, qq);
}

void MiraiClient::RecallGroupMessage(MessageId_t id, GID_t GroupId) const
{
	this->adaptor_->Recall(this->GetSessionKey_(), id, GroupId);
}

std::vector<MessageChain> MiraiClient::GetRoamingFriendMessage(QQ_t qq, std::time_t TimeStart, std::time_t TimeEnd) const
{
	return this->adaptor_->RoamingMessages(this->GetSessionKey_(), TimeStart, TimeEnd, qq);
}


std::vector<GroupFileInfo> MiraiClient::GetGroupFileList(GID_t GroupId, const FilePath& dir, int64_t offset,
                                                         int64_t size, bool withDownloadInfo) const
{
	return this->adaptor_->FileList(this->GetSessionKey_(), dir.GetId(), dir.GetPath(), GroupId, offset,
	                                         size, withDownloadInfo);
}

GroupFileInfo MiraiClient::GetGroupFileInfo(GID_t GroupId, const FilePath& dir, bool withDownloadInfo) const
{
	return this->adaptor_->FileInfo(this->GetSessionKey_(), dir.GetId(), dir.GetPath(), GroupId, withDownloadInfo);
}

void MiraiClient::GetGroupFileInfo(GID_t GroupId, GroupFileInfo& file, bool withDownloadInfo) const
{
	this->adaptor_->FileInfo(this->GetSessionKey_(), file.id, "", GroupId, withDownloadInfo);
}

GroupFileInfo MiraiClient::CreateGroupFileDirectory(GID_t GroupId, const string& directory) const
{
	return this->adaptor_->FileMkdir(this->GetSessionKey_(), "", "", GroupId, directory);
}

void MiraiClient::RemoveGroupFile(GID_t GroupId, const FilePath& dir) const
{
	this->adaptor_->FileDelete(this->GetSessionKey_(), dir.GetId(), dir.GetPath(), GroupId);
}

void MiraiClient::MoveGroupFile(GID_t GroupId, const FilePath& FileDir, const FilePath& MoveToDir) const
{
	this->adaptor_->FileMove(this->GetSessionKey_(), FileDir.GetId(), FileDir.GetPath(), GroupId,
	                                         MoveToDir.GetId(), MoveToDir.GetPath());
}

void MiraiClient::RenameGroupFile(GID_t GroupId, const FilePath& FileDir, const string& NewName) const
{
	this->adaptor_->FileRename(this->GetSessionKey_(), FileDir.GetId(), FileDir.GetPath(), GroupId,
	                                           NewName);
}


GroupFileInfo MiraiClient::UploadGroupFile(GID_t GroupId, const string& UploadPath, const string& name, string content) const
{
	return this->adaptor_->FileUpload(this->GetSessionKey_(), UploadPath, GroupId, "group", name,
	                                           std::move(content));
}

GroupFileInfo MiraiClient::UploadGroupFile(GID_t GroupId, const string& UploadPath, const string& name,
                                           std::istream& file) const
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
                             std::function<bool(size_t offset, std::ostream& sink, bool& finish)> ContentProvider) const
{
	return this->adaptor_->FileUploadChunked(this->GetSessionKey_(), UploadPath, GroupId, "group", name,
	                                                  std::move(ContentProvider));
}


FriendImage MiraiClient::UploadFriendImage(string content) const
{
	return this->adaptor_->UploadImage(this->GetSessionKey_(), "friend", std::move(content));
}

FriendImage MiraiClient::UploadFriendImage(std::istream& file) const
{
	string s;
	char buffer[4096];                        // NOLINT(*-avoid-c-arrays)
	while (file.read(buffer, sizeof(buffer))) // NOLINT(*-array-to-pointer-decay)
		s.append(buffer, sizeof(buffer));     // NOLINT(*-array-to-pointer-decay)
	s.append(buffer, file.gcount());          // NOLINT(*-array-to-pointer-decay)
	return this->UploadFriendImage(std::move(s));
}

FriendImage
MiraiClient::UploadFriendImage(std::function<bool(size_t offset, std::ostream& sink, bool& finish)> ContentProvider) const
{
	return this->adaptor_->UploadImageChunked(this->GetSessionKey_(), "friend", std::move(ContentProvider));
}


GroupImage MiraiClient::UploadGroupImage(string content) const
{
	return this->adaptor_->UploadImage(this->GetSessionKey_(), "group", std::move(content));
}

GroupImage MiraiClient::UploadGroupImage(std::istream& file) const
{
	string s;
	char buffer[4096];                        // NOLINT(*-avoid-c-arrays)
	while (file.read(buffer, sizeof(buffer))) // NOLINT(*-array-to-pointer-decay)
		s.append(buffer, sizeof(buffer));     // NOLINT(*-array-to-pointer-decay)
	s.append(buffer, file.gcount());          // NOLINT(*-array-to-pointer-decay)
	return this->UploadGroupImage(std::move(s));
}

GroupImage
MiraiClient::UploadGroupImage(std::function<bool(size_t offset, std::ostream& sink, bool& finish)> ContentProvider) const
{
	return this->adaptor_->UploadImageChunked(this->GetSessionKey_(), "group", std::move(ContentProvider));
}


TempImage MiraiClient::UploadTempImage(const string& content) const
{
	return this->adaptor_->UploadImage(this->GetSessionKey_(), "temp", content);
}

TempImage MiraiClient::UploadTempImage(std::istream& file) const
{
	string s;
	char buffer[4096];                        // NOLINT(*-avoid-c-arrays)
	while (file.read(buffer, sizeof(buffer))) // NOLINT(*-array-to-pointer-decay)
		s.append(buffer, sizeof(buffer));     // NOLINT(*-array-to-pointer-decay)
	s.append(buffer, file.gcount());          // NOLINT(*-array-to-pointer-decay)
	return this->UploadTempImage(s);
}

TempImage
MiraiClient::UploadTempImage(std::function<bool(size_t offset, std::ostream& sink, bool& finish)> ContentProvider) const
{
	return this->adaptor_->UploadImageChunked(this->GetSessionKey_(), "temp", std::move(ContentProvider));
}


GroupAudio MiraiClient::UploadGroupAudio(const string& content) const
{
	return this->adaptor_->UploadAudio(this->GetSessionKey_(), "group", content);
}

GroupAudio MiraiClient::UploadGroupAudio(std::istream& file) const
{
	string s;
	char buffer[4096];                        // NOLINT(*-avoid-c-arrays)
	while (file.read(buffer, sizeof(buffer))) // NOLINT(*-array-to-pointer-decay)
		s.append(buffer, sizeof(buffer));     // NOLINT(*-array-to-pointer-decay)
	s.append(buffer, file.gcount());          // NOLINT(*-array-to-pointer-decay)
	return this->UploadGroupAudio(s);
}

GroupAudio
MiraiClient::UploadGroupAudio(std::function<bool(size_t offset, std::ostream& sink, bool& finish)> ContentProvider) const
{
	return this->adaptor_->UploadAudioChunked(this->GetSessionKey_(), "group", std::move(ContentProvider));
}


void MiraiClient::DeleteFriend(QQ_t qq) const
{
	return this->adaptor_->DeleteFriend(this->GetSessionKey_(), qq);
}

void MiraiClient::Mute(GID_t GroupId, QQ_t member, std::chrono::seconds time) const
{
	return this->adaptor_->Mute(this->GetSessionKey_(), GroupId, member, time);
}

void MiraiClient::Mute(const GroupMember& member, std::chrono::seconds time) const
{
	return this->adaptor_->Mute(this->GetSessionKey_(), member.group.id, member.id, time);
}

void MiraiClient::Unmute(GID_t GroupId, QQ_t member) const
{
	return this->adaptor_->Unmute(this->GetSessionKey_(), GroupId, member);
}

void MiraiClient::Unmute(const GroupMember& member) const
{
	return this->adaptor_->Unmute(this->GetSessionKey_(), member.group.id, member.id);
}

void MiraiClient::Kick(GID_t GroupId, QQ_t member, const string& message) const
{
	return this->adaptor_->Kick(this->GetSessionKey_(), GroupId, member, message);
}

void MiraiClient::Kick(const GroupMember& member, const string& message) const
{
	return this->adaptor_->Kick(this->GetSessionKey_(), member.group.id, member.id, message);
}

void MiraiClient::LeaveGroup(GID_t GroupId) const
{
	return this->adaptor_->Quit(this->GetSessionKey_(), GroupId);
}

void MiraiClient::MuteAll(GID_t GroupId) const
{
	return this->adaptor_->MuteAll(this->GetSessionKey_(), GroupId);
}

void MiraiClient::UnmuteAll(GID_t GroupId) const
{
	return this->adaptor_->UnmuteAll(this->GetSessionKey_(), GroupId);
}

void MiraiClient::SetEssence(GID_t GroupId, MessageId_t MessageId) const
{
	return this->adaptor_->SetEssence(this->GetSessionKey_(), MessageId, GroupId);
}


GroupConfig MiraiClient::GetGroupConfig(GID_t GroupId) const
{
	return this->adaptor_->GetGroupConfig(this->GetSessionKey_(), GroupId);
}

void MiraiClient::SetGroupConfig(GID_t GroupId, const string& name, std::optional<bool> AllowMemberInvite) const
{
	return this->adaptor_->SetGroupConfig(this->GetSessionKey_(), GroupId, name, std::nullopt, AllowMemberInvite);
}

GroupMember MiraiClient::GetMemberInfo(GID_t GroupId, QQ_t member) const
{
	return this->adaptor_->GetMemberInfo(this->GetSessionKey_(), GroupId, member);
}

void MiraiClient::SetMemberInfo(GID_t GroupId, QQ_t member, const string& name, const string& title) const
{
	return this->adaptor_->SetMemberInfo(this->GetSessionKey_(), GroupId, member, name, title);
}

void MiraiClient::SetGroupAdmin(GID_t GroupId, QQ_t member, bool assign) const
{
	return this->adaptor_->MemberAdmin(this->GetSessionKey_(), GroupId, member, assign);
}


std::vector<GroupAnnouncement> MiraiClient::GetAnnouncementList(GID_t GroupId, int64_t offset, int64_t size) const
{
	return this->adaptor_->AnnoList(this->GetSessionKey_(), GroupId, offset, size);
}

GroupAnnouncement MiraiClient::PublishAnnouncement(GID_t GroupId, const string& content, const MiraiImage& cover,
                                                   bool ToNewMember, bool pinned, bool ShowEditCard, bool ShowPopup,
                                                   bool RequireConfirm) const
{
	return this->adaptor_->AnnoPublish(this->GetSessionKey_(), GroupId, content, cover.url, cover.path,
	                                    cover.base64, ToNewMember, pinned, ShowEditCard, ShowPopup, RequireConfirm);
}

void MiraiClient::DeleteAnnouncement(GID_t GroupId, const string& fid) const
{
	return this->adaptor_->AnnoDelete(this->GetSessionKey_(), GroupId, fid);
}

void MiraiClient::DeleteAnnouncement(const GroupAnnouncement& announcement) const
{
	return this->adaptor_->AnnoDelete(this->GetSessionKey_(), announcement.group.id, announcement.fid);
}


void MiraiClient::RespNewFriendRequestEvent(int64_t EventId, QQ_t FromId, GID_t GroupId, NewFriendRequestOp operation,
                                            const string& message) const
{
	return this->adaptor_->RespNewFriendRequestEvent(this->GetSessionKey_(), EventId, FromId, GroupId,
	                                                          (int)operation, message);
}

void MiraiClient::RespNewFriendRequestEvent(const NewFriendRequestEvent& event, NewFriendRequestOp operation,
                                            const string& message) const
{
	return this->adaptor_->RespNewFriendRequestEvent(
		this->GetSessionKey_(), event.GetEventId(), event.GetUserId(), event.GetGroupId(), (int)operation, message);
}

void MiraiClient::RespMemberJoinRequestEvent(int64_t EventId, QQ_t FromId, GID_t GroupId, MemberJoinRequestOp operation,
                                             const string& message) const
{
	return this->adaptor_->RespMemberJoinRequestEvent(this->GetSessionKey_(), EventId, FromId, GroupId,
	                                                           (int)operation, message);
}

void MiraiClient::RespMemberJoinRequestEvent(const MemberJoinRequestEvent& event, MemberJoinRequestOp operation,
                                             const string& message) const
{
	return this->adaptor_->RespMemberJoinRequestEvent(
		this->GetSessionKey_(), event.GetEventId(), event.GetUserId(), event.GetGroupId(), (int)operation, message);
}

void MiraiClient::RespBotInvitedJoinGroupRequestEvent(int64_t EventId, QQ_t FromId, GID_t GroupId,
                                                      BotInvitedJoinGroupRequestOp operation, const string& message) const
{
	return this->adaptor_->RespBotInvitedJoinGroupRequestEvent(this->GetSessionKey_(), EventId, FromId,
	                                                                    GroupId, (int)operation, message);
}

void MiraiClient::RespBotInvitedJoinGroupRequestEvent(const BotInvitedJoinGroupRequestEvent& event,
                                                      BotInvitedJoinGroupRequestOp operation, const string& message) const
{
	return this->adaptor_->RespBotInvitedJoinGroupRequestEvent(
		this->GetSessionKey_(), event.GetEventId(), event.GetUserId(), event.GetGroupId(), (int)operation, message);
}


void MiraiClient::RegisterCommand(const string& name, const std::vector<string>& alias, const string& usage,
                                  const string& description) const
{
	return this->adaptor_->CmdRegister(this->GetSessionKey_(), name, alias, usage, description);
}
void MiraiClient::ExecuteCommand(const MessageChain& command) const
{
	return this->adaptor_->CmdExecute(this->GetSessionKey_(), command);
}


string MiraiClient::CallAPI(const string& path, const string& method, const string& data) const
{
	return this->adaptor_->CallAPI(path, method, data);
}

} // namespace Mirai