#ifndef _MIRAI_BOT_OFFLINE_EVENT_DROPPED_HPP_
#define _MIRAI_BOT_OFFLINE_EVENT_DROPPED_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "BotEvent.hpp"

namespace Mirai
{

class BotOfflineEventDropped : public BotEvent
{
public:
	using BotEvent::BotEvent;
	static constexpr std::string_view _TYPE_ = "BotOfflineEventDropped";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	// virtual BotOfflineEventDropped* Clone() const override
	// {
	//	return new BotOfflineEventDropped(*this);
	// }
};

}


#endif