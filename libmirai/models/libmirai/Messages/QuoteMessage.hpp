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

#ifndef MIRAI_QUOTE_MESSAGE_HPP_
#define MIRAI_QUOTE_MESSAGE_HPP_

#include <memory>
#include <string>

#include <libmirai/Types/BasicTypes.hpp>

#include "IMessage.hpp"

namespace Mirai
{

class MessageChain;

/**
 * @brief 引用回复消息
 *
 * 仅用于接收，发送时将会被无视。发送引用回复消息请在消息发送函数中设置 `QuoteId`
 *
 * Member Variable | Default Value
 * --------------- | -------------
 * `QuoteMessage::QuoteId_` | `-1`
 * `QuoteMessage::GroupId_` | `0_gid`
 * `QuoteMessage::SenderId_` | `0_qq`
 * `QuoteMessage::TargetId_` | `0`
 * `QuoteMessage::origin_` | `MessageChain{}`
 */
class QuoteMessage final : public IMessageImpl<QuoteMessage>
{
	friend IMessageImpl<QuoteMessage>;

protected:
	MessageId_t QuoteId_ = -1;
	GID_t GroupId_{};
	QQ_t SenderId_{};
	int64_t TargetId_ = 0;
	std::unique_ptr<MessageChain> origin_{};

	static constexpr MessageTypes TYPE_ = MessageTypes::QUOTE;
	static constexpr bool SUPPORT_SEND_ = false;

	bool isValid_() const final { return true; }

public:
	QuoteMessage();
	QuoteMessage(const QuoteMessage&);
	QuoteMessage& operator=(const QuoteMessage&);
	QuoteMessage(QuoteMessage&&);
	QuoteMessage& operator=(QuoteMessage&&);

	/// 获取被引用消息id
	MessageId_t GetQuoteId() const { return this->QuoteId_; }
	/// 获取被引用消息所在群聊id，若为好友消息则为 `0`
	GID_t GetGroupId() const { return this->GroupId_; }
	/// 获取引用消息的发送者QQ
	QQ_t GetSenderId() const { return this->SenderId_; }
	/// 获取引用消息的接收者QQ，仅当好友消息时有效
	QQ_t GetTargetId() const
	{
		if (this->GroupId_ == (GID_t)0) return QQ_t(this->TargetId_);
		else
			return QQ_t(0);
	}
	/// 获取被引用消息
	MessageChain GetOriginMessage() const;

	struct Serializable;
};

template<> struct GetType<QuoteMessage::GetType()>
{
	using type = QuoteMessage;
};

} // namespace Mirai


#endif