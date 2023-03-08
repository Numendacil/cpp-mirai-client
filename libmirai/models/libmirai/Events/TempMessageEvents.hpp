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

#ifndef MIRAI_TEMP_MESSAGE_EVENTS_HPP_
#define MIRAI_TEMP_MESSAGE_EVENTS_HPP_

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
 * `TempMessageEvent::sender_` | `GroupMember{}`
 * `TempMessageEvent::message_` | `MessageChain{}`
 */
class TempMessageEvent final : public IEvent<TempMessageEvent>
{
	friend IEvent<TempMessageEvent>;

private:
	GroupMember sender_;
	MessageChain message_;

	static constexpr EventTypes TYPE_ = EventTypes::TempMessage;

public:
	/// 获取发送者资料
	GroupMember GetSender() const { return this->sender_; }
	/// 获取消息内容
	MessageChain GetMessage() const { return this->message_; }

	// TODO: add helper methods for quick reply

	struct Serializable;
};



/**
 * @brief 临时会话消息同步事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `TempSyncMessageEvent::subject_` | `GroupMember{}`
 * `TempSyncMessageEvent::message_` | `MessageChain{}`
 */
class TempSyncMessageEvent final : public IEvent<TempSyncMessageEvent>
{
	friend IEvent<TempSyncMessageEvent>;

private:
	GroupMember subject_;
	MessageChain message_;

	static constexpr EventTypes TYPE_ = EventTypes::TempSyncMessage;

public:
	/// 获取会话目标对象信息
	GroupMember GetGroupMember() const { return this->subject_; }
	/// 获取消息内容
	MessageChain GetMessage() const { return this->message_; }

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