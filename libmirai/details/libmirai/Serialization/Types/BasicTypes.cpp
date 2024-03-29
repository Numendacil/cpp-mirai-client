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

// *********************************************
// ************ ENUM DEFINITIONS ***************
// *********************************************

#define ENUM_TO_STR_FUNCNAME(enum_type) enum_type ## _ ## TO_STR_
#define STR_TO_ENUM_FUNCNAME(enum_type) STR_TO_ ## enum_type ## _
#define ENUM_STR_NAMESPACE(enum_type) enum_type ## Namespace

#define BEGIN_DECLARE_ENUM_STR(enum_type)                                                                                 \
	namespace ENUM_STR_NAMESPACE(enum_type)                                                                               \
	{                                                                                                                  \
	template<typename T> using TypesList = std::array<T, static_cast<std::size_t>(enum_type::ENUM_END)>;                  \
	namespace                                                                                                          \
	{                                                                                                                  \
	constexpr void DeclareName(TypesList<std::string_view>& list, enum_type type, std::string_view name)                  \
	{                                                                                                                  \
		list.at(static_cast<std::size_t>(type)) = name;                                                                \
	}                                                                                                                  \
                                                                                                                       \
	constexpr TypesList<std::string_view> GetNames()                                                                   \
	{                                                                                                                  \
		TypesList<std::string_view> names;

#define END_DECLARE_ENUM_STR(enum_type)                                                                                   \
	return names;                                                                                                      \
	}                                                                                                                  \
                                                                                                                       \
	constexpr auto names = GetNames();                                                                                 \
	}                                                                                                                  \
	constexpr std::string_view ENUM_TO_STR_FUNCNAME(enum_type)(const enum_type& m)                                           \
	{                                                                                                                  \
		auto i = static_cast<std::size_t>(m);                                                                          \
		if (i < names.size()) return names.at(i);                                                                      \
		else                                                                                                           \
			return "";                                                                                                 \
	}                                                                                                                  \
	constexpr enum_type STR_TO_ENUM_FUNCNAME(enum_type)(std::string_view s)                                                  \
	{                                                                                                                  \
		for (std::size_t i = 0; i < names.size(); i++)                                                                 \
			if (names.at(i) == s) return static_cast<enum_type>(i);                                                       \
		return enum_type::ENUM_END;                                                                                       \
	}                                                                                                                  \
	}

#define DECLARE_ENUM_STR(value_, name_) DeclareName(names, value_, name_);

#define ENUM_TO_STR(enum_type, input_) ENUM_STR_NAMESPACE(enum_type)::ENUM_TO_STR_FUNCNAME(enum_type)(input_)
#define STR_TO_ENUM(enum_type, input_) ENUM_STR_NAMESPACE(enum_type)::STR_TO_ENUM_FUNCNAME(enum_type)(input_)


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


} // namespace Mirai