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

#ifndef _MIRAI_MEMBER_JOIN_EVENT_HPP_
#define _MIRAI_MEMBER_JOIN_EVENT_HPP_

#include <optional>
#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 新成员加入群聊事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberJoinEvent::_member` | `GroupMember{}`
 * `MemberJoinEvent::_inviter` | `std::nullopt`
 */
class MemberJoinEvent : public EventBase
{
protected:
	GroupMember _member;
	std::optional<GroupMember> _inviter = std::nullopt;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "MemberJoinEvent";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual MemberJoinEvent* Clone() const override
	// {
	//	return new MemberJoinEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取群成员资料
	GroupMember GetMember() const { return this->_member; }
	/// 获取邀请人信息，若无则返回 `std::nullopt`
	std::optional<GroupMember> GetInviter() const { return this->_inviter; }
};

} // namespace Mirai


#endif