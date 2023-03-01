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

#ifndef _MIRAI_PLAIN_MESSAGE_HPP_
#define _MIRAI_PLAIN_MESSAGE_HPP_

#include <string>
#include <utility>

#include "IMessage.hpp"

namespace Mirai
{

/**
 * @brief 文字消息
 *
 * Member Variable | Default Value
 * --------------- | -------------
 * `PlainMessage::_text` | `""`
 */
class PlainMessage final : public IMessageImpl<PlainMessage>
{
	friend IMessageImpl<PlainMessage>;

protected:
	std::string _text{};

	static constexpr MessageTypes _TYPE_ = MessageTypes::PLAIN;
	static constexpr bool _SUPPORT_SEND_ = true;

	bool _isValid() const final { return !this->_text.empty(); }

public:
	PlainMessage() = default;
	PlainMessage(std::string text) : _text(std::move(text)) {}

	bool operator==(const PlainMessage& rhs) { return this->_text == rhs._text; }

	bool operator!=(const PlainMessage& rhs) { return !(*this == rhs); }

	/// 获取文字消息
	std::string GetText() const { return this->_text; }

	/// 设置文字消息
	PlainMessage& SetText(std::string text)
	{
		this->_text = std::move(text);
		return *this;
	}

	struct Serializable;
};

template<> struct GetType<PlainMessage::GetType()>
{
	using type = PlainMessage;
};

} // namespace Mirai


#endif