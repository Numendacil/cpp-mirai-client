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

#ifndef MIRAI_SERIALIZATION_JSON_MESSAGE_HPP_
#define MIRAI_SERIALIZATION_JSON_MESSAGE_HPP_

#include <nlohmann/json.hpp>

#include <libmirai/Messages/JsonMessage.hpp>
#include <libmirai/Serialization/Types/Types.hpp>

namespace Mirai
{

struct JsonMessage::Serializable
{

	static void from_json(const nlohmann::json& j, JsonMessage& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<MessageTypes>() == JsonMessage::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("json").get_to(p.content_);

		MIRAI_PARSE_GUARD_END(j);
	}

	static void to_json(nlohmann::json& j, const JsonMessage& p)
	{
		// assert(p.valid());	// NOLINT(*-array-to-pointer-decay)

		j["type"] = JsonMessage::GetType();
		j["json"] = p.content_;
	}

};

} // namespace Mirai

#endif