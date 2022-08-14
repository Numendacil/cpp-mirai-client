#ifndef _MIRAI_MEMBER_LEAVE_EVENT_QUIT_HPP_
#define _MIRAI_MEMBER_LEAVE_EVENT_QUIT_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>


#include "EventBase.hpp"

namespace Mirai
{

class MemberLeaveEventQuit : public EventBase
{
protected:
	GroupMember _member;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "MemberLeaveEventQuit";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	// virtual MemberLeaveEventQuit* Clone() const override
	// {
	//	return new MemberLeaveEventQuit(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	GroupMember GetMember() const { return this->_member; }
};

}


#endif