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

#include "HttpWsAdaptor.hpp"
#include <exception>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <thread>

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocketMessageType.h>
#include <nlohmann/json.hpp>
#include <libmirai/Utils/Common.hpp>
#include <libmirai/Serialization/Types/Types.hpp>
#include <libmirai/Serialization/Events/Events.hpp>
#include <libmirai/Exceptions/Exceptions.hpp>

namespace Mirai
{

using std::string;
using json = nlohmann::json;

namespace
{

constexpr const char* JSON_CONTENT_TYPE = "application/json;charset=UTF-8";

void ConfigFromJson(HttpWsAdaptorConfig& config, const json& json_config)
{
	config.HttpUrl = Utils::GetValue(json_config, "HttpUrl", config.HttpUrl);
	config.ConnectionTimeout =
		std::chrono::milliseconds(Utils::GetValue(json_config, "ConnectionTimeout", config.ConnectionTimeout.count()));
	config.ReadTimeout =
		std::chrono::milliseconds(Utils::GetValue(json_config, "ReadTimeout", config.ReadTimeout.count()));
	config.WriteTimeout =
		std::chrono::milliseconds(Utils::GetValue(json_config, "WriteTimeout", config.WriteTimeout.count()));

	config.WebsocketUrl = Utils::GetValue(json_config, "WebsocketUrl", config.WebsocketUrl);
	config.HeartbeatInterval =
		std::chrono::seconds(Utils::GetValue(json_config, "HeartbeatInterval", config.HeartbeatInterval.count()));
	config.HandshakeTimeout =
		std::chrono::seconds(Utils::GetValue(json_config, "HandshakeTimeout", config.HandshakeTimeout.count()));
	config.EnablePong = Utils::GetValue(json_config, "EnablePong", config.EnablePong);
	config.EnableDeflate = Utils::GetValue(json_config, "EnableDeflate", config.EnableDeflate);
	config.AutoReconnect = Utils::GetValue(json_config, "AutoReconnect", config.AutoReconnect);
	config.MinRetryInterval =
		std::chrono::milliseconds(Utils::GetValue(json_config, "MinRetryInterval", config.MinRetryInterval.count()));
	config.MaxRetryInterval =
		std::chrono::milliseconds(Utils::GetValue(json_config, "MaxRetryInterval", config.MaxRetryInterval.count()));

	config.BotQQ = Utils::GetValue(json_config, "BotQQ", config.BotQQ);
	config.VerifyKey = Utils::GetValue(json_config, "VerifyKey", config.VerifyKey);
}

} // namespace

void HttpWsAdaptorConfig::FromJsonFile(const std::string& path)
{
	std::ifstream file(path);
	json content = json::parse(file);
	ConfigFromJson(*this, content);
}

std::unique_ptr<IAdaptor> MakeHttpWsAdaptor(HttpWsAdaptorConfig config)
{
	return std::make_unique<Details::HttpWsAdaptor>(std::move(config));
}


namespace Details
{

size_t HttpWsAdaptor::init_ = 0; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

HttpWsAdaptor::HttpWsAdaptor(HttpWsAdaptorConfig config) : config_(std::move(config))
{
	if (!HttpWsAdaptor::init_)
	{
		ix::initNetSystem();
		HttpWsAdaptor::init_++;
	}
	this->wsclient_.setPingInterval(static_cast<int>(this->config_.HeartbeatInterval.count()));
	this->wsclient_.setHandshakeTimeout(static_cast<int>(this->config_.HandshakeTimeout.count()));
	if (!this->config_.EnablePong) this->wsclient_.disablePong();
	if (this->config_.EnableDeflate) this->wsclient_.enablePerMessageDeflate();
	if (this->config_.AutoReconnect)
	{
		this->wsclient_.enableAutomaticReconnection();
		this->wsclient_.setMinWaitBetweenReconnectionRetries(this->config_.MinRetryInterval.count());
		this->wsclient_.setMaxWaitBetweenReconnectionRetries(this->config_.MaxRetryInterval.count());
	}
	else
		this->wsclient_.disableAutomaticReconnection();
}

HttpWsAdaptor::~HttpWsAdaptor()
{
	if (HttpWsAdaptor::init_)
	{
		ix::uninitNetSystem();
		HttpWsAdaptor::init_--;
	}
}

httplib::Client HttpWsAdaptor::CreateClient_() const
{
	httplib::Client client(this->config_.HttpUrl);
	client.set_connection_timeout(this->config_.ConnectionTimeout.count() / 1000,
						(this->config_.ConnectionTimeout.count() % 1000) * 1000);
	client.set_read_timeout(this->config_.ReadTimeout.count() / 1000, 
				(this->config_.ReadTimeout.count() % 1000) * 1000);
	client.set_write_timeout(this->config_.WriteTimeout.count() / 1000, 
				(this->config_.WriteTimeout.count() % 1000) * 1000);

	client.set_keep_alive(true);
	client.set_follow_location(true);
	return client;
}

httplib::Client& HttpWsAdaptor::GetClient_()
{
	auto id = std::this_thread::get_id();

	std::lock_guard<std::mutex> lk(this->httpmtx_);
	auto it = this->httpclients_.try_emplace(id, this->config_.HttpUrl).first;
	return it->second;
}

string HttpWsAdaptor::Connect()
{
	if (this->wsclient_.getReadyState() == ix::ReadyState::Open)
		this->wsclient_.stop();
	this->httpclients_.clear();
	this->info_ = std::nullopt;
	this->bind_ = false;

	string key;
	{
		json body = {{"verifyKey", this->config_.VerifyKey}};
		auto result = this->GetClient_().Post("/verify", body.dump(), JSON_CONTENT_TYPE);
		json resp = Utils::ParseResponse(result);
		resp.at("session").get_to(key);
	}
	{
		json body = {{"sessionKey", key}, {"qq", int64_t(this->config_.BotQQ)}};
		auto result = this->GetClient_().Post("/bind", body.dump(), JSON_CONTENT_TYPE);
		(void)Utils::ParseResponse(result);
	}
	this->bind_ = true;

	this->wsclient_.setUrl(this->config_.WebsocketUrl + "/all?verifyKey=" + this->config_.VerifyKey
	                              + "&sessionKey=" + key);

	this->wsclient_.setOnMessageCallback(
		[this](const ix::WebSocketMessagePtr& msg)
		{
			switch (msg->type)
			{
			case ix::WebSocketMessageType::Open:
				if (this->ConnectionEstablishedCallback_) 
				{
					{
						std::lock_guard<std::mutex> lk(this->connectmtx_);
						this->info_ = ClientConnectionEstablishedEvent();
						this->info_->uri = msg->openInfo.uri;
						this->info_->headers = 
							{
								std::make_move_iterator(msg->openInfo.headers.begin()), 
								std::make_move_iterator(msg->openInfo.headers.end())
							};
						this->info_->protocol = msg->openInfo.protocol;
					}
					this->connectcv_.notify_all();
					{
						// Avoid calling other callbacks before OnConnectionEstablished
						std::unique_lock<std::mutex> lk(this->connectmtx_);
						this->connectcv_.wait(lk, [this]() { return !this->info_; });
					}
				}
				break;
			case ix::WebSocketMessageType::Message:
				if (!msg->binary)
				{
					if (this->TextCallback_)
					{
						this->TextCallback_(msg->str);
					}
				}
				break;
			case ix::WebSocketMessageType::Error:
				if (this->ConnectionErrorCallback_)
				{
					ClientConnectionErrorEvent event;
					event.RetryCount = msg->errorInfo.retries;
					event.WaitTime = msg->errorInfo.wait_time;
					event.HttpStatus = msg->errorInfo.http_status;
					event.reason = std::move(msg->errorInfo.reason);
					event.DecompressionError = msg->errorInfo.decompressionError;
					this->ConnectionErrorCallback_(std::move(event));
				}
				break;
			case ix::WebSocketMessageType::Close:
				if (this->ConnectionClosedCallback_)
				{
					ClientConnectionClosedEvent event;
					event.code = msg->closeInfo.code;
					event.reason = std::move(msg->closeInfo.reason);
					event.remote = msg->closeInfo.remote;
					this->ConnectionClosedCallback_(std::move(event));
				}
				break;
			default:
				// do nothing
				break;
			}
		});
	this->wsclient_.start();

	if (this->ConnectionEstablishedCallback_) 
	{
		ClientConnectionEstablishedEvent event;
		{
			std::unique_lock<std::mutex> lk(this->connectmtx_);
			this->connectcv_.wait(lk, [this]() { return this->info_; });

			this->info_->SessionKey = key;

			httplib::Params params = {{"sessionKey", key}};
			auto result = this->GetClient_().Get("/sessionInfo", params, httplib::Headers{});
			json resp = Utils::ParseResponse(result);

			MIRAI_PARSE_GUARD_BEGIN(resp);
			resp.at("data").at("qq").get_to(this->info_->BotProfile);
			MIRAI_PARSE_GUARD_END(resp);

			event = std::move(this->info_.value());
		}

		this->ConnectionEstablishedCallback_(std::move(event));

		{
			std::lock_guard<std::mutex> lk(this->connectmtx_);
			this->info_ = std::nullopt;
		}
		this->connectcv_.notify_all();
	}
	return key;
}

void HttpWsAdaptor::Disconnect(const string& SessionKey)
{
	if (this->wsclient_.getReadyState() == ix::ReadyState::Open)
		this->wsclient_.stop();

	if (this->bind_)
	{
		json body = {{"sessionKey", SessionKey}, {"qq", int64_t(this->config_.BotQQ)}};
		auto result = this->GetClient_().Post("/release", body.dump(), JSON_CONTENT_TYPE);
		(void)Utils::ParseResponse(result);
		this->bind_ = false;
	}
}


// *********************************************
// *************** Request API *****************
// *********************************************


string HttpWsAdaptor::Version()
{
	auto result = this->GetClient_().Get("/about");
	json resp = Utils::ParseResponse(result);

	MIRAI_PARSE_GUARD_BEGIN(resp);
	return resp.at("data").at("version").get<string>();
	MIRAI_PARSE_GUARD_END(resp);
}

QQ_t HttpWsAdaptor::GetBotQQ()
{
	return this->config_.BotQQ;
}

std::vector<QQ_t> HttpWsAdaptor::BotList()
{
	auto result = this->GetClient_().Get("/botList");
	json resp = Utils::ParseResponse(result);

	MIRAI_PARSE_GUARD_BEGIN(resp);
	return resp.at("data").get<std::vector<QQ_t>>();
	MIRAI_PARSE_GUARD_END(resp);
}


std::variant<FriendMessageEvent, GroupMessageEvent, TempMessageEvent, StrangerMessageEvent> 
HttpWsAdaptor::MessageFromId(const string& SessionKey, MessageId_t id, UID_t target)
{
	httplib::Params params = {
		{"sessionKey", SessionKey}, {"messageId", std::to_string(id)}, {"target", target.to_string()}};
	auto result = this->GetClient_().Get("/messageFromId", params, httplib::Headers{});
	json resp = Utils::ParseResponse(result);
	
	MIRAI_PARSE_GUARD_BEGIN(resp);
	EventTypes type = resp.at("data").at("type").get<EventTypes>();
	std::variant<FriendMessageEvent, GroupMessageEvent, TempMessageEvent, StrangerMessageEvent> m;
	std::visit([&type, &resp](auto&& p)
		{
			using Type = std::decay_t<decltype(p)>;
			if (type == Type::GetType())
			{
				resp.at("data").get_to(p);
			}
		}, m);
	return m;
	MIRAI_PARSE_GUARD_END(resp);
}


std::vector<User> HttpWsAdaptor::FriendList(const string& SessionKey)
{
	httplib::Params params = {{"sessionKey", SessionKey}};
	auto result = this->GetClient_().Get("/friendList", params, httplib::Headers{});
	json resp = Utils::ParseResponse(result);
	
	MIRAI_PARSE_GUARD_BEGIN(resp);
	return resp.at("data").get<std::vector<User>>();
	MIRAI_PARSE_GUARD_END(resp);
}

std::vector<Group> HttpWsAdaptor::GroupList(const string& SessionKey)
{
	httplib::Params params = {{"sessionKey", SessionKey}};
	auto result = this->GetClient_().Get("/groupList", params, httplib::Headers{});
	json resp = Utils::ParseResponse(result);
	
	MIRAI_PARSE_GUARD_BEGIN(resp);
	return resp.at("data").get<std::vector<Group>>();
	MIRAI_PARSE_GUARD_END(resp);
}

std::vector<GroupMember> HttpWsAdaptor::MemberList(const string& SessionKey, GID_t target)
{
	httplib::Params params = {{"sessionKey", SessionKey}, {"target", target.to_string()}};
	auto result = this->GetClient_().Get("/memberList", params, httplib::Headers{});
	json resp = Utils::ParseResponse(result);
	
	MIRAI_PARSE_GUARD_BEGIN(resp);
	return resp.at("data").get<std::vector<GroupMember>>();
	MIRAI_PARSE_GUARD_END(resp);
}


UserProfile HttpWsAdaptor::GetBotProfile(const string& SessionKey)
{
	httplib::Params params = {{"sessionKey", SessionKey}};
	auto result = this->GetClient_().Get("/botProfile", params, httplib::Headers{});
	json resp = Utils::ParseResponse(result);
	
	MIRAI_PARSE_GUARD_BEGIN(resp);
	return resp.get<UserProfile>();
	MIRAI_PARSE_GUARD_END(resp);
}

UserProfile HttpWsAdaptor::GetFriendProfile(const string& SessionKey, QQ_t target)
{
	httplib::Params params = {{"sessionKey", SessionKey}, {"target", target.to_string()}};
	auto result = this->GetClient_().Get("/friendProfile", params, httplib::Headers{});
	json resp = Utils::ParseResponse(result);
	
	MIRAI_PARSE_GUARD_BEGIN(resp);
	return resp.get<UserProfile>();
	MIRAI_PARSE_GUARD_END(resp);
}

UserProfile HttpWsAdaptor::GetMemberProfile(const string& SessionKey, GID_t target, QQ_t MemberId)
{
	httplib::Params params = {
		{"sessionKey", SessionKey}, {"target", target.to_string()}, {"memberId", MemberId.to_string()}};
	auto result = this->GetClient_().Get("/memberProfile", params, httplib::Headers{});
	json resp = Utils::ParseResponse(result);
	
	MIRAI_PARSE_GUARD_BEGIN(resp);
	return resp.get<UserProfile>();
	MIRAI_PARSE_GUARD_END(resp);
}

UserProfile HttpWsAdaptor::GetUserProfile(const string& SessionKey, QQ_t target)
{
	httplib::Params params = {{"sessionKey", SessionKey}, {"target", target.to_string()}};
	auto result = this->GetClient_().Get("/userProfile", params, httplib::Headers{});
	json resp = Utils::ParseResponse(result);
	
	MIRAI_PARSE_GUARD_BEGIN(resp);
	return resp.get<UserProfile>();
	MIRAI_PARSE_GUARD_END(resp);
}


MessageId_t HttpWsAdaptor::SendFriendMessage(const string& SessionKey, QQ_t qq, const MessageChain& message,
                                       std::optional<MessageId_t> QuoteId)
{
	json body = {{"sessionKey", SessionKey}, {"qq", qq}, {"messageChain", message}};
	if (QuoteId.has_value()) body["quote"] = QuoteId.value();
	auto result = this->GetClient_().Post("/sendFriendMessage", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	
	MIRAI_PARSE_GUARD_BEGIN(resp);
	return resp.at("messageId").get<MessageId_t>();
	MIRAI_PARSE_GUARD_END(resp);
}

MessageId_t HttpWsAdaptor::SendGroupMessage(const string& SessionKey, GID_t group, const MessageChain& message,
                                      std::optional<MessageId_t> QuoteId)
{
	json body = {{"sessionKey", SessionKey}, {"group", group}, {"messageChain", message}};
	if (QuoteId.has_value()) body["quote"] = QuoteId.value();
	auto result = this->GetClient_().Post("/sendGroupMessage", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	
	MIRAI_PARSE_GUARD_BEGIN(resp);
	return resp.at("messageId").get<MessageId_t>();
	MIRAI_PARSE_GUARD_END(resp);
}

MessageId_t HttpWsAdaptor::SendTempMessage(const string& SessionKey, QQ_t qq, GID_t group, const MessageChain& message,
                                     std::optional<MessageId_t> QuoteId)
{
	json body = {{"sessionKey", SessionKey}, {"qq", qq}, {"group", group}, {"messageChain", message}};
	if (QuoteId.has_value()) body["quote"] = QuoteId.value();
	auto result = this->GetClient_().Post("/sendTempMessage", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	
	MIRAI_PARSE_GUARD_BEGIN(resp);
	return resp.at("messageId").get<MessageId_t>();
	MIRAI_PARSE_GUARD_END(resp);
}

void HttpWsAdaptor::SendNudge(const string& SessionKey, QQ_t target, UID_t subject, NudgeType kind)
{
	json body = {{"sessionKey", SessionKey}, {"target", target}, {"subject", subject}, {"kind", kind}};
	auto result = this->GetClient_().Post("/sendNudge", body.dump(), JSON_CONTENT_TYPE);
	(void)Utils::ParseResponse(result);
}

void HttpWsAdaptor::Recall(const string& SessionKey, MessageId_t id, UID_t target)
{
	json body = {{"sessionKey", SessionKey}, {"messageId", id}, {"target", target}};
	auto result = this->GetClient_().Post("/recall", body.dump(), JSON_CONTENT_TYPE);
	(void)Utils::ParseResponse(result);
}

std::vector<MessageChain> HttpWsAdaptor::RoamingMessages(const string& SessionKey, std::time_t TimeStart, std::time_t TimeEnd, UID_t target)
{
	json body = {{"sessionKey", SessionKey}, {"timeStart", TimeStart}, {"timeEnd", TimeEnd}, {"target", target}};
	auto result = this->GetClient_().Post("/roamingMessages", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	
	MIRAI_PARSE_GUARD_BEGIN(resp);
	return resp.at("data").get<std::vector<MessageChain>>();
	MIRAI_PARSE_GUARD_END(resp);
}


std::vector<GroupFileInfo> HttpWsAdaptor::FileList(const string& SessionKey, const string& id, const string& path, UID_t target,
                              int64_t offset, int64_t size, bool withDownloadInfo)
{
	httplib::Params params = {{"sessionKey", SessionKey},
	                          {"target", target.to_string()},
	                          {"withDownloadInfo", withDownloadInfo ? "true" : "false"}};
	if (!path.empty()) params.emplace("path", path);
	else
		params.emplace("id", id);
	if (offset > 0) params.emplace("offset", std::to_string(offset));
	if (size > 0) params.emplace("size", std::to_string(size));
	auto result = this->GetClient_().Get("/file/list", params, httplib::Headers{});
	json resp = Utils::ParseResponse(result);
	
	MIRAI_PARSE_GUARD_BEGIN(resp);
	return resp.at("data").get<std::vector<GroupFileInfo>>();
	MIRAI_PARSE_GUARD_END(resp);
}

GroupFileInfo HttpWsAdaptor::FileInfo(const string& SessionKey, const string& id, const string& path, UID_t target,
                              bool withDownloadInfo)
{
	httplib::Params params = {{"sessionKey", SessionKey},
	                          {"target", target.to_string()},
	                          {"withDownloadInfo", withDownloadInfo ? "true" : "false"}};
	if (!path.empty()) params.emplace("path", path);
	else
		params.emplace("id", id);
	auto result = this->GetClient_().Get("/file/info", params, httplib::Headers{});
	json resp = Utils::ParseResponse(result);
	
	MIRAI_PARSE_GUARD_BEGIN(resp);
	return resp.at("data").get<GroupFileInfo>();
	MIRAI_PARSE_GUARD_END(resp);
}

GroupFileInfo HttpWsAdaptor::FileMkdir(const string& SessionKey, const string& id, const string& path, UID_t target,
                               const string& directory)
{
	json body = {{"sessionKey", SessionKey}, {"target", target}, {"directoryName", directory}};
	if (!path.empty()) body["path"] = path;
	else
		body["id"] = id;
	auto result = this->GetClient_().Post("/file/mkdir", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	
	MIRAI_PARSE_GUARD_BEGIN(resp);
	return resp.at("data").get<GroupFileInfo>();
	MIRAI_PARSE_GUARD_END(resp);
}

void HttpWsAdaptor::FileDelete(const string& SessionKey, const string& id, const string& path, UID_t target)
{
	json body = {{"sessionKey", SessionKey}, {"target", target}};
	if (!path.empty()) body["path"] = path;
	else
		body["id"] = id;
	auto result = this->GetClient_().Post("/file/delete", body.dump(), JSON_CONTENT_TYPE);
	(void)Utils::ParseResponse(result);
}

void HttpWsAdaptor::FileMove(const string& SessionKey, const string& id, const string& path, UID_t target,
                              const string& TargetDirId, const string& TargetDirPath)
{
	json body = {{"sessionKey", SessionKey}, {"target", target}};
	if (!path.empty()) body["path"] = path;
	else
		body["id"] = id;
	if (!TargetDirPath.empty()) body["moveToPath"] = TargetDirPath;
	else
		body["moveTo"] = TargetDirId;
	auto result = this->GetClient_().Post("/file/move", body.dump(), JSON_CONTENT_TYPE);
	(void)Utils::ParseResponse(result);
}

void HttpWsAdaptor::FileRename(const string& SessionKey, const string& id, const string& path, UID_t target,
                                const string& name)
{
	json body = {{"sessionKey", SessionKey}, {"target", target}, {"renameTo", name}};
	if (!path.empty()) body["path"] = path;
	else
		body["id"] = id;
	auto result = this->GetClient_().Post("/file/rename", body.dump(), JSON_CONTENT_TYPE);
	(void)Utils::ParseResponse(result);
}

GroupFileInfo HttpWsAdaptor::FileUpload(const string& SessionKey, const string& path, UID_t target, const string& type,
                                const string& name, string content)
{
	httplib::MultipartFormDataItems items = {{"sessionKey", SessionKey, "", ""},
	                                         {"path", path, "", ""},
	                                         {"target", target.to_string(), "", ""},
	                                         {"type", type, "", ""}};
	items.emplace_back(httplib::MultipartFormData{"file", std::move(content), name, "application/octet-stream"});

	this->GetClient_().set_compress(true);
	auto result = this->GetClient_().Post("/file/upload", items);

	json resp = Utils::ParseResponse(result);
	
	MIRAI_PARSE_GUARD_BEGIN(resp);
	return resp.at("data").get<GroupFileInfo>();
	MIRAI_PARSE_GUARD_END(resp);
}

GroupFileInfo HttpWsAdaptor::FileUploadChunked(
	const string& SessionKey, const string& path, UID_t target, const string& type, const string& name,
	std::function<bool(size_t offset, std::ostream& sink, bool& finish)> ContentProvider)
{
	httplib::MultipartFormDataItems items = {{"sessionKey", SessionKey, "", ""},
	                                         {"path", path, "", ""},
	                                         {"target", target.to_string(), "", ""},
	                                         {"type", type, "", ""}};

	// TODO: Remove httplib::detail dependency
	string boundary = httplib::detail::make_multipart_data_boundary();


	auto result = this->GetClient_().Post(
		"/file/upload",
		[&](size_t offset, httplib::DataSink& sink) -> bool
		{
			if (offset == 0)
			{
				for (const auto& item : items)
				{
					sink.os << "--" + boundary + "\r\n";
					sink.os << "Content-Disposition: form-data; name=\"" + item.name + "\"";
					if (!item.filename.empty()) sink.os << "; filename=\"" + item.filename + "\"";
					sink.os << "\r\n";
					if (!item.content_type.empty()) sink.os << "Content-Type: " + item.content_type + "\r\n";
					sink.os << "\r\n";
					sink.os << item.content + "\r\n";
				}

				sink.os << "--" + boundary + "\r\n";
				sink.os << "Content-Disposition: form-data; name=\"file\"";
				sink.os << "; filename=\"" + name + "\"";
				sink.os << "\r\n";
				sink.os << "Content-Type: application/octet-stream\r\n";
				sink.os << "\r\n";
			}

			bool finish = false;
			if (!ContentProvider(offset, sink.os, finish)) return false;

			if (finish)
			{
				sink.os << "\r\n--" + boundary + "--\r\n";
				sink.done();
			}
			return true;
		},
		"multipart/form-data;boundary=\"" + boundary + "\"");


	json resp = Utils::ParseResponse(result);
	
	MIRAI_PARSE_GUARD_BEGIN(resp);
	return resp.at("data").get<GroupFileInfo>();
	MIRAI_PARSE_GUARD_END(resp);
}


MiraiImage HttpWsAdaptor::UploadImage(const string& SessionKey, const string& type, string image)
{
	httplib::MultipartFormDataItems items = {{"sessionKey", SessionKey, "", ""}, {"type", type, "", ""}};
	items.emplace_back(httplib::MultipartFormData{"img", std::move(image), "Image", "application/octet-stream"});

	auto result = this->GetClient_().Post("/uploadImage", items);

	json resp = Utils::ParseResponse(result);
	
	MIRAI_PARSE_GUARD_BEGIN(resp);
	return resp.get<MiraiImage>();
	MIRAI_PARSE_GUARD_END(resp);
}

MiraiImage HttpWsAdaptor::UploadImageChunked(
	const string& SessionKey, const string& type,
	std::function<bool(size_t offset, std::ostream& sink, bool& finish)> ContentProvider)
{
	httplib::MultipartFormDataItems items = {{"sessionKey", SessionKey, "", ""}, {"type", type, "", ""}};
	// TODO: Remove httplib::detail dependency
	string boundary = httplib::detail::make_multipart_data_boundary();


	auto result = this->GetClient_().Post(
		"/uploadImage",
		[&](size_t offset, httplib::DataSink& sink) -> bool
		{
			if (offset == 0)
			{
				for (const auto& item : items)
				{
					sink.os << "--" + boundary + "\r\n";
					sink.os << "Content-Disposition: form-data; name=\"" + item.name + "\"";
					if (!item.filename.empty()) sink.os << "; filename=\"" + item.filename + "\"";
					sink.os << "\r\n";
					if (!item.content_type.empty()) sink.os << "Content-Type: " + item.content_type + "\r\n";
					sink.os << "\r\n";
					sink.os << item.content + "\r\n";
				}

				sink.os << "--" + boundary + "\r\n";
				sink.os << "Content-Disposition: form-data; name=\"img\"";
				sink.os << "; filename=\"Image\"";
				sink.os << "\r\n";
				sink.os << "Content-Type: application/octet-stream\r\n";
				sink.os << "\r\n";
			}

			bool finish = false;
			if (!ContentProvider(offset, sink.os, finish)) return false;

			if (finish)
			{
				sink.os << "\r\n--" + boundary + "--\r\n";
				sink.done();
			}
			return true;
		},
		"multipart/form-data;boundary=\"" + boundary + "\"");


	json resp = Utils::ParseResponse(result);
	
	MIRAI_PARSE_GUARD_BEGIN(resp);
	return resp.get<MiraiImage>();
	MIRAI_PARSE_GUARD_END(resp);
}

MiraiAudio HttpWsAdaptor::UploadAudio(const string& SessionKey, const string& type, string Audio)
{
	httplib::MultipartFormDataItems items = {
		{"sessionKey", SessionKey, "", ""},
		{"type", type, "", ""},
	};
	items.emplace_back(httplib::MultipartFormData{"voice", std::move(Audio), "Audio", "application/octet-stream"});

	auto result = this->GetClient_().Post("/uploadVoice", items);

	json resp = Utils::ParseResponse(result);
	
	MIRAI_PARSE_GUARD_BEGIN(resp);
	return resp.get<MiraiAudio>();
	MIRAI_PARSE_GUARD_END(resp);
}

MiraiAudio HttpWsAdaptor::UploadAudioChunked(
	const string& SessionKey, const string& type,
	std::function<bool(size_t offset, std::ostream& sink, bool& finish)> ContentProvider)
{
	httplib::MultipartFormDataItems items = {{"sessionKey", SessionKey, "", ""}, {"type", type, "", ""}};
	// TODO: Remove httplib::detail dependency
	string boundary = httplib::detail::make_multipart_data_boundary();


	auto result = this->GetClient_().Post(
		"/uploadVoice",
		[&](size_t offset, httplib::DataSink& sink) -> bool
		{
			if (offset == 0)
			{
				for (const auto& item : items)
				{
					sink.os << "--" + boundary + "\r\n";
					sink.os << "Content-Disposition: form-data; name=\"" + item.name + "\"";
					if (!item.filename.empty()) sink.os << "; filename=\"" + item.filename + "\"";
					sink.os << "\r\n";
					if (!item.content_type.empty()) sink.os << "Content-Type: " + item.content_type + "\r\n";
					sink.os << "\r\n";
					sink.os << item.content + "\r\n";
				}

				sink.os << "--" + boundary + "\r\n";
				sink.os << "Content-Disposition: form-data; name=\"voice\"";
				sink.os << "; filename=\"Audio\"";
				sink.os << "\r\n";
				sink.os << "Content-Type: application/octet-stream\r\n";
				sink.os << "\r\n";
			}

			bool finish = false;
			if (!ContentProvider(offset, sink.os, finish)) return false;

			if (finish)
			{
				sink.os << "\r\n--" + boundary + "--\r\n";
				sink.done();
			}
			return true;
		},
		"multipart/form-data;boundary=\"" + boundary + "\"");

	json resp = Utils::ParseResponse(result);
	
	MIRAI_PARSE_GUARD_BEGIN(resp);
	return resp.get<MiraiAudio>();
	MIRAI_PARSE_GUARD_END(resp);
}


void HttpWsAdaptor::DeleteFriend(const string& SessionKey, QQ_t target)
{
	json body = {{"sessionKey", SessionKey}, {"target", target}};
	auto result = this->GetClient_().Post("/deleteFriend", body.dump(), JSON_CONTENT_TYPE);
	(void)Utils::ParseResponse(result);
}


void HttpWsAdaptor::Mute(const string& SessionKey, GID_t target, QQ_t member, std::chrono::seconds time)
{
	json body = {{"sessionKey", SessionKey}, {"target", target}, {"memberId", member}, {"time", time.count()}};
	auto result = this->GetClient_().Post("/mute", body.dump(), JSON_CONTENT_TYPE);
	(void)Utils::ParseResponse(result);
}

void HttpWsAdaptor::Unmute(const string& SessionKey, GID_t target, QQ_t member)
{
	json body = {{"sessionKey", SessionKey}, {"target", target}, {"memberId", member}};
	auto result = this->GetClient_().Post("/unmute", body.dump(), JSON_CONTENT_TYPE);
	(void)Utils::ParseResponse(result);
}

void HttpWsAdaptor::Kick(const string& SessionKey, GID_t target, QQ_t member, const string& message)
{
	json body = {{"sessionKey", SessionKey}, {"target", target}, {"memberId", member}, {"msg", message}};
	auto result = this->GetClient_().Post("/kick", body.dump(), JSON_CONTENT_TYPE);
	(void)Utils::ParseResponse(result);
}

void HttpWsAdaptor::Quit(const string& SessionKey, GID_t target)
{
	json body = {{"sessionKey", SessionKey}, {"target", target}};
	auto result = this->GetClient_().Post("/quit", body.dump(), JSON_CONTENT_TYPE);
	(void)Utils::ParseResponse(result);
}

void HttpWsAdaptor::MuteAll(const string& SessionKey, GID_t target)
{
	json body = {{"sessionKey", SessionKey}, {"target", target}};
	auto result = this->GetClient_().Post("/muteAll", body.dump(), JSON_CONTENT_TYPE);
	(void)Utils::ParseResponse(result);
}

void HttpWsAdaptor::UnmuteAll(const string& SessionKey, GID_t target)
{
	json body = {{"sessionKey", SessionKey}, {"target", target}};
	auto result = this->GetClient_().Post("/unmuteAll", body.dump(), JSON_CONTENT_TYPE);
	(void)Utils::ParseResponse(result);
}

void HttpWsAdaptor::SetEssence(const string& SessionKey, MessageId_t id, GID_t target)
{
	json body = {{"sessionKey", SessionKey}, {"messageId", id}, {"target", target}};
	auto result = this->GetClient_().Post("/setEssence", body.dump(), JSON_CONTENT_TYPE);
	(void)Utils::ParseResponse(result);
}

GroupConfig HttpWsAdaptor::GetGroupConfig(const string& SessionKey, GID_t target)
{
	httplib::Params params = {{"sessionKey", SessionKey}, {"target", target.to_string()}};
	auto result = this->GetClient_().Get("/groupConfig", params, httplib::Headers{});
	json resp = Utils::ParseResponse(result);
	
	MIRAI_PARSE_GUARD_BEGIN(resp);
	return resp.get<GroupConfig>();
	MIRAI_PARSE_GUARD_END(resp);
}

void HttpWsAdaptor::SetGroupConfig(const string& SessionKey, GID_t target, const string& name,
                                     std::optional<bool> AllowConfessTalk, std::optional<bool> AllowMemberInvite,
                                     std::optional<bool> AutoApprove, std::optional<bool> AllowAnonymousChat)
{
	json body = {{"sessionKey", SessionKey}, {"target", target}, {"config", json::object()}};
	if (!name.empty()) body.at("config")["name"] = name;
	if (AllowConfessTalk.has_value()) body.at("config")["confessTalk"] = AllowConfessTalk.value();
	if (AllowMemberInvite.has_value()) body.at("config")["AllowMemberInvite"] = AllowMemberInvite.value();
	if (AutoApprove.has_value()) body.at("config")["autoApprove"] = AutoApprove.value();
	if (AllowAnonymousChat.has_value()) body.at("config")["anonymousChat"] = AllowAnonymousChat.value();
	auto result = this->GetClient_().Post("/groupConfig", body.dump(), JSON_CONTENT_TYPE);
	(void)Utils::ParseResponse(result);
}

GroupMember HttpWsAdaptor::GetMemberInfo(const string& SessionKey, GID_t target, QQ_t member)
{
	httplib::Params params = {
		{"sessionKey", SessionKey}, {"target", target.to_string()}, {"memberId", member.to_string()}};
	auto result = this->GetClient_().Get("/memberInfo", params, httplib::Headers{});
	json resp = Utils::ParseResponse(result);
	
	MIRAI_PARSE_GUARD_BEGIN(resp);
	return resp.get<GroupMember>();
	MIRAI_PARSE_GUARD_END(resp);
}

void HttpWsAdaptor::SetMemberInfo(const string& SessionKey, GID_t target, QQ_t member, const string& name,
                                    const string& title)
{
	json body = {{"sessionKey", SessionKey}, {"target", target}, {"info", json::object()}};
	if (!name.empty()) body["info"]["name"] = name;
	if (!title.empty()) body["info"]["specialTitle"] = title;
	auto result = this->GetClient_().Post("/memberInfo", body.dump(), JSON_CONTENT_TYPE);
	(void)Utils::ParseResponse(result);
}

void HttpWsAdaptor::MemberAdmin(const string& SessionKey, GID_t target, QQ_t member, bool assign)
{
	json body = {{"sessionKey", SessionKey}, {"target", target}, {"memberId", member}, {"assign", assign}};
	auto result = this->GetClient_().Post("/memberAdmin", body.dump(), JSON_CONTENT_TYPE);
	(void)Utils::ParseResponse(result);
}


std::vector<GroupAnnouncement> HttpWsAdaptor::AnnoList(const string& SessionKey, GID_t target, int64_t offset, int64_t size)
{
	httplib::Params params = {{"sessionKey", SessionKey}, {"id", target.to_string()}};
	if (offset > 0) params.emplace("offset", std::to_string(offset));
	if (size > 0) params.emplace("size", std::to_string(size));
	auto result = this->GetClient_().Get("/anno/list", params, httplib::Headers{});
	json resp = Utils::ParseResponse(result);
	
	MIRAI_PARSE_GUARD_BEGIN(resp);
	return resp.at("data").get<std::vector<GroupAnnouncement>>();
	MIRAI_PARSE_GUARD_END(resp);
}

GroupAnnouncement HttpWsAdaptor::AnnoPublish(const string& SessionKey, GID_t target, const string& content, const string& url,
                                 const string& path, const string& base64, bool ToNewMember, bool pinned,
                                 bool ShowEditCard, bool ShowPopup, bool RequireConfirm)
{
	json body = {{"sessionKey", SessionKey}, {"target", target},
	             {"content", content},       {"sendToNewMember", ToNewMember},
	             {"pinned", pinned},         {"showEditCard", ShowEditCard},
	             {"showPopup", ShowPopup},   {"requireConfirm", RequireConfirm}};
	if (!url.empty()) body["imageUrl"] = url;
	else if (!path.empty())
		body["imagePath"] = path;
	else if (!base64.empty())
		body["imageBase64"] = base64;
	auto result = this->GetClient_().Post("/anno/publish", body.dump(), JSON_CONTENT_TYPE);
	json resp = Utils::ParseResponse(result);
	
	MIRAI_PARSE_GUARD_BEGIN(resp);
	return resp.at("data").get<GroupAnnouncement>();
	MIRAI_PARSE_GUARD_END(resp);
}

void HttpWsAdaptor::AnnoDelete(const string& SessionKey, GID_t target, const string& fid)
{
	json body = {{"sessionKey", SessionKey}, {"id", target}, {"fid", fid}};
	auto result = this->GetClient_().Post("/anno/delete", body.dump(), JSON_CONTENT_TYPE);
	(void)Utils::ParseResponse(result);
}


void HttpWsAdaptor::RespNewFriendRequestEvent(const string& SessionKey, int64_t EventId, QQ_t FromId, GID_t GroupId,
                                               int operate, const string& message)
{
	json body = {{"sessionKey", SessionKey}, {"eventId", EventId}, {"fromId", FromId},
	             {"groupId", GroupId},       {"operate", operate}, {"message", message}};
	auto result = this->GetClient_().Post("/resp/newFriendRequestEvent", body.dump(), JSON_CONTENT_TYPE);
	(void)Utils::ParseResponse(result);
}

void HttpWsAdaptor::RespMemberJoinRequestEvent(const string& SessionKey, int64_t EventId, QQ_t FromId, GID_t GroupId,
                                                int operate, const string& message)
{
	json body = {{"sessionKey", SessionKey}, {"eventId", EventId}, {"fromId", FromId},
	             {"groupId", GroupId},       {"operate", operate}, {"message", message}};
	auto result = this->GetClient_().Post("/resp/memberJoinRequestEvent", body.dump(), JSON_CONTENT_TYPE);
	(void)Utils::ParseResponse(result);
}

void HttpWsAdaptor::RespBotInvitedJoinGroupRequestEvent(const string& SessionKey, int64_t EventId, QQ_t FromId,
                                                         GID_t GroupId, int operate, const string& message)
{
	json body = {{"sessionKey", SessionKey}, {"eventId", EventId}, {"fromId", FromId},
	             {"groupId", GroupId},       {"operate", operate}, {"message", message}};
	auto result = this->GetClient_().Post("/resp/botInvitedJoinGroupRequestEvent", body.dump(), JSON_CONTENT_TYPE);
	(void)Utils::ParseResponse(result);
}


void HttpWsAdaptor::CmdExecute(const string& SessionKey, const MessageChain& command)
{
	json body = {{"sessionKey", SessionKey}, {"command", command}};
	auto result = this->GetClient_().Post("/cmd/execute", body.dump(), JSON_CONTENT_TYPE);
	(void)Utils::ParseResponse(result);
}

void HttpWsAdaptor::CmdRegister(const string& SessionKey, const string& name, const std::vector<string>& alias,
                                 const string& usage, const string& description)
{
	json body = {{"sessionKey", SessionKey}, {"name", name}, {"usage", usage}, {"description", description}};
	if (!alias.empty()) body["alias"] = alias;
	auto result = this->GetClient_().Post("/cmd/register", body.dump(), JSON_CONTENT_TYPE);
	(void)Utils::ParseResponse(result);
}

string HttpWsAdaptor::CallAPI(const string& path, const string& method, const string& data)
{
	if (method == "GET")
	{
		auto result = this->GetClient_().Get(path);
		if (!result || result.error() != httplib::Error::Success)
			throw NetworkException(-1, httplib::to_string(result.error()));
		if (result->status < 200 || result->status > 299) 
			throw NetworkException(result->status, result->body);
		return result->body;
	}
	else if(method == "POST")
	{
		auto result = this->GetClient_().Post(path, data, JSON_CONTENT_TYPE);
		if (!result || result.error() != httplib::Error::Success)
			throw NetworkException(-1, httplib::to_string(result.error()));
		if (result->status < 200 || result->status > 299) 
			throw NetworkException(result->status, result->body);
		return result->body;
	}
	else
		throw NotImplementedError(("Method not implemented in HttpWsAdaptor::CallAPI: " + method).c_str());
}

}


}