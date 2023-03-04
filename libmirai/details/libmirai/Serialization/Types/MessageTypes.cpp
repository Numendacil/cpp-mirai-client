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

#include "MessageTypes.hpp"

#include <nlohmann/json.hpp>

namespace Mirai
{

using json = nlohmann::json;

namespace
{

template <typename T>
using TypesList = std::array<T, static_cast<std::size_t>(MessageTypes::ENUM_END)>;

constexpr void DeclareName(TypesList<std::string_view>& list, MessageTypes type, std::string_view name)
{
	list.at(static_cast<std::size_t>(type)) = name;
}

constexpr TypesList<std::string_view> GetNames()
{
	TypesList<std::string_view> names;

	DeclareName(names, MessageTypes::APP, "App");
	DeclareName(names, MessageTypes::AT_ALL, "AtAll");
	DeclareName(names, MessageTypes::AT, "At");
	DeclareName(names, MessageTypes::AUDIO, "Voice");
	DeclareName(names, MessageTypes::DICE, "Dice");
	DeclareName(names, MessageTypes::FACE, "Face");
	DeclareName(names, MessageTypes::FILE, "File");
	DeclareName(names, MessageTypes::FLASH_IMAGE, "FlashImage");
	DeclareName(names, MessageTypes::FORWARD, "Forward");
	DeclareName(names, MessageTypes::IMAGE, "Image");
	DeclareName(names, MessageTypes::JSON, "Json");
	DeclareName(names, MessageTypes::MARKET_FACE, "MarketFace");
	DeclareName(names, MessageTypes::MIRAI_CODE, "MiraiCode");
	DeclareName(names, MessageTypes::MUSIC_SHARE, "MusicShare");
	DeclareName(names, MessageTypes::PLAIN, "Plain");
	DeclareName(names, MessageTypes::POKE, "Poke");
	DeclareName(names, MessageTypes::QUOTE, "Quote");
	DeclareName(names, MessageTypes::SOURCE, "Source");
	DeclareName(names, MessageTypes::XML, "Xml");

	return names;
}

constexpr auto names = GetNames();

constexpr std::string_view enum_to_str(const MessageTypes& m)
{
	auto i = static_cast<std::size_t>(m);
	if (i < names.size()) return names.at(i);
	else
		return "";
}

constexpr MessageTypes str_to_enum(std::string_view s)
{
	for (std::size_t i = 0; i < names.size(); i++)
		if (names.at(i) == s) return static_cast<MessageTypes>(i);

	return MessageTypes::ENUM_END;
}

} // namespace

void from_json(const json& j, MessageTypes& p)
{
	MIRAI_PARSE_GUARD_BEGIN(j);

	p = str_to_enum(j.get<std::string>());

	MIRAI_PARSE_GUARD_END(j);
}
void to_json(json& j, const MessageTypes& p)
{
	j = enum_to_str(p);
}

std::string to_string(MessageTypes type)
{
	return std::string{enum_to_str(type)};
}

} // namespace Mirai