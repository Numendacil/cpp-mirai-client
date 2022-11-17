#ifndef _MIRAI_EVENT_TYPES_HPP_
#define _MIRAI_EVENT_TYPES_HPP_

#include <array>
#include <string>

namespace Mirai
{

enum class EventTypes
{
	BOT_GROUP = 0,

	// Used for counting
	ENUM_END
};

/**
 * @brief 用于类型之间转换的辅助模板
 * 
 */
template <EventTypes>
struct GetType;

template <EventTypes type>
using GetType_t = typename GetType<type>::type;

}

#endif