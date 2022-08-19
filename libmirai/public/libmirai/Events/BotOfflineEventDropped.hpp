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

#ifndef _MIRAI_BOT_OFFLINE_EVENT_DROPPED_HPP_
#define _MIRAI_BOT_OFFLINE_EVENT_DROPPED_HPP_

#include <string>

#include "BotEvent.hpp"

namespace Mirai
{

/**
 * @brief Bot掉线事件
 * 
 * 网络原因或服务器原因导致的掉线
 */
class BotOfflineEventDropped : public BotEvent
{
public:
	using BotEvent::BotEvent;
	static constexpr std::string_view _TYPE_ = "BotOfflineEventDropped";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual BotOfflineEventDropped* Clone() const override
	// {
	//	return new BotOfflineEventDropped(*this);
	// }
};

} // namespace Mirai


#endif