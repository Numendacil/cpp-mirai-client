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

#ifndef _MIRAI_SERIALIZATION_DICE_MESSAGE_HPP_
#define _MIRAI_SERIALIZATION_DICE_MESSAGE_HPP_

#include <nlohmann/json.hpp>

#include <libmirai/Messages/DiceMessage.hpp>
#include <libmirai/Serialization/Types/Types.hpp>

namespace Mirai
{

struct DiceMessage::Serializable
{

	static void from_json(const nlohmann::json& j, DiceMessage& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<MessageTypes>() == DiceMessage::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("value").get_to(p._value);

		MIRAI_PARSE_GUARD_END(j)(j);
	}

	static void to_json(nlohmann::json& j, const DiceMessage& p)
	{
		// assert(p.valid());	// NOLINT(*-array-to-pointer-decay)

		j["type"] = DiceMessage::GetType();
		j["value"] = p._value;
	}

};

} // namespace Mirai

#endif