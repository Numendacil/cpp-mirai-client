#ifndef _MIRAI_BOT_ONLINE_EVENT_HPP_
#define _MIRAI_BOT_ONLINE_EVENT_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "BotEvent.hpp"

namespace Mirai
{

class BotOnlineEvent : public BotEvent
{
public:
	static constexpr std::string_view _TYPE_ = "BotOnlineEvent";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	// virtual BotOnlineEvent* Clone() const override
	// {
	//	return new BotOnlineEvent(*this);
	// }
};

}


#endif