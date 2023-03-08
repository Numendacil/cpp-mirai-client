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

#ifndef MIRAI_APP_MESSAGE_HPP_
#define MIRAI_APP_MESSAGE_HPP_

#include <string>
#include <utility>

#include "IMessage.hpp"

namespace Mirai
{

/**
 * @brief 小程序消息
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `AppMessage::content_` | `""`
 */
class AppMessage final : public IMessageImpl<AppMessage>
{
	friend IMessageImpl<AppMessage>;

private:
	std::string content_{};

	bool isValid_() const final { return !this->content_.empty(); }

	static constexpr MessageTypes TYPE_ = MessageTypes::APP;
	static constexpr bool SUPPORT_SEND_ = true;

public:
	AppMessage() = default;
	AppMessage(std::string content) : content_(std::move(content)) {};

	bool operator==(const AppMessage& rhs) { return this->content_ == rhs.content_; }

	bool operator!=(const AppMessage& rhs) { return !(*this == rhs); }

	/// 获取App消息内容
	std::string GetContent() const { return this->content_; }

	/// 设置App消息内容
	AppMessage& SetContent(std::string content)
	{
		this->content_ = std::move(content);
		return *this;
	}

	struct Serializable;
};

template<> struct GetType<AppMessage::GetType()>
{
	using type = AppMessage;
};

} // namespace Mirai


#endif