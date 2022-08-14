#ifndef _MIRAI_GROUP_ALLOW_MEMBER_INVITE_EVENT_HPP_
#define _MIRAI_GROUP_ALLOW_MEMBER_INVITE_EVENT_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>


#include "EventBase.hpp"

namespace Mirai
{

class GroupAllowMemberInviteEvent : public EventBase
{
protected:
	bool _origin = false;
	bool _current = false;
	Group _group;
	std::optional<GroupMember> _operator = std::nullopt;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "GroupAllowMemberInviteEvent";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	// virtual GroupAllowMemberInviteEvent* Clone() const override
	// {
	//	return new GroupAllowMemberInviteEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	bool GetOriginal() const { return this->_origin; }
	bool GetCurrent() const { return this->_current; }
	Group GetGroup() const { return this->_group; }
	std::optional<GroupMember> GetOperator() const { return this->_operator; }
};

}


#endif