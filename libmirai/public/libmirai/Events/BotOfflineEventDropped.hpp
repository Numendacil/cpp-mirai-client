#ifndef _MIRAI_BOT_OFFLINE_EVENT_DROPPED_HPP_
#define _MIRAI_BOT_OFFLINE_EVENT_DROPPED_HPP_

#include <string>

#include "BotEvent.hpp"

namespace Mirai
{

/**
 * @brief Bot掉线事件
 * 
 * 网络原因或服务器原因导致的掉线
 */
class BotOfflineEventDropped : public BotEvent
{
public:
	using BotEvent::BotEvent;
	static constexpr std::string_view _TYPE_ = "BotOfflineEventDropped";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual BotOfflineEventDropped* Clone() const override
	// {
	//	return new BotOfflineEventDropped(*this);
	// }
};

} // namespace Mirai


#endif