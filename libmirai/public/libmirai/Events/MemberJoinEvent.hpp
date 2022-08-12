#ifndef _MIRAI_MEMBER_JOIN_EVENT_HPP_
#define _MIRAI_MEMBER_JOIN_EVENT_HPP_

#include <optional>
#include <string>
#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>


#include "EventBase.hpp"

namespace Mirai
{

class MemberJoinEvent : public EventBase
{
protected:
	GroupMember _member;
	std::optional<GroupMember> _inviter = std::nullopt;

public:
	static constexpr std::string_view _TYPE_ = "MemberJoinEvent";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	virtual MemberJoinEvent* Clone() const override
	{
		return new MemberJoinEvent(*this);
	}

	virtual void FromJson(const nlohmann::json& data) override;

	GroupMember GetMember() const { return this->_member; }
	std::optional<GroupMember> GetInviter() const { return this->_inviter;}
};

}


#endif