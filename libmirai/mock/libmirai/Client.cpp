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

#include <any>
#include <chrono>
#include <condition_variable>
#include <exception>
#include <fstream>
#include <map>
#include <thread>
#include <memory>
#include <mutex>
#include <string>
#include <tuple>

#include <httplib.h>
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
#include <libmirai/Messages/MessageChain.hpp>
#include <libmirai/Types/Types.hpp>
#include <libmirai/Utils/Common.hpp>
#include <libmirai/Utils/ThreadPool.hpp>

#include <libmirai/Utils/Logger.hpp>

namespace Mirai
{

using json = nlohmann::json;

MiraiClient::MiraiClient()
{
	this->_logger = std::make_shared<NullLogger>();
}

MiraiClient::MiraiClient(SessionConfigs config) : _config(std::move(config)) 
{
	this->_logger = std::make_shared<NullLogger>();
}

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
		this->_ThreadPool = std::move(rhs._ThreadPool);
	}
	return *this;
}

MiraiClient::~MiraiClient() = default;

void MiraiClient::_OnOpen()
{
	std::map<std::string, std::string> headers{};
	LOG_DEBUG(
	this->_GetLogger(),
	"MessageClient connected with response header: " + [&]() -> std::string
	{
		std::string header;
		for (const auto& p : headers)
			header += "\n" + p.first + ": " + p.second;
		return header;
	}());

	this->_HandshakeInfo = {"/path", {headers.begin(), headers.end()}, "protocol"};
}

void MiraiClient::_OnError()
{
	LOG_DEBUG(
	this->_GetLogger(),
	"MessageClient failed to connect with error: " + [&]() -> std::string
	{
		std::string msg;
		msg += "\nHTTP Status: 500";
		msg += "\nReason: unknown";
		msg += "\nDecompressionError: False";
		return msg;
	}());

	auto callback = this->_GetErrorCallback();
	if (callback)
	{
		ClientConnectionErrorEvent event{0, 1000, 500, "unknown", false};
		callback(event);
	}
}

void MiraiClient::_OnClose()
{
	LOG_DEBUG(
	this->_GetLogger(),
	"MessageClient connection closed: " + [&]() -> std::string
	{
		std::string msg;
		msg += "\nCode: 0";
		msg += "\nReason: unknown";
		msg += "\nClosedByRemote: False";
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
		ClientConnectionClosedEvent event{0, "unknown", false};
		callback(event);
	}
}

void MiraiClient::_OnText(const std::string& message)
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
}

void MiraiClient::Connect()
{
	std::lock_guard<std::mutex> lk_c(this->_ConnectMtx);

	{
		std::lock_guard<std::mutex> lk(this->_mtx);
		if (this->_connected) return;
	}

	LOG_DEBUG(this->_GetLogger(), "Connecting to Mirai-Api-Http, initializing clients and threadpool");

	this->_SessionKey = "";
	this->_SessionKeySet = false;

	this->_ThreadPool = std::make_unique<Utils::ThreadPool>(this->_config.ThreadPoolSize);

	LOG_DEBUG(this->_GetLogger(), "Threadpool initialized");

	LOG_DEBUG(this->_GetLogger(), "Clients initialized");

	LOG_DEBUG(this->_GetLogger(),
	          "Calling connect from MessageClient: " + this->_config.WebsocketUrl
	              + "/all?verifyKey=" + this->_config.VerifyKey + "&qq=" + this->_config.BotQQ.to_string());

	std::thread th([this]
	{
		using namespace std::literals;
		std::this_thread::sleep_for(100ms);
		this->_OnOpen();
		std::this_thread::sleep_for(100ms);

		this->_OnText(R"(
		{
			"syncId": "",
			"data": 
			{
				"code": 0,
				"session": "Numendacil"
			}
		})");
	});

	{
		std::unique_lock<std::mutex> lk(this->_mtx);
		if (!this->_cv.wait_for(lk, std::chrono::seconds(10), [this]() -> bool { return this->_connected; }))
		{
			LOG_DEBUG(this->_GetLogger(), "Timeout waiting for session key, closing down");

			// failed to get sessionKey
			this->_OnClose();
			throw NetworkException(-2, "Failed to receive session key from server");
		}
	}

	th.join();
	LOG_DEBUG(this->_GetLogger(), "Successfully connected");
}

void MiraiClient::Disconnect()
{
	std::lock_guard<std::mutex> lk(this->_ConnectMtx);
	{
		std::lock_guard<std::mutex> lk(this->_mtx);
		if (!this->_connected) return;
	}

	this->_OnClose();

	this->_ThreadPool = nullptr;

	LOG_DEBUG(this->_GetLogger(), "Threadpool shutdown complete");

	LOG_DEBUG(this->_GetLogger(), "Clients shutdown complete");
}

bool MiraiClient::_ReadSessionKey(const json& data)
{
	if (!(data.contains("session") && data.at("session").is_string())) return false;

	std::string session = data.at("session").get<std::string>();

	LOG_DEBUG(this->_GetLogger(), "Session key obtained: " + session);

	// Check http connection
	LOG_DEBUG(this->_GetLogger(), "Checking HTTP connection, calling /sessionInfo with session key: " + session);

	json resp = {
		{"code", 0},
		{"msg", ""},
		{"data", {
			{"sessionKey", "Numendacil"},
			{"qq", {
				{"id", 1234567890},
				{"nickname", ""},
				{"remark", ""}
			}
			}
		}
		}
	};

	resp = resp["data"];

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

} // namespace Mirai