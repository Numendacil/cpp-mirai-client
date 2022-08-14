#ifndef _MIRAI_BOT_LEAVE_EVENT_DISBAND_HPP_
#define _MIRAI_BOT_LEAVE_EVENT_DISBAND_HPP_

#include <string>
#include <optional>
#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>


#include "EventBase.hpp"

namespace Mirai
{

class BotLeaveEventDisband : public EventBase
{
protected:
	Group _group;
	std::optional<GroupMember> _operator = std::nullopt;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "BotLeaveEventDisband";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	// virtual BotLeaveEventDisband* Clone() const override
	// {
	//	return new BotLeaveEventDisband(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	Group GetGroup() const { return this->_group; }
	std::optional<GroupMember> GetOperator() const { return this->_operator; }
};

}


#endif