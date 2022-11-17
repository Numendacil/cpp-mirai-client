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

#include "BasicTypes.hpp"

#include <array>
#include <cstdint>
#include <string>

#include <nlohmann/json.hpp>
#include "libmirai/Types/BasicTypes.hpp"

namespace Mirai
{

using json = nlohmann::json;

void UID_t::Serializable::from_json(const json& j, UID_t& p)
{
	p = UID_t(j.get<int64_t>());
}

void UID_t::Serializable::to_json(json& j, const UID_t& p)
{
	j = int64_t(p);
}



// *********************************************
// ************ ENUM DEFINITIONS ***************
// *********************************************

#define ENUM_TO_STR_FUNCNAME(_enum_) _##_enum_##_TO_STR_ 
#define STR_TO_ENUM_FUNCNAME(_enum_) _STR_TO_##_enum_##_
#define ENUM_STR_ARRAY(_enum_) _enum_##Str

#define DECLARE_ENUM_STR(_enum_, ...)	\
namespace	\
{	\
constexpr std::array<std::string_view, static_cast<size_t>(_enum_::ENUM_END)> ENUM_STR_ARRAY(_enum_) = {__VA_ARGS__};	\
constexpr std::string_view ENUM_TO_STR_FUNCNAME(_enum_)(const _enum_& m)	\
{	\
	auto i = static_cast<std::size_t>(m);	\
	if (i < ENUM_STR_ARRAY(_enum_).size()) return ENUM_STR_ARRAY(_enum_).at(i);	\
	else return "";	\
}	\
constexpr _enum_ STR_TO_ENUM_FUNCNAME(_enum_)(std::string_view s)	\
{	\
	for (std::size_t i = 0; i < ENUM_STR_ARRAY(_enum_).size(); i++)	\
		if (ENUM_STR_ARRAY(_enum_).at(i) == s) return static_cast<_enum_>(i);	\
	return _enum_::ENUM_END;	\
}	\
}

#define ENUM_TO_STR(_enum_, _input_) ENUM_TO_STR_FUNCNAME(_enum_)(_input_)
#define STR_TO_ENUM(_enum_, _input_) STR_TO_ENUM_FUNCNAME(_enum_)(_input_)



DECLARE_ENUM_STR(SEX, "MALE", "FEMALE", "UNKNOWN")

void from_json(const json& j, SEX& p)
{
	p = STR_TO_ENUM(SEX, j.get<std::string>());
}
void to_json(json& j, const SEX& p)
{
	j = ENUM_TO_STR(SEX, p);
}


DECLARE_ENUM_STR(PERMISSION, "OWNER", "ADMINISTRATOR", "MEMBER")

void from_json(const json& j, PERMISSION& p)
{
	p = STR_TO_ENUM(PERMISSION, j.get<std::string>());
}
void to_json(json& j, const PERMISSION& p)
{
	j = ENUM_TO_STR(PERMISSION, p);
}


DECLARE_ENUM_STR(MusicShareType, "NeteaseCloudMusic", "QQMusic", "MiguMusic", "KugouMusic", "KuwoMusic");

void from_json(const json& j, MusicShareType& p)
{
	p = STR_TO_ENUM(MusicShareType, j.get<std::string>());
}
void to_json(json& j, const MusicShareType& p)
{
	j = ENUM_TO_STR(MusicShareType, p);
}


DECLARE_ENUM_STR(PokeType,
	"ChuoYiChuo", "BiXin",
	"DianZan",  "XinSui", 
	"LiuLiuLiu", "FangDaZhao", 
	"GouYin",  "BaoBeiQiu",
	"Rose", "ZhaoHuanShu", 
	"RangNiPi", "JieYin", 
	"ShouLei", "ZhuaYiXia", 
	"SuiPing", "QiaoMen"
);

void from_json(const json& j, PokeType& p)
{
	p = STR_TO_ENUM(PokeType, j.get<std::string>());
}
void to_json(json& j, const PokeType& p)
{
	j = ENUM_TO_STR(PokeType, p);
}

// *********************************************
// ********** ENUM DEFINITIONS END *************
// *********************************************


void from_json(const json& j, User& p)
{
	j.at("id").get_to(p.id);
	j.at("id").get_to(p.id);
	j.at("nickname").get_to(p.nickname);
	j.at("remark").get_to(p.remark);
}

void User::Serializable::to_json(json& j, const User& p)
{
	j["id"] = p.id;
	j["nickname"] = p.nickname;
	j["remark"] = p.remark;
}


void from_json(const json& j, Group& p)
{
	j.at("id").get_to(p.id);
	j.at("name").get_to(p.name);
	j.at("permission").get_to(p.permission);
}

void Group::Serializable::to_json(json& j, const Group& p)
{
	j["id"] = p.id;
	j["name"] = p.name;
	j["permission"] = p.permission;
}


void from_json(const json& j, GroupMember& p)
{
	j.at("id").get_to(p.id);
	j.at("memberName").get_to(p.MemberName);
	j.at("permission").get_to(p.permission);
	j.at("specialTitle").get_to(p.SpecialTitle);
	j.at("joinTimestamp").get_to(p.JoinTimestamp);
	j.at("lastSpeakTimestamp").get_to(p.LastSpeakTimestamp);
	p.MuteTimeRemaining = std::chrono::seconds(j.at("muteTimeRemaining").get<int64_t>());
	j.at("group").get_to(p.group);
}

void GroupMember::Serializable::to_json(json& j, const GroupMember& p)
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


void from_json(const json& j, UserProfile& p)
{
	j.at("nickname").get_to(p.nickname);
	j.at("email").get_to(p.email);
	j.at("age").get_to(p.age);
	j.at("level").get_to(p.level);
	j.at("sign").get_to(p.sign);
	j.at("sex").get_to(p.sex);
}

void UserProfile::Serializable::to_json(json& j, const UserProfile& p)
{
	j["nickname"] = p.nickname;
	j["email"] = p.email;
	j["age"] = p.age;
	j["level"] = p.level;
	j["sign"] = p.sign;
	j["sex"] = p.sex;
}


void from_json(const json& j, ClientDevice& p)
{
	j.at("id").get_to(p.id);
	j.at("platform").get_to(p.platform);
}

void ClientDevice::Serializable::to_json(json& j, const ClientDevice& p)
{
	json data = json::object();
	j["id"] = p.id;
	j["platform"] = p.platform;
}

} // namespace Mirai