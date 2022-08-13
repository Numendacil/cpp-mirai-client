#ifndef _MIRAI_BOT_MUTE_EVENT_HPP_
#define _MIRAI_BOT_MUTE_EVENT_HPP_

#include <chrono>
#include <ctime>
#include <string>
#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>


#include "EventBase.hpp"

namespace Mirai
{

class BotMuteEvent : public EventBase
{
protected:
	GroupMember _operator;
	std::time_t _duration = 0;

public:
	static constexpr std::string_view _TYPE_ = "BotMuteEvent";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	// virtual BotMuteEvent* Clone() const override
	// {
	//	return new BotMuteEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	GroupMember GetOperator() const { return this->_operator; }
	std::chrono::seconds GetMuteTime() const { return std::chrono::seconds(this->_duration); }
};

}


#endif