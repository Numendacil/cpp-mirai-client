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


#include "MessageBase.hpp"

namespace Mirai
{

/**
 * @brief XML消息
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `XmlMessage::_content` | `""`
 */
class XmlMessage : public MessageBase
{

protected:
	std::string _content;

	void Serialize(void*) const final;
	void Deserialize(const void*) final;

public:
	static constexpr MessageTypes _TYPE_ = MessageTypes::XML;

	XmlMessage() : MessageBase(_TYPE_) {}
	XmlMessage(std::string content) : _content(std::move(content)), MessageBase(_TYPE_) {}

	std::unique_ptr<MessageBase> CloneUnique() const final { return std::make_unique<XmlMessage>(*this); }

	bool isValid() const final
	{
		return !this->_content.empty();
	}


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
};

template <>
struct GetType<XmlMessage::_TYPE_>
{
	using type = XmlMessage;
};

} // namespace Mirai


#endif