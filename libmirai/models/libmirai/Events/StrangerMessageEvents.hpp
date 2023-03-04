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

#ifndef _MIRAI_STRANGER_MESSAGE_EVENTS_HPP_
#define _MIRAI_STRANGER_MESSAGE_EVENTS_HPP_

#include <string>

#include <libmirai/Messages/MessageChain.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "IEvent.hpp"

namespace Mirai
{

/**
 * @brief 陌生人消息事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `StrangerMessageEvent::_sender` | `User{}`
 * `StrangerMessageEvent::_message` | `MessageChain{}`
 */
class StrangerMessageEvent final : public IEvent<StrangerMessageEvent>
{
	friend IEvent<StrangerMessageEvent>;

private:
	User _sender;
	MessageChain _message;

	static constexpr EventTypes _TYPE_ = EventTypes::StrangerMessage;

public:
	/// 获取发送者资料
	User GetSender() const { return this->_sender; }
	/// 获取消息内容
	MessageChain GetMessage() const { return this->_message; }

	// TODO: add helper methods for quick reply

	struct Serializable;
};



/**
 * @brief 陌生人消息同步事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `StrangerSyncMessageEvent::_subject` | `User{}`
 * `StrangerSyncMessageEvent::_message` | `MessageChain{}`
 */
class StrangerSyncMessageEvent final : public IEvent<StrangerSyncMessageEvent>
{
	friend IEvent<StrangerSyncMessageEvent>;

private:
	User _subject;
	MessageChain _message;

	static constexpr EventTypes _TYPE_ = EventTypes::StrangerSyncMessage;

public:
	/// 获取目标会话对象信息
	User GetStranger() const { return this->_subject; }
	/// 获取消息内容
	MessageChain GetMessage() const { return this->_message; }

	struct Serializable;
};

template<>
struct GetEventType<StrangerMessageEvent::GetType()>
{
	using type = StrangerMessageEvent;
};

template<>
struct GetEventType<StrangerSyncMessageEvent::GetType()>
{
	using type = StrangerSyncMessageEvent;
};

} // namespace Mirai


#endif