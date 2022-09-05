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

#ifndef _MIRAI_BOT_EVENT_HPP_
#define _MIRAI_BOT_EVENT_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 与Bot本身相关的事件的基类
 * 
 * 不可直接使用，仅用作其它事件的基类
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `BotEvent::_qq` | `0_qq`
 */
class BotEvent : public EventBase
{

protected:
	QQ_t _qq;

public:
	using EventBase::EventBase;
	// static constexpr std::string_view _TYPE_ = "BotEvent";

	// std::string_view GetType() const override
	// {
	// 	return _TYPE_;
	// }

	// BotEvent* Clone() const override
	// {
	// 	return new BotEvent(*this);
	// }

	void FromJson(const nlohmann::json& data) override;

	/// 获取BotQQ
	QQ_t GetQQ() const { return this->_qq; }
};

} // namespace Mirai


#endif