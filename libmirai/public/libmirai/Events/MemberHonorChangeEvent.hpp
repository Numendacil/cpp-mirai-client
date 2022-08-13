#ifndef _MIRAI_MEMBER_HONOR_CHANGE_EVENT_HPP_
#define _MIRAI_MEMBER_HONOR_CHANGE_EVENT_HPP_

#include <string>
#include <array>
#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>


#include "EventBase.hpp"

namespace Mirai
{

class MemberHonorChangeEvent : public EventBase
{
public:
	enum ActionKind : std::size_t 
	{ 
		ACHIEVE = 0,
		LOSE,
		UNKNOWN
	};
protected:
	std::string _action;	// {achieve, lose}
	std::string _honor;
	GroupMember _member;

	static constexpr std::array<std::string_view, static_cast<std::size_t>(ActionKind::UNKNOWN)> _ActionKindStr = 
	{
		"achieve",
		"lose"
	};

	static constexpr std::string_view _to_string(const ActionKind& m)
	{
		auto i = static_cast<std::size_t>(m);
		if (i < _ActionKindStr.size())
			return _ActionKindStr[i];
		else
			return "";
	}

	static constexpr ActionKind _to_enum(std::string_view s)
	{
		for (std::size_t i = 0; i < _ActionKindStr.size(); i++)
			if (_ActionKindStr[i] == s)
				return static_cast<ActionKind>(i);

		return ActionKind::UNKNOWN;
	}

public:
	static constexpr std::string_view _TYPE_ = "MemberHonorChangeEvent";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	// virtual MemberHonorChangeEvent* Clone() const override
	// {
	//	return new MemberHonorChangeEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	ActionKind GetAction() const { return _to_enum(this->_action); }
	std::string GetHonor() const { return this->_honor; }
	GroupMember GetMember() const { return this->_member; }
};

}


#endif