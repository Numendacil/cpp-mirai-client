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

constexpr std::array<MessageTypes, static_cast<size_t>(MessageTypes::ENUM_END)> MessageTypesList = 
[]() {
	std::array<MessageTypes, static_cast<size_t>(MessageTypes::ENUM_END)> arr{};
	for (size_t i = 0; i < arr.size(); i++)
		arr.at(i) = static_cast<MessageTypes>(i);
	return arr;
}();

/**
 * @brief 用于类型之间转换的辅助模板
 * 
 */
template <MessageTypes>
struct GetType;

template <MessageTypes type>
using GetType_t = typename GetType<type>::type;

}

#endif