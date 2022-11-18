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

#ifndef _MIRAI_MEMBER_LEAVE_EVENT_KICK_HPP_
#define _MIRAI_MEMBER_LEAVE_EVENT_KICK_HPP_

#include <optional>
#include <string>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

// TODO: add a base class for all MemberLeaveEvent

/**
 * @brief 成员被踢出群聊事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberLeaveEventKick::_member` | `GroupMember{}`
 * `MemberLeaveEventKick::_operator` | `std::nullopt`
 *
 * 该成员不是Bot自己
 */
class MemberLeaveEventKick : public EventBase
{
protected:
	GroupMember _member;
	std::optional<GroupMember> _operator = std::nullopt;

	void Deserialize(const void *) final;
public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "MemberLeaveEventKick";

	/// 获取群成员资料
	GroupMember GetMember() const { return this->_member; }
	/// 获取操作员信息，为Bot时返回 `std::nullopt`
	std::optional<GroupMember> GetOperator() const { return this->_operator; }
};

} // namespace Mirai


#endif