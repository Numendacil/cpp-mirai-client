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

#ifndef _MIRAI_FRIEND_EVENTS_HPP_
#define _MIRAI_FRIEND_EVENTS_HPP_

#include <string>

#include <libmirai/Messages/MessageChain.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "IEvent.hpp"

namespace Mirai
{

/**
 * @brief 好友输入状态改变事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `FriendInputStatusChangedEvent::_friend` | `User{}`
 * `FriendInputStatusChangedEvent::_inputting` | `false`
 */
class FriendInputStatusChangedEvent final : public IEvent<FriendInputStatusChangedEvent>
{
	friend IEvent<FriendInputStatusChangedEvent>;

private:
	User _friend;
	bool _inputting = false;

	static constexpr EventTypes _TYPE_ = EventTypes::FriendInputStatusChanged;

public:
	/// 获取好友信息
	User GetFriend() const { return this->_friend; }
	/// 是否正在输入
	bool isInputting() const { return this->_inputting; }

	struct Serializable;
};



/**
 * @brief 好友消息事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `FriendMessageEvent::_sender` | `User{}`
 * `FriendMessageEvent::_message` | `MessageChain{}`
 */
class FriendMessageEvent final : public IEvent<FriendMessageEvent>
{
	friend IEvent<FriendMessageEvent>;

private:
	User _sender;
	MessageChain _message;

	static constexpr EventTypes _TYPE_ = EventTypes::FriendMessage;

public:
	/// 获取发送者资料
	User GetSender() const { return this->_sender; }
	/// 获取消息内容
	MessageChain GetMessage() const { return this->_message; }

	// TODO: add helper methods for quick reply

	struct Serializable;
};



/**
 * @brief 好友昵称改变事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `FriendNickChangedEvent::_friend` | `User{}`
 * `FriendNickChangedEvent::_from` | `""`
 * `FriendNickChangedEvent::_to` | `""`
 */
class FriendNickChangedEvent final : public IEvent<FriendNickChangedEvent>
{
	friend IEvent<FriendNickChangedEvent>;
private:
	User _friend;
	std::string _from;
	std::string _to;

	static constexpr EventTypes _TYPE_ = EventTypes::FriendNickChanged;

public:
	/// 获取好友信息
	User GetFriend() const { return this->_friend; }
	/// 获取更改前的昵称
	std::string GetOriginalNickname() const { return this->_from; }
	/// 获取当前昵称
	std::string GetNewNickname() const { return this->_to; }

	struct Serializable;
};



/**
 * @brief 好友消息撤回事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `FriendRecallEvent::_AuthorId` | `0_qq`
 * `FriendRecallEvent::_MessageId` | `0`
 * `FriendRecallEvent::_time` | `0`
 * `FriendRecallEvent::_operator` | `0_qq`
 */
class FriendRecallEvent final : public IEvent<FriendRecallEvent>
{
	friend IEvent<FriendRecallEvent>;
private:
	QQ_t _AuthorId;
	MessageId_t _MessageId = 0;
	std::time_t _time = 0;
	QQ_t _operator;

	static constexpr EventTypes _TYPE_ = EventTypes::FriendRecall;

public:
	/// 获取被撤回消息的发送者QQ
	QQ_t GetSender() const { return this->_AuthorId; }
	/// 获取被撤回消息的消息id
	MessageId_t GetMessageId() const { return this->_MessageId; }
	/// 获取被撤回消息的发送时间
	std::time_t GetSendTime() const { return this->_time; }
	/// 获取操作员QQ
	QQ_t GetOperator() const { return this->_operator; }

	struct Serializable;
};



/**
 * @brief 好友消息同步事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `FriendSyncMessageEvent::_subject` | `User{}`
 * `FriendSyncMessageEvent::_message` | `MessageChain{}`
 */
class FriendSyncMessageEvent final : public IEvent<FriendSyncMessageEvent>
{
	friend IEvent<FriendSyncMessageEvent>;
private:
	User _subject;
	MessageChain _message;

	static constexpr EventTypes _TYPE_ = EventTypes::FriendSyncMessage;

public:
	/// 获取目标好友信息
	User GetFriend() const { return this->_subject; }
	/// 获取消息内容
	MessageChain GetMessage() const { return this->_message; }

	struct Serializable;
};



#define DECLARE_TYPE_ENUM(_type_)                                                                                      \
	template<> struct GetEventType<_type_::GetType()>                                                                  \
	{                                                                                                                  \
		using type = _type_;                                                                                           \
	}

DECLARE_TYPE_ENUM(FriendInputStatusChangedEvent);
DECLARE_TYPE_ENUM(FriendMessageEvent);
DECLARE_TYPE_ENUM(FriendNickChangedEvent);
DECLARE_TYPE_ENUM(FriendRecallEvent);
DECLARE_TYPE_ENUM(FriendSyncMessageEvent);

#undef DECLARE_TYPE_ENUM

} // namespace Mirai


#endif