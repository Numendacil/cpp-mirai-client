#ifndef _MIRAI_BOT_ONLINE_EVENT_HPP_
#define _MIRAI_BOT_ONLINE_EVENT_HPP_

#include <string>

#include "BotEvent.hpp"

namespace Mirai
{

/**
 * @brief Bot上线事件
 * 
 */
class BotOnlineEvent : public BotEvent
{
public:
	using BotEvent::BotEvent;
	static constexpr std::string_view _TYPE_ = "BotOnlineEvent";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual BotOnlineEvent* Clone() const override
	// {
	//	return new BotOnlineEvent(*this);
	// }
};

} // namespace Mirai


#endif