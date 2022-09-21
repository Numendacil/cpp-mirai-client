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

#ifndef _MIRAI_MEMBER_LEAVE_EVENT_QUIT_HPP_
#define _MIRAI_MEMBER_LEAVE_EVENT_QUIT_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

// TODO: add a base class for all MemberLeaveEvent

/**
 * @brief 成员主动退出群聊事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberLeaveEventQuit::_member` | `GroupMember{}`
 *
 * 该成员不是Bot自己
 */
class MemberLeaveEventQuit : public EventBase
{
protected:
	GroupMember _member;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "MemberLeaveEventQuit";

	std::string_view GetType() const override { return _TYPE_; }

	// MemberLeaveEventQuit* Clone() const override
	// {
	//	return new MemberLeaveEventQuit(*this);
	// }

	void FromJson(const nlohmann::json& data) override;

	/// 获取群成员资料
	GroupMember GetMember() const { return this->_member; }
};

} // namespace Mirai


#endif