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

#ifndef _MIRAI_STRANGER_SYNC_MESSAGE_EVENT_HPP_
#define _MIRAI_STRANGER_SYNC_MESSAGE_EVENT_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Messages/MessageChain.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 陌生人消息同步事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `StrangerSyncMessageEvent::_subject` | `User{}`
 * `StrangerSyncMessageEvent::_message` | `MessageChain{}`
 */
class StrangerSyncMessageEvent : public EventBase
{

protected:
	User _subject;
	MessageChain _message;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "StrangerSyncMessage";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual StrangerSyncMessageEvent* Clone() const override
	// {
	//	return new StrangerSyncMessageEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取目标会话对象信息
	User GetStranger() const { return this->_subject; }
	/// 获取消息内容
	MessageChain GetMessage() const { return this->_message; }
};

} // namespace Mirai


#endif