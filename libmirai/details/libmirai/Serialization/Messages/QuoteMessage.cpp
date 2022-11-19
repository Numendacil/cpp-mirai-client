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

#include <nlohmann/json.hpp>

#include <libmirai/Messages/QuoteMessage.hpp>
#include <libmirai/Serialization/Messages/MessageChain.hpp>
#include <libmirai/Serialization/Types/Types.hpp>


namespace Mirai
{

using json = nlohmann::json;

void QuoteMessage::Deserialize(const void* data)
{
	const auto& j = *static_cast<const json*>(data);

	assert(j.at("type").get<MessageTypes>() == this->GetType()); // NOLINT(*-array-to-pointer-decay)

	j.at("id").get_to(this->_QuoteId);
	j.at("groupId").get_to(this->_GroupId);
	j.at("senderId").get_to(this->_SenderId);
	j.at("targetId").get_to(this->_TargetId);
	j.at("origin").get_to(this->_origin);
}

void QuoteMessage::Serialize(void* data) const
{
	auto& j = *static_cast<json*>(data);
	// assert(this->isValid());	// NOLINT(*-array-to-pointer-decay)

	// json data = json::object();
	// j["type"] = this->GetType();
	// j["id"] = this->id;
	// j["name"] = this->name;
	// j["size"] = this->size;
	// return data;

	// Not allowed for sending
}

} // namespace Mirai
