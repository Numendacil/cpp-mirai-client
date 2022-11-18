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

#ifndef _MIRAI_NUDGE_EVENT_HPP_
#define _MIRAI_NUDGE_EVENT_HPP_

#include <string>

#include <libmirai/Types/BasicTypes.hpp>
#include <libmirai/Types/NudgeTarget.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 戳一戳事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `NudgeEvent::_FromId` | `0_qq`
 * `NudgeEvent::_target` | `NudgeTarget{}`
 * `NudgeEvent::_action` | `""`
 * `NudgeEvent::_suffix` | `""`
 */
class NudgeEvent : public EventBase
{
protected:
	QQ_t _FromId;
	NudgeTarget _target;
	std::string _action;
	std::string _suffix;

	void Deserialize(const void *) final;
public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "NudgeEvent";

	/// 获取发送者QQ
	QQ_t GetSender() const { return this->_FromId; }
	/// 获取戳一戳消息的接收对象
	NudgeTarget GetTarget() const { return this->_target; }
	/// 获取自定义戳一戳的动作消息
	std::string GetAction() const { return this->_action; }
	/// 获取自定义戳一戳的内容
	std::string GetSuffix() const { return this->_suffix; }
};

} // namespace Mirai


#endif