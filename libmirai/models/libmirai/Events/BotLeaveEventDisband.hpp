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

#ifndef _MIRAI_BOT_LEAVE_EVENT_DISBAND_HPP_
#define _MIRAI_BOT_LEAVE_EVENT_DISBAND_HPP_

#include <string>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

// TODO: add a base class for all BotLeaveEvent

/**
 * @brief 群聊解散导致Bot退群事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `BotLeaveEventDisband::_group` | `Group{}`
 * `BotLeaveEventDisband::_operator` | `GroupMember{}`
 */
class BotLeaveEventDisband : public EventBase
{
protected:
	Group _group;
	GroupMember _operator;

	void Deserialize(const void *) final;
public:
	using EventBase::EventBase;

	static constexpr std::string_view _TYPE_ = "BotLeaveEventDisband";

	/// 获取群聊信息
	Group GetGroup() const { return this->_group; }
	/// 获取操作员（群主）信息
	GroupMember GetOperator() const { return this->_operator; }
};

} // namespace Mirai


#endif