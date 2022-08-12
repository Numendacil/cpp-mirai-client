#ifndef _MIRAI_BOT_OFFLINE_EVENT_ACTIVE_HPP_
#define _MIRAI_BOT_OFFLINE_EVENT_ACTIVE_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "BotEvent.hpp"

namespace Mirai
{

class BotOfflineEventActive : public BotEvent
{
public:
	static constexpr std::string_view _TYPE_ = "BotOfflineEventActive";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	virtual BotOfflineEventActive* Clone() const override
	{
		return new BotOfflineEventActive(*this);
	}
};

}


#endif