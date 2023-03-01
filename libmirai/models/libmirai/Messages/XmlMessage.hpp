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

#ifndef _MIRAI_XML_MESSAGE_HPP_
#define _MIRAI_XML_MESSAGE_HPP_


#include <string>
#include <utility>

#include "IMessage.hpp"

namespace Mirai
{

/**
 * @brief XML消息
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `XmlMessage::_content` | `""`
 */
class XmlMessage final : public IMessageImpl<XmlMessage>
{
	friend IMessageImpl<XmlMessage>;

protected:
	std::string _content;

	static constexpr MessageTypes _TYPE_ = MessageTypes::XML;
	static constexpr bool _SUPPORT_SEND_ = true;

	bool _isValid() const final { return !this->_content.empty(); }

public:
	XmlMessage() = default;
	XmlMessage(std::string content) : _content(std::move(content)) {}

	bool operator==(const XmlMessage& rhs) { return this->_content == rhs._content; }

	bool operator!=(const XmlMessage& rhs) { return !(*this == rhs); }

	/// 获取消息内容
	std::string GetContent() const { return this->_content; }

	/// 设置消息内容
	XmlMessage& SetContent(std::string content)
	{
		this->_content = std::move(content);
		return *this;
	}

	struct Serializable;
};

template<> struct GetType<XmlMessage::GetType()>
{
	using type = XmlMessage;
};

} // namespace Mirai


#endif