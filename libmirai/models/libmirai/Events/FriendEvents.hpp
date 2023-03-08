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

#ifndef MIRAI_FRIEND_EVENTS_HPP_
#define MIRAI_FRIEND_EVENTS_HPP_

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
 * `FriendInputStatusChangedEvent::friend_` | `User{}`
 * `FriendInputStatusChangedEvent::inputting_` | `false`
 */
class FriendInputStatusChangedEvent final : public IEvent<FriendInputStatusChangedEvent>
{
	friend IEvent<FriendInputStatusChangedEvent>;

private:
	User friend_;
	bool inputting_ = false;

	static constexpr EventTypes TYPE_ = EventTypes::FriendInputStatusChanged;

public:
	/// 获取好友信息
	User GetFriend() const { return this->friend_; }
	/// 是否正在输入
	bool isInputting() const { return this->inputting_; }

	struct Serializable;
};



/**
 * @brief 好友消息事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `FriendMessageEvent::sender_` | `User{}`
 * `FriendMessageEvent::message_` | `MessageChain{}`
 */
class FriendMessageEvent final : public IEvent<FriendMessageEvent>
{
	friend IEvent<FriendMessageEvent>;

private:
	User sender_;
	MessageChain message_;

	static constexpr EventTypes TYPE_ = EventTypes::FriendMessage;

public:
	/// 获取发送者资料
	User GetSender() const { return this->sender_; }
	/// 获取消息内容
	MessageChain GetMessage() const { return this->message_; }

	// TODO: add helper methods for quick reply

	struct Serializable;
};



/**
 * @brief 好友昵称改变事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `FriendNickChangedEvent::friend_` | `User{}`
 * `FriendNickChangedEvent::from_` | `""`
 * `FriendNickChangedEvent::to_` | `""`
 */
class FriendNickChangedEvent final : public IEvent<FriendNickChangedEvent>
{
	friend IEvent<FriendNickChangedEvent>;
private:
	User friend_;
	std::string from_;
	std::string to_;

	static constexpr EventTypes TYPE_ = EventTypes::FriendNickChanged;

public:
	/// 获取好友信息
	User GetFriend() const { return this->friend_; }
	/// 获取更改前的昵称
	std::string GetOriginalNickname() const { return this->from_; }
	/// 获取当前昵称
	std::string GetNewNickname() const { return this->to_; }

	struct Serializable;
};



/**
 * @brief 好友消息撤回事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `FriendRecallEvent::AuthorId_` | `0_qq`
 * `FriendRecallEvent::MessageId_` | `0`
 * `FriendRecallEvent::time_` | `0`
 * `FriendRecallEvent::operator_` | `0_qq`
 */
class FriendRecallEvent final : public IEvent<FriendRecallEvent>
{
	friend IEvent<FriendRecallEvent>;
private:
	QQ_t AuthorId_;
	MessageId_t MessageId_ = 0;
	std::time_t time_ = 0;
	QQ_t operator_;

	static constexpr EventTypes TYPE_ = EventTypes::FriendRecall;

public:
	/// 获取被撤回消息的发送者QQ
	QQ_t GetSender() const { return this->AuthorId_; }
	/// 获取被撤回消息的消息id
	MessageId_t GetMessageId() const { return this->MessageId_; }
	/// 获取被撤回消息的发送时间
	std::time_t GetSendTime() const { return this->time_; }
	/// 获取操作员QQ
	QQ_t GetOperator() const { return this->operator_; }

	struct Serializable;
};



/**
 * @brief 好友消息同步事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `FriendSyncMessageEvent::subject_` | `User{}`
 * `FriendSyncMessageEvent::message_` | `MessageChain{}`
 */
class FriendSyncMessageEvent final : public IEvent<FriendSyncMessageEvent>
{
	friend IEvent<FriendSyncMessageEvent>;
private:
	User subject_;
	MessageChain message_;

	static constexpr EventTypes TYPE_ = EventTypes::FriendSyncMessage;

public:
	/// 获取目标好友信息
	User GetFriend() const { return this->subject_; }
	/// 获取消息内容
	MessageChain GetMessage() const { return this->message_; }

	struct Serializable;
};



#define DECLARE_TYPE_ENUM(event_type)                                                                                      \
	template<> struct GetEventType<event_type::GetType()>                                                                  \
	{                                                                                                                  \
		using type = event_type;                                                                                           \
	}

DECLARE_TYPE_ENUM(FriendInputStatusChangedEvent);
DECLARE_TYPE_ENUM(FriendMessageEvent);
DECLARE_TYPE_ENUM(FriendNickChangedEvent);
DECLARE_TYPE_ENUM(FriendRecallEvent);
DECLARE_TYPE_ENUM(FriendSyncMessageEvent);

#undef DECLARE_TYPE_ENUM

} // namespace Mirai


#endif