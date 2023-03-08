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

#ifndef MIRAI_SERIALIZATION_NEW_FRIEND_REQUEST_EVENT_HPP_
#define MIRAI_SERIALIZATION_NEW_FRIEND_REQUEST_EVENT_HPP_

#include <cstdint>

#include <nlohmann/json.hpp>

#include <libmirai/Events/NewFriendRequestEvent.hpp>
#include <libmirai/Serialization/Types/Types.hpp>

namespace Mirai
{

struct NewFriendRequestEvent::Serializable
{

	static void from_json(const nlohmann::json& j, NewFriendRequestEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == NewFriendRequestEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("eventId").get_to(p.EventId_);
		j.at("fromId").get_to(p.FromId_);
		j.at("groupId").get_to(p.GroupId_);
		j.at("nick").get_to(p.nickname_);
		j.at("message").get_to(p.message_);

		MIRAI_PARSE_GUARD_END(j);
	}

};

} // namespace Mirai

#endif