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

#ifndef _MIRAI_SERIALIZATION_BOT_EVENTS_HPP_
#define _MIRAI_SERIALIZATION_BOT_EVENTS_HPP_

#include <nlohmann/json.hpp>

#include <libmirai/Events/BotEvents.hpp>
#include <libmirai/Serialization/Types/Types.hpp>
#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

template <class Event>
struct BotAccountEvent<Event>::Serializable
{

	static void from_json(const nlohmann::json& j, BotAccountEvent<Event>& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == Event::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("qq").get_to(p._qq);

		MIRAI_PARSE_GUARD_END(j);
	}

};



struct BotGroupPermissionChangeEvent::Serializable
{

	static void from_json(const nlohmann::json& j, BotGroupPermissionChangeEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == BotGroupPermissionChangeEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("group").get_to(p._group);
		j.at("origin").get_to(p._origin);
		j.at("current").get_to(p._current);

		MIRAI_PARSE_GUARD_END(j);
	}

};



struct BotInvitedJoinGroupRequestEvent::Serializable
{

	static void from_json(const nlohmann::json& j, BotInvitedJoinGroupRequestEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == BotInvitedJoinGroupRequestEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("eventId").get_to(p._EventId);
		j.at("fromId").get_to(p._FromId);
		j.at("groupId").get_to(p._GroupId);
		j.at("groupName").get_to(p._GroupName);
		j.at("nick").get_to(p._nickname);
		j.at("message").get_to(p._message);

		MIRAI_PARSE_GUARD_END(j);
	}

};



struct BotJoinGroupEvent::Serializable
{

	static void from_json(const nlohmann::json& j, BotJoinGroupEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == BotJoinGroupEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("group").get_to(p._group);
		Utils::GetOptional(j, "invitor", p._inviter);

		MIRAI_PARSE_GUARD_END(j);
	}

};



template <class Event>
struct BotLeaveEvent<Event>::Serializable
{

	static void from_json(const nlohmann::json& j, BotLeaveEvent<Event>& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == Event::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("group").get_to(p._group);

		MIRAI_PARSE_GUARD_END(j);
	}

};

struct BotLeaveEventDisband::Serializable
{
	static void from_json(const nlohmann::json& j, BotLeaveEventDisband& p)
	{
		BotLeaveEvent<BotLeaveEventDisband>::Serializable::from_json(j, p);

		MIRAI_PARSE_GUARD_BEGIN(j);

		j.at("operator").get_to(p._operator);

		MIRAI_PARSE_GUARD_END(j);
	}
};

struct BotLeaveEventKick::Serializable
{
	static void from_json(const nlohmann::json& j, BotLeaveEventKick& p)
	{
		BotLeaveEvent<BotLeaveEventKick>::Serializable::from_json(j, p);

		MIRAI_PARSE_GUARD_BEGIN(j);

		Utils::GetOptional(j, "operator", p._operator);

		MIRAI_PARSE_GUARD_END(j);
	}
};



struct BotMuteEvent::Serializable
{

	static void from_json(const nlohmann::json& j, BotMuteEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == BotMuteEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("operator").get_to(p._operator);
		j.at("durationSeconds").get_to(p._duration);

		MIRAI_PARSE_GUARD_END(j);
	}

};



struct BotUnmuteEvent::Serializable
{

	static void from_json(const nlohmann::json& j, BotUnmuteEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == BotUnmuteEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("operator").get_to(p._operator);

		MIRAI_PARSE_GUARD_END(j);
	}

};

} // namespace Mirai

#endif