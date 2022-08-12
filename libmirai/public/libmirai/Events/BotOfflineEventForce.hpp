#ifndef _MIRAI_BOT_OFFLINE_EVENT_FORCE_HPP_
#define _MIRAI_BOT_OFFLINE_EVENT_FORCE_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "BotEvent.hpp"

namespace Mirai
{

class BotOfflineEventForce : public BotEvent
{
public:
	static constexpr std::string_view _TYPE_ = "BotOfflineEventForce";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	virtual BotOfflineEventForce* Clone() const override
	{
		return new BotOfflineEventForce(*this);
	}
};

}


#endif