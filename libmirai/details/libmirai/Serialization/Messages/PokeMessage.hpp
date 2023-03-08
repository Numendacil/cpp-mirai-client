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

#ifndef MIRAI_SERIALIZATION_POKE_MESSAGE_HPP_
#define MIRAI_SERIALIZATION_POKE_MESSAGE_HPP_

#include <nlohmann/json.hpp>

#include <libmirai/Messages/PokeMessage.hpp>
#include <libmirai/Serialization/Types/Types.hpp>

namespace Mirai
{

struct PokeMessage::Serializable
{

	static void from_json(const nlohmann::json& j, PokeMessage& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<MessageTypes>() == PokeMessage::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("name").get_to(p.kind_);

		MIRAI_PARSE_GUARD_END(j);
	}

	static void to_json(nlohmann::json& j, const PokeMessage& p)
	{
		// assert(p.valid());	// NOLINT(*-array-to-pointer-decay)

		j["type"] = PokeMessage::GetType();
		j["name"] = p.kind_;
	}

};

} // namespace Mirai

#endif