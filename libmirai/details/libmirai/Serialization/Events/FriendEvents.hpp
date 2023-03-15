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

#ifndef MIRAI_SERIALIZATION_FRIEND_EVENTS_HPP_
#define MIRAI_SERIALIZATION_FRIEND_EVENTS_HPP_

#include <nlohmann/json.hpp>

#include <libmirai/Events/FriendEvents.hpp>
#include <libmirai/Serialization/Messages/MessageChain.hpp>
#include <libmirai/Serialization/Types/Types.hpp>

namespace Mirai
{

struct FriendInputStatusChangedEvent::Serializable
{

	static void from_json(const nlohmann::json& j, FriendInputStatusChangedEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == FriendInputStatusChangedEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("friend").get_to(p.friend_);
		j.at("inputting").get_to(p.inputting_);
		
		MIRAI_PARSE_GUARD_END(j);
	}

};



struct FriendMessageEvent::Serializable
{

	static void from_json(const nlohmann::json& j, FriendMessageEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == FriendMessageEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("sender").get_to(p.sender_);
		j.at("messageChain").get_to(p.message_);

		MIRAI_PARSE_GUARD_END(j);
	}

	static void from_json(nlohmann::json&& j, FriendMessageEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == FriendMessageEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("sender").get_to(p.sender_);
		::Mirai::from_json(std::move(j.at("messageChain")), p.message_);

		MIRAI_PARSE_GUARD_END(j);
	}

};



struct FriendNickChangedEvent::Serializable
{

	static void from_json(const nlohmann::json& j, FriendNickChangedEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == FriendNickChangedEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("friend").get_to(p.friend_);
		j.at("from").get_to(p.from_);
		j.at("to").get_to(p.to_);

		MIRAI_PARSE_GUARD_END(j);
	}

};



struct FriendRecallEvent::Serializable
{

	static void from_json(const nlohmann::json& j, FriendRecallEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == FriendRecallEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("authorId").get_to(p.AuthorId_);
		j.at("messageId").get_to(p.MessageId_);
		j.at("time").get_to(p.time_);
		j.at("operator").get_to(p.operator_);

		MIRAI_PARSE_GUARD_END(j);
	}

};



struct FriendSyncMessageEvent::Serializable
{

	static void from_json(const nlohmann::json& j, FriendSyncMessageEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == FriendSyncMessageEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("subject").get_to(p.subject_);
		j.at("messageChain").get_to(p.message_);

		MIRAI_PARSE_GUARD_END(j);
	}

	static void from_json(nlohmann::json& j, FriendSyncMessageEvent& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<EventTypes>() == FriendSyncMessageEvent::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("subject").get_to(p.subject_);
		::Mirai::from_json(std::move(j.at("messageChain")), p.message_);

		MIRAI_PARSE_GUARD_END(j);
	}

};

} // namespace Mirai

#endif