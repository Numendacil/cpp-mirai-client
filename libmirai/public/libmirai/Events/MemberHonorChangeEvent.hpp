#ifndef _MIRAI_MEMBER_HONOR_CHANGE_EVENT_HPP_
#define _MIRAI_MEMBER_HONOR_CHANGE_EVENT_HPP_

#include <array>
#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 群成员荣誉改变事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberHonorChangeEvent::_member` | `GroupMember{}`
 * `MemberHonorChangeEvent::_action` | `ActionKind::UNKNOWN`
 * `MemberHonorChangeEvent::_honor` | `""`
 *
 * 目前只支持龙王
 */
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
	GroupMember _member;
	ActionKind _action = ActionKind::UNKNOWN; // {achieve, lose}
	std::string _honor;

	static constexpr std::array<std::string_view, static_cast<std::size_t>(ActionKind::UNKNOWN)> _ActionKindStr = {
		"achieve", "lose"};

	static constexpr std::string_view _to_string(const ActionKind& m)
	{
		auto i = static_cast<std::size_t>(m);
		if (i < _ActionKindStr.size()) return _ActionKindStr[i];
		else
			return "";
	}

	static constexpr ActionKind _to_enum(std::string_view s)
	{
		for (std::size_t i = 0; i < _ActionKindStr.size(); i++)
			if (_ActionKindStr[i] == s) return static_cast<ActionKind>(i);

		return ActionKind::UNKNOWN;
	}

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "MemberHonorChangeEvent";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual MemberHonorChangeEvent* Clone() const override
	// {
	//	return new MemberHonorChangeEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取群成员资料
	GroupMember GetMember() const { return this->_member; }
	/// 获取群荣誉变化行为
	ActionKind GetAction() const { return this->_action; }
	/// 获取群荣耀名称
	std::string GetHonor() const { return this->_honor; }
};

} // namespace Mirai


#endif