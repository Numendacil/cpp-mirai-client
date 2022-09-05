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

#ifndef _MIRAI_MEMBER_UNMUTE_EVENT_HPP_
#define _MIRAI_MEMBER_UNMUTE_EVENT_HPP_

#include <optional>
#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 群成员被取消禁言事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberUnmuteEvent::_member` | `GroupMember{}`
 * `MemberUnmuteEvent::_operator` | `std::nullopt`
 *
 * 该成员不是Bot自己
 */
class MemberUnmuteEvent : public EventBase
{
protected:
	GroupMember _member;
	std::optional<GroupMember> _operator = std::nullopt;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "MemberUnmuteEvent";

	std::string_view GetType() const override { return _TYPE_; }

	// MemberUnmuteEvent* Clone() const override
	// {
	//	return new MemberUnmuteEvent(*this);
	// }

	void FromJson(const nlohmann::json& data) override;

	/// 获取群成员资料
	GroupMember GetMember() const { return this->_member; }
	/// 获取操作员信息，为Bot时返回 `std::nullopt`
	std::optional<GroupMember> GetOperator() const { return this->_operator; }
};

} // namespace Mirai


#endif