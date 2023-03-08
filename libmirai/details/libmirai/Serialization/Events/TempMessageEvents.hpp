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

#ifndef MIRAI_SERIALIZATION_TEMP_MESSAGE_EVENTS_HPP_
#define MIRAI_SERIALIZATION_TEMP_MESSAGE_EVENTS_HPP_

#include <nlohmann/json.hpp>

#include <libmirai/Events/TempMessageEvents.hpp>
#include <libmirai/Serialization/Messages/MessageChain.hpp>
#include <libmirai/Serialization/Types/Types.hpp>

namespace Mirai
{

struct TempMessageEvent::Serializable
{

	static void from_json(const nlohmann::json& j, TempMessageEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == TempMessageEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("sender").get_to(p.sender_);
		j.at("messageChain").get_to(p.message_);

		MIRAI_PARSE_GUARD_END(j);
	}

};



struct TempSyncMessageEvent::Serializable
{

	static void from_json(const nlohmann::json& j, TempSyncMessageEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == TempSyncMessageEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("subject").get_to(p.subject_);
		j.at("messageChain").get_to(p.message_);

		MIRAI_PARSE_GUARD_END(j);
	}

};

} // namespace Mirai

#endif