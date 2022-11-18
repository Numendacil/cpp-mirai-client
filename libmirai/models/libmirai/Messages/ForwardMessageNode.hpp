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

#ifndef _MIRAI_FORWARD_MESSAGE_NODE_HPP_
#define _MIRAI_FORWARD_MESSAGE_NODE_HPP_

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
* `Node::_SenderId` | `0_qq`
* `Node::_time` | `0`
* `Node::_SenderName` | `""`
* `Node::_message` | `MessageChain{}`
* `Node::_MessageId` | `std::nullopt`
*/
class ForwardMessage::Node
{
protected:
	QQ_t _SenderId{};
	std::time_t _time = 0;
	std::string _SenderName{};
	MessageChain _message{};
	std::optional<MessageId_t> _MessageId = std::nullopt;

public:
	Node() = default;

	bool isValid() const
	{
		return this->_MessageId.has_value() || (!this->_SenderName.empty() && this->_message.isValid());
	}

	/// 获取发送者QQ
	QQ_t GetSenderId() const { return this->_SenderId; }
	/// 获取发送时间
	std::time_t GetTimestamp() const { return this->_time; }
	/// 获取发送者名称
	std::string GetSenderName() const { return this->_SenderName; }
	/// 获取消息链
	MessageChain GetMessageChain() const { return this->_message; }
	/// 是否含有消息id
	bool hasMessageId() const { return this->_MessageId.has_value(); }
	/// 获取消息id，不存在时返回 `0`
	MessageId_t GetMessageId() const { return this->_MessageId.value_or(0); }

	/// 设置发送者
	Node& SetSenderId(QQ_t SenderId)
	{
		this->_SenderId = SenderId;
		return *this;
	}
	/// 设置发送时间戳
	Node& SetTimestamp(std::time_t time)
	{
		this->_time = time;
		return *this;
	}
	/// 设置发送者名称
	Node& SetSenderName(const std::string& SenderName)
	{
		this->_SenderName = SenderName;
		return *this;
	}
	/// 设置消息内容
	Node& SetMessageChain(const MessageChain& message)
	{
		this->_MessageId = std::nullopt;
		this->_message = message;
		return *this;
	}
	/// 设置消息内容
	Node& SetMessageChain(MessageChain&& message)
	{
		this->_MessageId = std::nullopt;
		this->_message = std::move(message);
		return *this;
	}
	/// 设置消息id
	Node& SetMessageId(std::optional<MessageId_t> MessageId)
	{
		this->_MessageId = MessageId;
		return *this;
	}

	struct Serializable;;
};

}

#endif