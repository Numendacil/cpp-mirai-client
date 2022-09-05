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

#include <nlohmann/json_fwd.hpp>
#include <utility>

#include "MessageBase.hpp"

namespace Mirai
{

/**
 * @brief 文字消息
 *
 * Member Variable | Default Value
 * --------------- | -------------
 * `PlainMessage::_text` | `""`
 */
class PlainMessage : public MessageBase
{
protected:
	std::string _text{};

public:
	PlainMessage() = default;
	PlainMessage(std::string text) : _text(std::move(text)) {}

	static constexpr std::string_view _TYPE_ = "Plain";

	std::string_view GetType() const override { return _TYPE_; }

	std::unique_ptr<MessageBase> CloneUnique() const override { return std::make_unique<PlainMessage>(*this); }

	bool isValid() const override;

	void FromJson(const nlohmann::json& data) override;
	nlohmann::json ToJson() const override;


	bool operator==(const PlainMessage& rhs) { return this->_text == rhs._text; }

	bool operator!=(const PlainMessage& rhs) { return !(*this == rhs); }

	/// 获取文字消息
	std::string GetText() const { return this->_text; }

	/// 设置文字消息
	PlainMessage& SetText(const std::string& text)
	{
		this->_text = text;
		return *this;
	}
};

} // namespace Mirai


#endif