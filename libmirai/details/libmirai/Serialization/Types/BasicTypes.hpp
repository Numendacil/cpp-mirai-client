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

#ifndef MIRAI_SERIALIZATION_TYPES_BASIC_TYPES_HPP_
#define MIRAI_SERIALIZATION_TYPES_BASIC_TYPES_HPP_

#include <nlohmann/json.hpp>

#include <libmirai/Serialization/Types/Serializable.hpp>
#include <libmirai/Types/BasicTypes.hpp>
#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

MIRAI_DECLARE_FROM_TO_JSON(SEX);
MIRAI_DECLARE_FROM_TO_JSON(PERMISSION);
MIRAI_DECLARE_FROM_TO_JSON(NudgeType);
MIRAI_DECLARE_FROM_TO_JSON(MusicShareType);
MIRAI_DECLARE_FROM_TO_JSON(PokeType);
MIRAI_DECLARE_FROM_TO_JSON(HonorChangeType);

struct UID_t::Serializable
{
	static void from_json(const nlohmann::json& j, UID_t& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		p = UID_t(j.get<int64_t>());

		MIRAI_PARSE_GUARD_END(j);
	}

	static void to_json(nlohmann::json& j, const UID_t& p) { j = int64_t(p); }
};


struct User::Serializable
{
	static void from_json(const nlohmann::json& j, User& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		j.at("id").get_to(p.id);
		j.at("nickname").get_to(p.nickname);
		j.at("remark").get_to(p.remark);

		MIRAI_PARSE_GUARD_END(j);
	}

	static void to_json(nlohmann::json& j, const User& p)
	{
		j["id"] = p.id;
		j["nickname"] = p.nickname;
		j["remark"] = p.remark;
	}
};


struct Group::Serializable
{
	static void from_json(const nlohmann::json& j, Group& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		j.at("id").get_to(p.id);
		j.at("name").get_to(p.name);
		j.at("permission").get_to(p.permission);

		MIRAI_PARSE_GUARD_END(j);
	}

	static void to_json(nlohmann::json& j, const Group& p)
	{
		j["id"] = p.id;
		j["name"] = p.name;
		j["permission"] = p.permission;
	}
};


struct GroupMember::Serializable
{
	static void from_json(const nlohmann::json& j, GroupMember& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		j.at("id").get_to(p.id);
		j.at("memberName").get_to(p.MemberName);
		j.at("permission").get_to(p.permission);
		p.SpecialTitle = Utils::GetValue(j, "specialTitle", "");                                         // Can be empty
		p.JoinTimestamp = Utils::GetValue(j, "joinTimestamp", (std::time_t)0);                           // Can be empty
		p.LastSpeakTimestamp = Utils::GetValue(j, "lastSpeakTimestamp", (std::time_t)0);                 // Can be empty
		p.MuteTimeRemaining = std::chrono::seconds(Utils::GetValue(j, "muteTimeRemaining", (int64_t)0)); // Can be empty
		j.at("group").get_to(p.group);

		MIRAI_PARSE_GUARD_END(j);
	}

	static void to_json(nlohmann::json& j, const GroupMember& p)
	{
		j["id"] = p.id;
		j["memberName"] = p.MemberName;
		j["permission"] = p.permission;
		j["specialTitle"] = p.SpecialTitle;
		j["joinTimestamp"] = p.JoinTimestamp;
		j["lastSpeakTimestamp"] = p.LastSpeakTimestamp;
		j["muteTimeRemaining"] = p.MuteTimeRemaining.count();
		j["group"] = p.group;
	}
};


struct UserProfile::Serializable
{
	static void from_json(const nlohmann::json& j, UserProfile& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		j.at("nickname").get_to(p.nickname);
		j.at("email").get_to(p.email);
		j.at("age").get_to(p.age);
		j.at("level").get_to(p.level);
		j.at("sign").get_to(p.sign);
		j.at("sex").get_to(p.sex);

		MIRAI_PARSE_GUARD_END(j);
	}

	static void to_json(nlohmann::json& j, const UserProfile& p)
	{
		j["nickname"] = p.nickname;
		j["email"] = p.email;
		j["age"] = p.age;
		j["level"] = p.level;
		j["sign"] = p.sign;
		j["sex"] = p.sex;
	}
};


struct ClientDevice::Serializable
{
	static void from_json(const nlohmann::json& j, ClientDevice& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		j.at("id").get_to(p.id);
		j.at("platform").get_to(p.platform);

		MIRAI_PARSE_GUARD_END(j);
	}

	static void to_json(nlohmann::json& j, const ClientDevice& p)
	{
		j["id"] = p.id;
		j["platform"] = p.platform;
	}
};

} // namespace Mirai

#endif