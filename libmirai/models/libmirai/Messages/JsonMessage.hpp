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

#ifndef MIRAI_JSON_MESSAGE_HPP_
#define MIRAI_JSON_MESSAGE_HPP_

#include <string>

#include "IMessage.hpp"

namespace Mirai
{

/**
 * @brief JSON消息
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `JsonMessage::content_` | `""`
 */
class JsonMessage final : public IMessageImpl<JsonMessage>
{
	friend IMessageImpl<JsonMessage>;

protected:
	std::string content_{};

	static constexpr MessageTypes TYPE_ = MessageTypes::JSON;
	static constexpr bool SUPPORT_SEND_ = true;

	bool isValid_() const final { return !this->content_.empty(); }

public:
	JsonMessage() = default;
	JsonMessage(std::string content) : content_(std::move(content)) {}

	bool operator==(const JsonMessage& rhs) { return this->content_ == rhs.content_; }

	bool operator!=(const JsonMessage& rhs) { return !(*this == rhs); }

	/// 获取消息内容
	std::string GetContent() const { return this->content_; }

	/// 设置消息内容
	JsonMessage& SetContent(std::string content)
	{
		this->content_ = std::move(content);
		return *this;
	}

	struct Serializable;
};

template<> struct GetType<JsonMessage::GetType()>
{
	using type = JsonMessage;
};

} // namespace Mirai


#endif