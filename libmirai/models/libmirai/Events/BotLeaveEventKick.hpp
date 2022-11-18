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

#ifndef _MIRAI_BOT_LEAVE_EVENT_KICK_HPP_
#define _MIRAI_BOT_LEAVE_EVENT_KICK_HPP_

#include <string>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

// TODO: add a base class for all BotLeaveEvent

/**
 * @brief Bot被踢出群聊事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `BotLeaveEventKick::_group` | `Group{}`
 * `BotLeaveEventKick::_operator` | `GroupMember{}`
 */
class BotLeaveEventKick : public EventBase
{
protected:
	Group _group;
	GroupMember _operator;

	void Deserialize(const void *) final;
public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "BotLeaveEventKick";

	/// 获取群聊信息
	Group GetGroup() const { return this->_group; }
	/// 获取操作员信息，若无则返回 `std::nullopt`
	GroupMember GetOperator() const { return this->_operator; }
};

} // namespace Mirai


#endif