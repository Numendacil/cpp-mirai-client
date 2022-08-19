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

#include "QuoteMessage.hpp"

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

bool QuoteMessage::isValid() const
{
	return true;
}

void QuoteMessage::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());

	this->_QuoteId = Utils::GetValue(data, "id", (MessageId_t)-1);
	this->_GroupId = Utils::GetValue(data, "groupId", GID_t{});
	this->_SenderId = Utils::GetValue(data, "senderId", QQ_t{});
	this->_TargetId = Utils::GetValue(data, "targetId", (int64_t)0);
	this->_origin = Utils::GetValue(data, "origin", MessageChain{});
}

json QuoteMessage::ToJson() const
{
	// assert(this->isValid());

	// json data = json::object();
	// data["type"] = this->GetType();
	// data["id"] = this->id;
	// data["name"] = this->name;
	// data["size"] = this->size;
	// return data;

	// Not allowed for sending
	return json::object();
}

} // namespace Mirai
