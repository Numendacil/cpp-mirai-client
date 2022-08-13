#ifndef _MIRAI_MEMBER_MUTE_EVENT_HPP_
#define _MIRAI_MEMBER_MUTE_EVENT_HPP_

#include <chrono>
#include <ctime>
#include <string>
#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>


#include "EventBase.hpp"

namespace Mirai
{

class MemberMuteEvent : public EventBase
{
protected:
	std::time_t _duration = 0;
	GroupMember _member;
	std::optional<GroupMember> _operator = std::nullopt;

public:
	static constexpr std::string_view _TYPE_ = "MemberMuteEvent";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	// virtual MemberMuteEvent* Clone() const override
	// {
	//	return new MemberMuteEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	std::chrono::seconds GetDuration() const { return std::chrono::seconds(this->_duration); }
	GroupMember GetMember() const { return this->_member; }
	std::optional<GroupMember> GetOperator() const { return this->_operator; }
};

}


#endif