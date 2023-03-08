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

#ifndef MIRAI_NUDGE_EVENT_HPP_
#define MIRAI_NUDGE_EVENT_HPP_

#include <string>

#include <libmirai/Types/BasicTypes.hpp>
#include <libmirai/Types/NudgeTarget.hpp>

#include "IEvent.hpp"

namespace Mirai
{

/**
 * @brief 戳一戳事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `NudgeEvent::FromId_` | `0_qq`
 * `NudgeEvent::target_` | `NudgeTarget{}`
 * `NudgeEvent::action_` | `""`
 * `NudgeEvent::suffix_` | `""`
 */
class NudgeEvent final : public IEvent<NudgeEvent>
{
	friend IEvent<NudgeEvent>;
private:
	QQ_t FromId_;
	NudgeTarget target_;
	std::string action_;
	std::string suffix_;

	static constexpr EventTypes TYPE_ = EventTypes::Nudge;

public:
	/// 获取发送者QQ
	QQ_t GetSender() const { return this->FromId_; }
	/// 获取戳一戳消息的接收对象
	NudgeTarget GetTarget() const { return this->target_; }
	/// 获取自定义戳一戳的动作消息
	std::string GetAction() const { return this->action_; }
	/// 获取自定义戳一戳的内容
	std::string GetSuffix() const { return this->suffix_; }

	struct Serializable;
};

template<>
struct GetEventType<NudgeEvent::GetType()>
{
	using type = NudgeEvent;
};

} // namespace Mirai


#endif