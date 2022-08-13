#ifndef _MIRAI_BOT_UNMUTE_EVENT_HPP_
#define _MIRAI_BOT_UNMUTE_EVENT_HPP_

#include <chrono>
#include <ctime>
#include <string>
#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>


#include "EventBase.hpp"

namespace Mirai
{

class BotUnmuteEvent : public EventBase
{
protected:
	GroupMember _operator;

public:
	static constexpr std::string_view _TYPE_ = "BotUnmuteEvent";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	// virtual BotUnmuteEvent* Clone() const override
	// {
	//	return new BotUnmuteEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	GroupMember GetOperator() const { return this->_operator; }
};

}


#endif