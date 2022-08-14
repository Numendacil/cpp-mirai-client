#ifndef _MIRAI_MEMBER_CARD_CHANGE_EVENT_HPP_
#define _MIRAI_MEMBER_CARD_CHANGE_EVENT_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>


#include "EventBase.hpp"

namespace Mirai
{

class MemberCardChangeEvent : public EventBase
{
protected:
	std::string _origin;
	std::string _current;
	GroupMember _member;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "MemberCardChangeEvent";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	// virtual MemberCardChangeEvent* Clone() const override
	// {
	//	return new MemberCardChangeEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	std::string GetOriginal() const { return this->_origin; }
	std::string GetCurrent() const { return this->_current; }
	GroupMember GetMember() const { return this->_member; }
};

}


#endif