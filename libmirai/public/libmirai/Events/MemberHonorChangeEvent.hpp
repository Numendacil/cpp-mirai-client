// Copyright (C) 2022 Numendacil and contributors
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
		if (i < _ActionKindStr.size()) return _ActionKindStr.at(i);
		else
			return "";
	}

	static constexpr ActionKind _to_enum(std::string_view s)
	{
		for (std::size_t i = 0; i < _ActionKindStr.size(); i++)
			if (_ActionKindStr.at(i) == s) return static_cast<ActionKind>(i);

		return ActionKind::UNKNOWN;
	}

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "MemberHonorChangeEvent";

	std::string_view GetType() const override { return _TYPE_; }

	// MemberHonorChangeEvent* Clone() const override
	// {
	//	return new MemberHonorChangeEvent(*this);
	// }

	void FromJson(const nlohmann::json& data) override;

	/// 获取群成员资料
	GroupMember GetMember() const { return this->_member; }
	/// 获取群荣誉变化行为
	ActionKind GetAction() const { return this->_action; }
	/// 获取群荣耀名称
	std::string GetHonor() const { return this->_honor; }
};

} // namespace Mirai


#endif