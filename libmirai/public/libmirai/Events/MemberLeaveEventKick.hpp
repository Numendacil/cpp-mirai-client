#ifndef _MIRAI_MEMBER_LEAVE_EVENT_KICK_HPP_
#define _MIRAI_MEMBER_LEAVE_EVENT_KICK_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>


#include "EventBase.hpp"

namespace Mirai
{

class MemberLeaveEventKick : public EventBase
{
protected:
	GroupMember _member;
	std::optional<GroupMember> _operator = std::nullopt;

public:
	static constexpr std::string_view _TYPE_ = "MemberLeaveEventKick";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	virtual MemberLeaveEventKick* Clone() const override
	{
		return new MemberLeaveEventKick(*this);
	}

	virtual void FromJson(const nlohmann::json& data) override;

	GroupMember GetMember() const { return this->_member; }
	std::optional<GroupMember> GetOperator() const { return this->_operator; }
};

}


#endif