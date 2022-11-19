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

#ifndef _MIRAI_MESSAGE_BASE_HPP_
#define _MIRAI_MESSAGE_BASE_HPP_


#include <memory>
#include <string>

#include <libmirai/Types/MessageTypes.hpp>

namespace Mirai
{

// TODO: Consider switching to typeid?

/**
 * @brief Base class for all message types
 * 
 * 所有有效的派生类均需要定义 `_TYPE_`，对应于mirai-api-http传来的JSON消息中的"type"属性
 */
class MessageBase
{
protected:
	MessageTypes _type;
	bool _SupportSend;

	MessageBase(MessageTypes type, bool SupportSend = true) : _type(type), _SupportSend(SupportSend) {}

	virtual void Serialize(void*) const = 0;
	virtual void Deserialize(const void*) = 0;

public:
	// static constexpr MessageTypes _TYPE_ = MessageTypes::"MessageBase";

	/**
	 * @brief Return the type of the class
	 * 
	 * Used for RTTI and message parsing, it should always return
	 * the value of the static `_TYPE_` member
	 */
	MessageTypes GetType() const { return this->_type; }

	/**
	 * @brief 检查消息是否可以用于发送
	 * 
	 * 部分消息仅支持接收
	 * @return `bool`
	 */
	bool isSendSupported() const { return this->_SupportSend; }

	/// Clone the class, used for copying polymorphic objects
	virtual std::unique_ptr<MessageBase> CloneUnique() const = 0;

	/**
	 * @brief 检查消息是否有效
	 * 
	 * 发送无效消息( `isValid() = false` )会导致mirai-api-http返回400或500错误，并抛出异常。
	 * 检测的内容靠测试经验和mirai-api-http源码确定， `isValid() = true` 不保证一定能发送成功。
	 * @return `bool`
	 */
	virtual bool isValid() const = 0;

	virtual ~MessageBase() = default;


	struct Serializable;
	;
};

} // namespace Mirai


#endif