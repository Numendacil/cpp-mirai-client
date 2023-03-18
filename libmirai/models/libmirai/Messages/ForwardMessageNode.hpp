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

#ifndef MIRAI_FORWARD_MESSAGE_NODE_HPP_
#define MIRAI_FORWARD_MESSAGE_NODE_HPP_

#include <optional>
#include "ForwardMessage.hpp"
#include "MessageChain.hpp"

namespace Mirai
{

/**
* @brief 转发消息节点，代表一条转发消息
*
* 接收消息时会传入 `MessageChain`，消息id一般为 `null`。
* 发送时可以选择使用消息链手动构造转发消息或传入消息id自动引用历史消息，
* 优先级为 `MessageId > MessageChain`
*
* Member Variable | Default Value
* --------------- | -------------
* `Node::SenderId_` | `0_qq`
* `Node::time_` | `0`
* `Node::SenderName_` | `""`
* `Node::message_` | `MessageChain{}`
* `Node::MessageId_` | `std::nullopt`
* `Node::ref_` | `std::nullopt`
*/
class ForwardMessage::Node
{
protected:
	QQ_t SenderId_{};
	std::time_t time_ = 0;
	std::string SenderName_{};
	MessageChain message_{};
	std::optional<MessageId_t> MessageId_ = std::nullopt;

	struct MessageRef
	{
		MessageId_t MessageId;
		int64_t target;
	};
	std::optional<MessageRef> ref_ = std::nullopt;

public:
	Node() = default;
	~Node() = default;

	bool valid() const
	{
		return this->MessageId_.has_value() || (!this->SenderName_.empty() && this->message_.valid());
	}

	/// 获取发送者QQ
	QQ_t GetSenderId() const { return this->SenderId_; }
	/// 获取发送时间
	std::time_t GetTimestamp() const { return this->time_; }
	/// 获取发送者名称
	std::string GetSenderName() const { return this->SenderName_; }
	/// 获取消息链
	MessageChain GetMessageChain() const { return this->message_; }
	/// 是否含有消息id
	bool hasMessageId() const { return this->MessageId_.has_value() || this->ref_.has_value(); }

	/// 获取消息id
	std::optional<MessageId_t> GetMessageId() const 
	{ 
		return this->ref_ ? this->ref_->MessageId : this->MessageId_;
	}

	/// 设置发送者
	Node& SetSenderId(QQ_t SenderId)
	{
		this->SenderId_ = SenderId;
		return *this;
	}
	/// 设置发送时间戳
	Node& SetTimestamp(std::time_t time)
	{
		this->time_ = time;
		return *this;
	}
	/// 设置发送者名称
	Node& SetSenderName(const std::string& SenderName)
	{
		this->SenderName_ = SenderName;
		return *this;
	}
	/// 设置消息内容
	Node& SetMessageChain(const MessageChain& message)
	{
		this->MessageId_ = std::nullopt;
		this->message_ = message;
		return *this;
	}
	/// 设置消息内容
	Node& SetMessageChain(MessageChain&& message)
	{
		this->MessageId_ = std::nullopt;
		this->message_ = std::move(message);
		return *this;
	}
	/// 设置消息id，限定为当前会话内的消息
	Node& SetMessageId(MessageId_t MessageId)
	{
		this->MessageId_ = MessageId;
		return *this;
	}
	/// 设置消息来源，需要指定消息上下文
	Node& SetMessageRef(MessageId_t MessageId, UID_t target)
	{
		this->ref_ = MessageRef{MessageId, (int64_t)target};
		return *this;
	}

	struct Serializable;
};

} // namespace Mirai

#endif