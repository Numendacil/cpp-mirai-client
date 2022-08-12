#include <fstream>
#include <nlohmann/json.hpp>
#include <libmirai/Utils/Common.hpp>

#include "SessionConfig.hpp"
namespace Mirai
{

using json = nlohmann::json;

void SessionConfigs::FromFile(const std::string& path)
{
	std::ifstream file(path);
	json content = json::parse(file);
	this->FromJson(content);
}

void SessionConfigs::FromJson(const nlohmann::json& json_config)
{
	this->HttpUrl = Utils::GetValue(json_config, "HttpUrl", this->HttpUrl);
	this->ConnectionTimeout = std::chrono::milliseconds(Utils::GetValue(json_config, "ConnectionTimeout", this->ConnectionTimeout.count()));
	this->ReadTimeout = std::chrono::milliseconds(Utils::GetValue(json_config, "ReadTimeout", this->ReadTimeout.count()));
	this->WriteTimeout = std::chrono::milliseconds(Utils::GetValue(json_config, "WriteTimeout", this->WriteTimeout.count()));

	this->WebsocketUrl = Utils::GetValue(json_config, "WebsocketUrl", this->WebsocketUrl);
	this->HeartbeatInterval = std::chrono::seconds(Utils::GetValue(json_config, "HeartbeatInterval", this->HeartbeatInterval.count()));
	this->HandshakeTimeout = std::chrono::seconds(Utils::GetValue(json_config, "HandshakeTimeout", this->HandshakeTimeout.count()));
	this->EnablePong = Utils::GetValue(json_config, "EnablePong", this->EnablePong);
	this->EnableDeflate = Utils::GetValue(json_config, "EnableDeflate", this->EnableDeflate);
	this->AutoReconnect = Utils::GetValue(json_config, "AutoReconnect", this->AutoReconnect);
	this->MinRetryInterval = std::chrono::milliseconds(Utils::GetValue(json_config, "MinRetryInterval", this->MinRetryInterval.count()));
	this->MaxRetryInterval = std::chrono::milliseconds(Utils::GetValue(json_config, "MaxRetryInterval", this->MaxRetryInterval.count()));

	this->BotQQ = Utils::GetValue(json_config, "BotQQ", this->BotQQ);
	this->VerifyKey = Utils::GetValue(json_config, "VerifyKey", this->VerifyKey);
	this->ThreadPoolSize = Utils::GetValue(json_config, "ThreadPoolSize", this->ThreadPoolSize);
}

}