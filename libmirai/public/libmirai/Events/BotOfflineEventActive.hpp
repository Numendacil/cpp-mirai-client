#ifndef _MIRAI_BOT_OFFLINE_EVENT_ACTIVE_HPP_
#define _MIRAI_BOT_OFFLINE_EVENT_ACTIVE_HPP_

#include <string>

#include "BotEvent.hpp"

namespace Mirai
{

/**
 * @brief Bot主动下线事件
 * 
 */
class BotOfflineEventActive : public BotEvent
{
public:
	using BotEvent::BotEvent;
	static constexpr std::string_view _TYPE_ = "BotOfflineEventActive";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual BotOfflineEventActive* Clone() const override
	// {
	//	return new BotOfflineEventActive(*this);
	// }
};

} // namespace Mirai


#endif