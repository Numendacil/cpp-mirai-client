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

#ifndef MIRAI_SERIALIZATION_GROUP_EVENTS_HPP_
#define MIRAI_SERIALIZATION_GROUP_EVENTS_HPP_

#include <nlohmann/json.hpp>

#include <libmirai/Events/GroupEvents.hpp>
#include <libmirai/Serialization/Messages/MessageChain.hpp>
#include <libmirai/Serialization/Types/Types.hpp>
#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

struct GroupAllowAnonymousChatEvent::Serializable
{

	static void from_json(const nlohmann::json& j, GroupAllowAnonymousChatEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == GroupAllowAnonymousChatEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("origin").get_to(p.origin_);
		j.at("current").get_to(p.current_);
		j.at("group").get_to(p.group_);
		Utils::GetOptional(j, "operator", p.operator_);

		MIRAI_PARSE_GUARD_END(j);
	}

};



struct GroupAllowConfessTalkEvent::Serializable
{

	static void from_json(const nlohmann::json& j, GroupAllowConfessTalkEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == GroupAllowConfessTalkEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("origin").get_to(p.origin_);
		j.at("current").get_to(p.current_);
		j.at("isByBot").get_to(p.ByBot_);
		j.at("group").get_to(p.group_);

		MIRAI_PARSE_GUARD_END(j);
	}

};



struct GroupAllowMemberInviteEvent::Serializable
{

	static void from_json(const nlohmann::json& j, GroupAllowMemberInviteEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == GroupAllowMemberInviteEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("origin").get_to(p.origin_);
		j.at("current").get_to(p.current_);
		j.at("group").get_to(p.group_);
		Utils::GetOptional(j, "operator", p.operator_);

		MIRAI_PARSE_GUARD_END(j);
	}

};



struct GroupEntranceAnnouncementChangeEvent::Serializable
{

	static void from_json(const nlohmann::json& j, GroupEntranceAnnouncementChangeEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == GroupEntranceAnnouncementChangeEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("origin").get_to(p.origin_);
		j.at("current").get_to(p.current_);
		j.at("group").get_to(p.group_);
		Utils::GetOptional(j, "operator", p.operator_);

		MIRAI_PARSE_GUARD_END(j);
	}

};



struct GroupNameChangeEvent::Serializable
{

	static void from_json(const nlohmann::json& j, GroupNameChangeEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == GroupNameChangeEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("origin").get_to(p.origin_);
		j.at("current").get_to(p.current_);
		j.at("group").get_to(p.group_);
		Utils::GetOptional(j, "operator", p.operator_);

		MIRAI_PARSE_GUARD_END(j);
	}

};



struct GroupMessageEvent::Serializable
{

	static void from_json(const nlohmann::json& j, GroupMessageEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == GroupMessageEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("sender").get_to(p.sender_);
		j.at("messageChain").get_to(p.message_);
		
		MIRAI_PARSE_GUARD_END(j);
	}

	static void from_json(nlohmann::json&& j, GroupMessageEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == GroupMessageEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("sender").get_to(p.sender_);
		::Mirai::from_json(std::move(j.at("messageChain")), p.message_);
		
		MIRAI_PARSE_GUARD_END(j);
	}

};



struct GroupMuteAllEvent::Serializable
{

	static void from_json(const nlohmann::json& j, GroupMuteAllEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == GroupMuteAllEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("origin").get_to(p.origin_);
		j.at("current").get_to(p.current_);
		j.at("group").get_to(p.group_);
		Utils::GetOptional(j, "operator", p.operator_);

		MIRAI_PARSE_GUARD_END(j);
	}

};



struct GroupRecallEvent::Serializable
{

	static void from_json(const nlohmann::json& j, GroupRecallEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == GroupRecallEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("authorId").get_to(p.AuthorId_);
		j.at("messageId").get_to(p.MessageId_);
		j.at("time").get_to(p.time_);
		j.at("group").get_to(p.group_);
		Utils::GetOptional(j, "operator", p.operator_);

		MIRAI_PARSE_GUARD_END(j);
	}

};



struct GroupSyncMessageEvent::Serializable
{

	static void from_json(const nlohmann::json& j, GroupSyncMessageEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == GroupSyncMessageEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("subject").get_to(p.subject_);
		j.at("messageChain").get_to(p.message_);

		MIRAI_PARSE_GUARD_END(j);
	}

	static void from_json(nlohmann::json&& j, GroupSyncMessageEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == GroupSyncMessageEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("subject").get_to(p.subject_);
		::Mirai::from_json(std::move(j.at("messageChain")), p.message_);

		MIRAI_PARSE_GUARD_END(j);
	}

};

} // namespace Mirai

#endif