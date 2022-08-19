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

#include <nlohmann/json_fwd.hpp>

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

public:
	XmlMessage() {}
	XmlMessage(const std::string& content) : _content(content) {}
	XmlMessage(const XmlMessage&) = default;
	XmlMessage& operator=(const XmlMessage&) = default;
	XmlMessage(XmlMessage&&) noexcept = default;
	XmlMessage& operator=(XmlMessage&&) noexcept = default;


	static constexpr std::string_view _TYPE_ = "Xml";

	virtual std::string_view GetType() const override { return _TYPE_; }

	virtual XmlMessage* Clone() const override { return new XmlMessage(*this); }

	virtual void FromJson(const nlohmann::json& data) override;
	virtual nlohmann::json ToJson() const override;

	virtual bool isValid() const override;


	bool operator==(const XmlMessage& rhs) { return this->_content == rhs._content; }

	bool operator!=(const XmlMessage& rhs) { return !(*this == rhs); }

	/// 获取消息内容
	std::string GetContent() const { return this->_content; }

	/// 设置消息内容
	XmlMessage& SetContent(const std::string& content)
	{
		this->_content = content;
		return *this;
	}
};

} // namespace Mirai


#endif