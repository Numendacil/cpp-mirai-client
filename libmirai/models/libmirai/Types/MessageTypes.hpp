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

#ifndef _MIRAI_MESSAGE_TYPES_HPP_
#define _MIRAI_MESSAGE_TYPES_HPP_

#include <array>
#include <string>

namespace Mirai
{

enum class MessageTypes
{
	APP = 0,
	AT_ALL,
	AT,
	AUDIO,
	DICE,
	FACE,
	FILE,
	FLASH_IMAGE,
	FORWARD,
	IMAGE,
	JSON,
	MARKET_FACE,
	MIRAI_CODE,
	MUSIC_SHARE,
	PLAIN,
	POKE,
	QUOTE,
	SOURCE,
	XML,

	// Used for counting
	ENUM_END
};

std::string to_string(MessageTypes type);

constexpr std::array<MessageTypes, static_cast<size_t>(MessageTypes::ENUM_END)> MessageTypesList = []()
{
	std::array<MessageTypes, static_cast<size_t>(MessageTypes::ENUM_END)> arr{};
	for (size_t i = 0; i < arr.size(); i++)
		arr.at(i) = static_cast<MessageTypes>(i);
	return arr;
}();

/**
 * @brief 用于类型之间转换的辅助模板
 * 
 */
template<MessageTypes> struct GetType;

template<MessageTypes type> using GetType_t = typename GetType<type>::type;

} // namespace Mirai

#endif