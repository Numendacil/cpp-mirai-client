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

#ifndef _MIRAI_BOT_MUTE_EVENT_HPP_
#define _MIRAI_BOT_MUTE_EVENT_HPP_

#include <chrono>
#include <ctime>
#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief Bot被禁言事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `BotMuteEvent::_operator` | `GroupMember{}`
 * `BotMuteEvent::_duration` | `0`
 */
class BotMuteEvent : public EventBase
{
protected:
	GroupMember _operator;
	std::time_t _duration = 0;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "BotMuteEvent";

	std::string_view GetType() const override { return _TYPE_; }

	// BotMuteEvent* Clone() const override
	// {
	//	return new BotMuteEvent(*this);
	// }

	void FromJson(const nlohmann::json& data) override;

	/// 获取操作员信息
	GroupMember GetOperator() const { return this->_operator; }
	/// 获取禁言时间
	std::chrono::seconds GetMuteTime() const { return std::chrono::seconds(this->_duration); }
};

} // namespace Mirai


#endif