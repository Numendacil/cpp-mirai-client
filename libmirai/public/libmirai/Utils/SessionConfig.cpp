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

#include "SessionConfig.hpp"

#include <fstream>

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
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
	this->ConnectionTimeout =
		std::chrono::milliseconds(Utils::GetValue(json_config, "ConnectionTimeout", this->ConnectionTimeout.count()));
	this->ReadTimeout =
		std::chrono::milliseconds(Utils::GetValue(json_config, "ReadTimeout", this->ReadTimeout.count()));
	this->WriteTimeout =
		std::chrono::milliseconds(Utils::GetValue(json_config, "WriteTimeout", this->WriteTimeout.count()));

	this->WebsocketUrl = Utils::GetValue(json_config, "WebsocketUrl", this->WebsocketUrl);
	this->HeartbeatInterval =
		std::chrono::seconds(Utils::GetValue(json_config, "HeartbeatInterval", this->HeartbeatInterval.count()));
	this->HandshakeTimeout =
		std::chrono::seconds(Utils::GetValue(json_config, "HandshakeTimeout", this->HandshakeTimeout.count()));
	this->EnablePong = Utils::GetValue(json_config, "EnablePong", this->EnablePong);
	this->EnableDeflate = Utils::GetValue(json_config, "EnableDeflate", this->EnableDeflate);
	this->AutoReconnect = Utils::GetValue(json_config, "AutoReconnect", this->AutoReconnect);
	this->MinRetryInterval =
		std::chrono::milliseconds(Utils::GetValue(json_config, "MinRetryInterval", this->MinRetryInterval.count()));
	this->MaxRetryInterval =
		std::chrono::milliseconds(Utils::GetValue(json_config, "MaxRetryInterval", this->MaxRetryInterval.count()));

	this->BotQQ = Utils::GetValue(json_config, "BotQQ", this->BotQQ);
	this->VerifyKey = Utils::GetValue(json_config, "VerifyKey", this->VerifyKey);
	this->ThreadPoolSize = Utils::GetValue(json_config, "ThreadPoolSize", this->ThreadPoolSize);
}

} // namespace Mirai