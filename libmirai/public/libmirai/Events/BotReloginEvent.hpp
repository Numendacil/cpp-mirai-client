#ifndef _MIRAI_BOT_RELOGIN_EVENT_HPP_
#define _MIRAI_BOT_RELOGIN_EVENT_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "BotEvent.hpp"

namespace Mirai
{

class BotReloginEvent : public BotEvent
{
public:
	static constexpr std::string_view _TYPE_ = "BotReloginEvent";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	virtual BotReloginEvent* Clone() const override
	{
		return new BotReloginEvent(*this);
	}
};

}


#endif