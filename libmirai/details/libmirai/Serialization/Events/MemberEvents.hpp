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

#ifndef MIRAI_SERIALIZATION_MEMBER_EVENTS_HPP_
#define MIRAI_SERIALIZATION_MEMBER_EVENTS_HPP_

#include <nlohmann/json.hpp>

#include <libmirai/Events/MemberEvents.hpp>
#include <libmirai/Serialization/Types/Types.hpp>
#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

struct MemberCardChangeEvent::Serializable
{

	static void from_json(const nlohmann::json& j, MemberCardChangeEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == MemberCardChangeEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("origin").get_to(p.origin_);
		j.at("current").get_to(p.current_);
		j.at("member").get_to(p.member_);

		MIRAI_PARSE_GUARD_END(j);
	}

};



struct MemberHonorChangeEvent::Serializable
{

	static void from_json(const nlohmann::json& j, MemberHonorChangeEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == MemberHonorChangeEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("action").get_to(p.action_);
		j.at("honor").get_to(p.honor_);
		j.at("member").get_to(p.member_);

		MIRAI_PARSE_GUARD_END(j);
	}

};



struct MemberPermissionChangeEvent::Serializable
{

	static void from_json(const nlohmann::json& j, MemberPermissionChangeEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == MemberPermissionChangeEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("origin").get_to(p.origin_);
		j.at("current").get_to(p.current_);
		j.at("member").get_to(p.member_);

		MIRAI_PARSE_GUARD_END(j);
	}

};



struct MemberSpecialTitleChangeEvent::Serializable
{

	static void from_json(const nlohmann::json& j, MemberSpecialTitleChangeEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == MemberSpecialTitleChangeEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("origin").get_to(p.origin_);
		j.at("current").get_to(p.current_);
		j.at("member").get_to(p.member_);

		MIRAI_PARSE_GUARD_END(j);
	}

};



struct MemberJoinEvent::Serializable
{

	static void from_json(const nlohmann::json& j, MemberJoinEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == MemberJoinEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("member").get_to(p.member_);
		Utils::GetOptional(j, "invitor", p.inviter_);

		MIRAI_PARSE_GUARD_END(j);
	}

};



struct MemberJoinRequestEvent::Serializable
{

	static void from_json(const nlohmann::json& j, MemberJoinRequestEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == MemberJoinRequestEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("eventId").get_to(p.EventId_);
		j.at("fromId").get_to(p.FromId_);
		j.at("groupId").get_to(p.GroupId_);
		j.at("groupName").get_to(p.GroupName_);
		j.at("nick").get_to(p.nickname_);
		j.at("message").get_to(p.message_);

		MIRAI_PARSE_GUARD_END(j);
	}

};




template <class Event>
struct MemberLeaveEvent<Event>::Serializable
{

	static void from_json(const nlohmann::json& j, MemberLeaveEvent<Event>& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == Event::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("member").get_to(p.member_);

		MIRAI_PARSE_GUARD_END(j);
	}

};

struct MemberLeaveEventKick::Serializable
{

	static void from_json(const nlohmann::json& j, MemberLeaveEventKick& p)
	{
		MemberLeaveEvent<MemberLeaveEventKick>::Serializable::from_json(j, p);

		MIRAI_PARSE_GUARD_BEGIN(j);

		Utils::GetOptional(j, "operator", p.operator_);

		MIRAI_PARSE_GUARD_END(j);
	}

};



struct MemberMuteEvent::Serializable
{

	static void from_json(const nlohmann::json& j, MemberMuteEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == MemberMuteEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("durationSeconds").get_to(p.duration_);
		j.at("member").get_to(p.member_);
		Utils::GetOptional(j, "operator", p.operator_);

		MIRAI_PARSE_GUARD_END(j);
	}

};



struct MemberUnmuteEvent::Serializable
{

	static void from_json(const nlohmann::json& j, MemberUnmuteEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == MemberUnmuteEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("member").get_to(p.member_);
		Utils::GetOptional(j, "operator", p.operator_);

		MIRAI_PARSE_GUARD_END(j);
	}

};

} // namespace Mirai

#endif