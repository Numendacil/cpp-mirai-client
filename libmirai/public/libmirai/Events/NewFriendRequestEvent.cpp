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

#include "NewFriendRequestEvent.hpp"

#include <cstdint>

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

void NewFriendRequestEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_EventId = Utils::GetValue(data, "eventId", (int64_t)0);
	this->_FromId = Utils::GetValue(data, "fromId", QQ_t{});
	this->_GroupId = Utils::GetValue(data, "groupId", GID_t{});
	this->_nickname = Utils::GetValue(data, "nick", "");
	this->_message = Utils::GetValue(data, "message", "");
}

} // namespace Mirai