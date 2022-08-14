#ifndef _MIRAI_BOT_LEAVE_EVENT_ACTIVE_HPP_
#define _MIRAI_BOT_LEAVE_EVENT_ACTIVE_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>


#include "EventBase.hpp"

namespace Mirai
{

class BotLeaveEventActive : public EventBase
{
protected:
	Group _group;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "BotLeaveEventActive";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	// virtual BotLeaveEventActive* Clone() const override
	// {
	//	return new BotLeaveEventActive(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	Group GetGroup() const { return this->_group; }
};

}


#endif