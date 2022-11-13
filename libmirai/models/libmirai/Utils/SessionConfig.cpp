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

#include <libmirai/Serialization/Types/Types.hpp>
#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

namespace
{

using json = nlohmann::json;

void ConfigFromJson(SessionConfigs& config, const nlohmann::json& json_config)
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
	config.ThreadPoolSize = Utils::GetValue(json_config, "ThreadPoolSize", config.ThreadPoolSize);
}

} // namespace

void SessionConfigs::FromJsonFile(const std::string& path)
{
	std::ifstream file(path);
	json content = json::parse(file);
	ConfigFromJson(*this, content);
}


} // namespace Mirai