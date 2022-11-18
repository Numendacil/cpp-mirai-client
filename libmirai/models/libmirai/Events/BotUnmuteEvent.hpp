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

#ifndef _MIRAI_BOT_UNMUTE_EVENT_HPP_
#define _MIRAI_BOT_UNMUTE_EVENT_HPP_

#include <chrono>
#include <ctime>
#include <string>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief Bot被取消禁言事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `BotUnmuteEvent::_operator` | `GroupMember{}`
 */
class BotUnmuteEvent : public EventBase
{
protected:
	GroupMember _operator;

	void Deserialize(const void *) final;
public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "BotUnmuteEvent";

	/// 获取操作员信息
	GroupMember GetOperator() const { return this->_operator; }
};

} // namespace Mirai


#endif