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

#ifndef _MIRAI_MEMBER_MUTE_EVENT_HPP_
#define _MIRAI_MEMBER_MUTE_EVENT_HPP_

#include <chrono>
#include <ctime>
#include <optional>
#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 群成员被禁言事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberMuteEvent::_member` | `GroupMember{}`
 * `MemberMuteEvent::_duration` | `0`
 * `MemberMuteEvent::_operator` | `std::nullopt`
 *
 * 该成员不是Bot自己
 */
class MemberMuteEvent : public EventBase
{
protected:
	GroupMember _member;
	std::time_t _duration = 0;
	std::optional<GroupMember> _operator = std::nullopt;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "MemberMuteEvent";

	std::string_view GetType() const override { return _TYPE_; }

	// MemberMuteEvent* Clone() const override
	// {
	//	return new MemberMuteEvent(*this);
	// }

	void FromJson(const nlohmann::json& data) override;

	/// 获取群成员资料
	GroupMember GetMember() const { return this->_member; }
	/// 获取禁言时长
	std::chrono::seconds GetDuration() const { return std::chrono::seconds(this->_duration); }
	/// 获取操作员信息，为Bot时返回 `std::nullopt`
	std::optional<GroupMember> GetOperator() const { return this->_operator; }
};

} // namespace Mirai


#endif