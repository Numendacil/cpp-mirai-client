#ifndef _MIRAI_MEMBER_PERMISSION_CHANGE_EVENT_HPP_
#define _MIRAI_MEMBER_PERMISSION_CHANGE_EVENT_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>


#include "EventBase.hpp"

namespace Mirai
{

class MemberPermissionChangeEvent : public EventBase
{
protected:
	PERMISSION _origin = PERMISSION::UNKNOWN;
	PERMISSION _current = PERMISSION::UNKNOWN;
	GroupMember _member;

public:
	static constexpr std::string_view _TYPE_ = "MemberPermissionChangeEvent";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	// virtual MemberPermissionChangeEvent* Clone() const override
	// {
	//	return new MemberPermissionChangeEvent(*this);
	// }/ }

	virtual void FromJson(const nlohmann::json& data) override;

	PERMISSION GetOriginal() const { return this->_origin; }
	PERMISSION GetCurrent() const { return this->_current; }
	GroupMember GetMember() const { return this->_member; }
};

}


#endif