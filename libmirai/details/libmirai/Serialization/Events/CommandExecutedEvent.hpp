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

#ifndef _MIRAI_SERIALIZATION_COMMAND_EXECUTED_EVENT_HPP_
#define _MIRAI_SERIALIZATION_COMMAND_EXECUTED_EVENT_HPP_

#include <nlohmann/json.hpp>

#include <libmirai/Events/CommandExecutedEvent.hpp>
#include <libmirai/Serialization/Messages/MessageChain.hpp>
#include <libmirai/Serialization/Types/Types.hpp>
#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

struct CommandExecutedEvent::Serializable
{

	static void from_json(const nlohmann::json& j, CommandExecutedEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == CommandExecutedEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("name").get_to(p._name);
		Utils::GetOptional(j, "friend", p._friend);
		Utils::GetOptional(j, "member", p._member);
		j.at("args").get_to(p._args);

		MIRAI_PARSE_GUARD_END(j);
	}

};

} // namespace Mirai

#endif