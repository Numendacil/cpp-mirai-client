#ifndef _MIRAI_SESSION_CONFIG_HPP_
#define _MIRAI_SESSION_CONFIG_HPP_

#include <ctime>
#include <string>
#include <chrono>

#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>

namespace Mirai
{

struct SessionConfigs
{
	// Http client settings
	std::string HttpUrl = "http://localhost:8080";
	std::chrono::milliseconds ConnectionTimeout = std::chrono::seconds(30);
	std::chrono::milliseconds ReadTimeout = std::chrono::seconds(5);
	std::chrono::milliseconds WriteTimeout = std::chrono::seconds(5);

	// Websocket client settings
	std::string WebsocketUrl = "ws://localhost:8080";
	std::chrono::seconds HeartbeatInterval = std::chrono::seconds(60);
	std::chrono::seconds HandshakeTimeout = std::chrono::seconds(60);
	bool EnablePong = true;
	bool EnableDeflate =
	#ifdef IXWEBSOCKET_USE_ZLIB
		true
	#else
		false
	#endif
	;
	bool AutoReconnect = true;
	std::chrono::milliseconds MinRetryInterval = std::chrono::seconds(1);
	std::chrono::milliseconds MaxRetryInterval = std::chrono::seconds(30);

	// Bot settings
	QQ_t BotQQ = 12345_qq;
	std::string VerifyKey = "";

	std::size_t ThreadPoolSize = 12;

	void FromFile(const std::string& path);
	void FromJson(const nlohmann::json& json_config);
};

}

#endif