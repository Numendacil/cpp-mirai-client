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

#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

void UID_t::Serializable::from_json(const json& j, UID_t& p)
{
	MIRAI_PARSE_GUARD_BEGIN(j);

	p = UID_t(j.get<int64_t>());

	MIRAI_PARSE_GUARD_END(j);
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
#define ENUM_STR_NAMESPACE(_enum_) _enum_##Namespace

#define BEGIN_DECLARE_ENUM_STR(_enum_)                                                                                 \
	namespace ENUM_STR_NAMESPACE(_enum_)                                                                               \
	{                                                                                                                  \
	template<typename T> using TypesList = std::array<T, static_cast<std::size_t>(_enum_::ENUM_END)>;                  \
	namespace                                                                                                          \
	{                                                                                                                  \
	constexpr void DeclareName(TypesList<std::string_view>& list, _enum_ type, std::string_view name)                  \
	{                                                                                                                  \
		list.at(static_cast<std::size_t>(type)) = name;                                                                \
	}                                                                                                                  \
                                                                                                                       \
	constexpr TypesList<std::string_view> GetNames()                                                                   \
	{                                                                                                                  \
		TypesList<std::string_view> names;                                                                             

#define END_DECLARE_ENUM_STR(_enum_)                                                                                                                         \
		return names;                                                                                                  \
	}                                                                                                                  \
                                                                                                                       \
	constexpr auto names = GetNames();                                                                                 \
	}                                                                                                                  \
	constexpr std::string_view ENUM_TO_STR_FUNCNAME(_enum_)(const _enum_& m)                                           \
	{                                                                                                                  \
		auto i = static_cast<std::size_t>(m);                                                                          \
		if (i < names.size()) return names.at(i);                                    \
		else                                                                                                           \
			return "";                                                                                                 \
	}                                                                                                                  \
	constexpr _enum_ STR_TO_ENUM_FUNCNAME(_enum_)(std::string_view s)                                                  \
	{                                                                                                                  \
		for (std::size_t i = 0; i < names.size(); i++)                                                \
			if (names.at(i) == s) return static_cast<_enum_>(i);                                      \
		return _enum_::ENUM_END;                                                                                       \
	}                                                                                                                  \
	}

#define DECLARE_ENUM_STR(_value_, _name_) DeclareName(names, _value_, _name_);

#define ENUM_TO_STR(_enum_, _input_) ENUM_STR_NAMESPACE(_enum_)::ENUM_TO_STR_FUNCNAME(_enum_)(_input_)
#define STR_TO_ENUM(_enum_, _input_) ENUM_STR_NAMESPACE(_enum_)::STR_TO_ENUM_FUNCNAME(_enum_)(_input_)


BEGIN_DECLARE_ENUM_STR(SEX)

DECLARE_ENUM_STR(SEX::MALE, "MALE");
DECLARE_ENUM_STR(SEX::FEMALE, "FEMALE");
DECLARE_ENUM_STR(SEX::UNKNOWN, "UNKNOWN");

END_DECLARE_ENUM_STR(SEX)

void from_json(const json& j, SEX& p)
{
	p = STR_TO_ENUM(SEX, j.get<std::string>());
}
void to_json(json& j, const SEX& p)
{
	j = ENUM_TO_STR(SEX, p);
}


BEGIN_DECLARE_ENUM_STR(PERMISSION)

DECLARE_ENUM_STR(PERMISSION::OWNER, "OWNER");
DECLARE_ENUM_STR(PERMISSION::ADMINISTRATOR, "ADMINISTRATOR");
DECLARE_ENUM_STR(PERMISSION::MEMBER, "MEMBER");

END_DECLARE_ENUM_STR(PERMISSION)

void from_json(const json& j, PERMISSION& p)
{
	p = STR_TO_ENUM(PERMISSION, j.get<std::string>());
}
void to_json(json& j, const PERMISSION& p)
{
	j = ENUM_TO_STR(PERMISSION, p);
}


BEGIN_DECLARE_ENUM_STR(NudgeType)

DECLARE_ENUM_STR(NudgeType::FRIEND, "Friend");
DECLARE_ENUM_STR(NudgeType::GROUP, "Group");
DECLARE_ENUM_STR(NudgeType::STRANGER, "Stranger");

END_DECLARE_ENUM_STR(NudgeType)

void from_json(const json& j, NudgeType& p)
{
	p = STR_TO_ENUM(NudgeType, j.get<std::string>());
}
void to_json(json& j, const NudgeType& p)
{
	j = ENUM_TO_STR(NudgeType, p);
}


BEGIN_DECLARE_ENUM_STR(MusicShareType)

DECLARE_ENUM_STR(MusicShareType::NETEASECLOUDMUSIC, "NeteaseCloudMusic");
DECLARE_ENUM_STR(MusicShareType::QQMUSIC, "QQMusic");
DECLARE_ENUM_STR(MusicShareType::MIGUMUSIC, "MiguMusic");
DECLARE_ENUM_STR(MusicShareType::KUGOUMUSIC, "KugouMusic");
DECLARE_ENUM_STR(MusicShareType::KUWOMUSIC, "KuwoMusic");

END_DECLARE_ENUM_STR(MusicShareType)

void from_json(const json& j, MusicShareType& p)
{
	p = STR_TO_ENUM(MusicShareType, j.get<std::string>());
}
void to_json(json& j, const MusicShareType& p)
{
	j = ENUM_TO_STR(MusicShareType, p);
}


BEGIN_DECLARE_ENUM_STR(PokeType)

DECLARE_ENUM_STR(PokeType::CHUOYICHUO, "ChuoYiChuo");
DECLARE_ENUM_STR(PokeType::BIXIN, "BiXin");
DECLARE_ENUM_STR(PokeType::DIANZAN, "DianZan");
DECLARE_ENUM_STR(PokeType::XINSUI, "XinSui");
DECLARE_ENUM_STR(PokeType::LIULIULIU, "LiuLiuLiu");
DECLARE_ENUM_STR(PokeType::FANGDAZHAO, "FangDaZhao");
DECLARE_ENUM_STR(PokeType::GOUYIN, "GouYin");
DECLARE_ENUM_STR(PokeType::BAOBEIQIU, "BaoBeiQiu");
DECLARE_ENUM_STR(PokeType::ROSE, "Rose");
DECLARE_ENUM_STR(PokeType::ZHAOHUANSHU, "ZhaoHuanShu");
DECLARE_ENUM_STR(PokeType::RANGNIPI, "RangNiPi");
DECLARE_ENUM_STR(PokeType::JIEYIN, "JieYin");
DECLARE_ENUM_STR(PokeType::SHOULEI, "ShouLei");
DECLARE_ENUM_STR(PokeType::ZHUAYIXIA, "ZhuaYiXia");
DECLARE_ENUM_STR(PokeType::SUIPING, "SuiPing");
DECLARE_ENUM_STR(PokeType::QIAOMEN, "QiaoMen");

END_DECLARE_ENUM_STR(PokeType)

void from_json(const json& j, PokeType& p)
{
	p = STR_TO_ENUM(PokeType, j.get<std::string>());
}
void to_json(json& j, const PokeType& p)
{
	j = ENUM_TO_STR(PokeType, p);
}


BEGIN_DECLARE_ENUM_STR(HonorChangeType)

DECLARE_ENUM_STR(HonorChangeType::ACHIEVE, "achieve");
DECLARE_ENUM_STR(HonorChangeType::LOSE, "lose");

END_DECLARE_ENUM_STR(HonorChangeType)

void from_json(const json& j, HonorChangeType& p)
{
	p = STR_TO_ENUM(HonorChangeType, j.get<std::string>());
}
void to_json(json& j, const HonorChangeType& p)
{
	j = ENUM_TO_STR(HonorChangeType, p);
}


#undef ENUM_TO_STR_FUNCNAME
#undef STR_TO_ENUM_FUNCNAME
#undef ENUM_STR_NAMESPACE
#undef BEGIN_DECLARE_ENUM_STR
#undef END_DECLARE_ENUM_STR
#undef DECLARE_ENUM_STR
#undef ENUM_TO_STR
#undef STR_TO_ENUM


// *********************************************
// ********** ENUM DEFINITIONS END *************
// *********************************************


void User::Serializable::from_json(const json& j, User& p)
{
	MIRAI_PARSE_GUARD_BEGIN(j);

	j.at("id").get_to(p.id);
	j.at("id").get_to(p.id);
	j.at("nickname").get_to(p.nickname);
	j.at("remark").get_to(p.remark);

	MIRAI_PARSE_GUARD_END(j);
}

void User::Serializable::to_json(json& j, const User& p)
{
	j["id"] = p.id;
	j["nickname"] = p.nickname;
	j["remark"] = p.remark;
}


void Group::Serializable::from_json(const json& j, Group& p)
{
	MIRAI_PARSE_GUARD_BEGIN(j);

	j.at("id").get_to(p.id);
	j.at("name").get_to(p.name);
	j.at("permission").get_to(p.permission);

	MIRAI_PARSE_GUARD_END(j);
}

void Group::Serializable::to_json(json& j, const Group& p)
{
	j["id"] = p.id;
	j["name"] = p.name;
	j["permission"] = p.permission;
}


void GroupMember::Serializable::from_json(const json& j, GroupMember& p)
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


void UserProfile::Serializable::from_json(const json& j, UserProfile& p)
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


void ClientDevice::Serializable::from_json(const json& j, ClientDevice& p)
{
	MIRAI_PARSE_GUARD_BEGIN(j);

	j.at("id").get_to(p.id);
	j.at("platform").get_to(p.platform);

	MIRAI_PARSE_GUARD_END(j);
}

void ClientDevice::Serializable::to_json(json& j, const ClientDevice& p)
{
	json data = json::object();
	j["id"] = p.id;
	j["platform"] = p.platform;
}

} // namespace Mirai