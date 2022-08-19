#ifndef _MIRAI_BOT_RELOGIN_EVENT_HPP_
#define _MIRAI_BOT_RELOGIN_EVENT_HPP_

#include <string>

#include "BotEvent.hpp"

namespace Mirai
{

/**
 * @brief Bot重新登录事件
 * 
 */
class BotReloginEvent : public BotEvent
{
public:
	using BotEvent::BotEvent;
	static constexpr std::string_view _TYPE_ = "BotReloginEvent";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual BotReloginEvent* Clone() const override
	// {
	//	return new BotReloginEvent(*this);
	// }
};

} // namespace Mirai


#endif