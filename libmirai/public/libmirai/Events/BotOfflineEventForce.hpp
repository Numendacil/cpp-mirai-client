#ifndef _MIRAI_BOT_OFFLINE_EVENT_FORCE_HPP_
#define _MIRAI_BOT_OFFLINE_EVENT_FORCE_HPP_

#include <string>

#include "BotEvent.hpp"

namespace Mirai
{

/**
 * @brief Bot强制下线事件
 * 
 * 通常是因为在相同平台的其它设备登录了Bot被挤下线
 */
class BotOfflineEventForce : public BotEvent
{
public:
	using BotEvent::BotEvent;
	static constexpr std::string_view _TYPE_ = "BotOfflineEventForce";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual BotOfflineEventForce* Clone() const override
	// {
	//	return new BotOfflineEventForce(*this);
	// }
};

} // namespace Mirai


#endif