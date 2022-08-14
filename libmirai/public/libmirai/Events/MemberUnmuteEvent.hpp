#ifndef _MIRAI_MEMBER_UNMUTE_EVENT_HPP_
#define _MIRAI_MEMBER_UNMUTE_EVENT_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>


#include "EventBase.hpp"

namespace Mirai
{

class MemberUnmuteEvent : public EventBase
{
protected:
	GroupMember _member;
	std::optional<GroupMember> _operator = std::nullopt;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "MemberUnmuteEvent";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	// virtual MemberUnmuteEvent* Clone() const override
	// {
	//	return new MemberUnmuteEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	GroupMember GetMember() const { return this->_member; }
	std::optional<GroupMember> GetOperator() const { return this->_operator; }
};

}


#endif