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

#ifndef _MIRAI_BOT_OFFLINE_EVENT_FORCE_HPP_
#define _MIRAI_BOT_OFFLINE_EVENT_FORCE_HPP_

#include <string>

#include "BotEvent.hpp"

namespace Mirai
{

/**
 * @brief Bot强制下线事件
 * 
 * 通常是因为在相同平台的其它设备登录了Bot被挤下线
 */
class BotOfflineEventForce : public BotEvent
{
public:
	using BotEvent::BotEvent;
	static constexpr std::string_view _TYPE_ = "BotOfflineEventForce";

protected:
	std::string GetEventType() const final { return std::string(_TYPE_); }
};

} // namespace Mirai


#endif