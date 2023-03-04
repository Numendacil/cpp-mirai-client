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

#ifndef _MIRAI_TEMP_MESSAGE_EVENTS_HPP_
#define _MIRAI_TEMP_MESSAGE_EVENTS_HPP_

#include <string>

#include <libmirai/Messages/MessageChain.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "IEvent.hpp"

namespace Mirai
{

/**
 * @brief 临时会话消息事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `TempMessageEvent::_sender` | `GroupMember{}`
 * `TempMessageEvent::_message` | `MessageChain{}`
 */
class TempMessageEvent final : public IEvent<TempMessageEvent>
{
	friend IEvent<TempMessageEvent>;

private:
	GroupMember _sender;
	MessageChain _message;

	static constexpr EventTypes _TYPE_ = EventTypes::TempMessage;

public:
	/// 获取发送者资料
	GroupMember GetSender() const { return this->_sender; }
	/// 获取消息内容
	MessageChain GetMessage() const { return this->_message; }

	// TODO: add helper methods for quick reply

	struct Serializable;
};



/**
 * @brief 临时会话消息同步事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `TempSyncMessageEvent::_subject` | `GroupMember{}`
 * `TempSyncMessageEvent::_message` | `MessageChain{}`
 */
class TempSyncMessageEvent final : public IEvent<TempSyncMessageEvent>
{
	friend IEvent<TempSyncMessageEvent>;

private:
	GroupMember _subject;
	MessageChain _message;

	static constexpr EventTypes _TYPE_ = EventTypes::TempSyncMessage;

public:
	/// 获取会话目标对象信息
	GroupMember GetGroupMember() const { return this->_subject; }
	/// 获取消息内容
	MessageChain GetMessage() const { return this->_message; }

	struct Serializable;
};

template<>
struct GetEventType<TempMessageEvent::GetType()>
{
	using type = TempMessageEvent;
};

template<>
struct GetEventType<TempSyncMessageEvent::GetType()>
{
	using type = TempSyncMessageEvent;
};

} // namespace Mirai


#endif