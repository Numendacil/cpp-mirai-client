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

#ifndef _MIRAI_MESSAGE_INTERFACE_HPP_
#define _MIRAI_MESSAGE_INTERFACE_HPP_


#include <memory>
#include <string>
#include <type_traits>

#include <libmirai/Types/MessageTypes.hpp>

namespace Mirai
{

/**
 * @brief Common interface for all message types
 * 
 */
class IMessage
{
protected:
	IMessage() = default;

	virtual MessageTypes _GetType() const = 0;
	virtual bool _isSendSupported() const = 0;
	virtual bool _isValid() const = 0;

public:
	virtual ~IMessage() = default;

	virtual std::unique_ptr<IMessage> clone() const = 0;

	/**
	 * @brief Return the type of the class
	 * 
	 * @return `MessageTypes`
	 */
	MessageTypes type() const { return this->_GetType(); }

	/**
	 * @brief 检查消息是否可以用于发送
	 * 
	 * 部分消息仅支持接收
	 * @return `bool`
	 */
	bool allowSend() const { return this->_isSendSupported(); }

	/**
	 * @brief 检查消息是否有效
	 * 
	 * 发送无效消息( `valid() = false` )会导致mirai-api-http返回400或500错误，并抛出异常。
	 * 检测的内容靠测试经验和mirai-api-http源码确定， `valid() = true` 不保证一定能发送成功。
	 * @return `bool`
	 */
	bool valid() const { return this->_isValid(); }  
};

/**
 * @brief CRTP helper layer
 * 
 */
template <class Message>
class IMessageImpl : public IMessage
{
protected:
	// Better not to set default value to avoid errors
	// static constexpr bool _SUPPORT_SEND_ = true;
	// static constexpr MessageTypes _TYPE_ = MessageTypes::ENUM_END;

	IMessageImpl() = default;

	MessageTypes _GetType() const override { return Message::_TYPE_; }
	
	bool _isSendSupported() const override { return Message::_SUPPORT_SEND_; }

private:
	Message& _top() { return *static_cast<Message*>(this); }
	const Message& _top() const { return *static_cast<const Message*>(this); }

public:
	static constexpr MessageTypes GetType() { return Message::_TYPE_; }

	static constexpr bool isSendSupported() { return Message::_SUPPORT_SEND_; }

	std::unique_ptr<IMessage> clone() const override
	{
		return std::make_unique<Message>(_top());
	}
};

} // namespace Mirai


#endif