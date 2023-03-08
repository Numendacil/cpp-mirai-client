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

#ifndef MIRAI_DICE_MESSAGE_HPP_
#define MIRAI_DICE_MESSAGE_HPP_

#include <string>

#include "IMessage.hpp"

namespace Mirai
{

/**
 * @brief 骰子消息
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `DiceMessage::value_` | `-1`
 */
class DiceMessage final : public IMessageImpl<DiceMessage>
{
	friend IMessageImpl<DiceMessage>;

protected:
	int value_ = -1;

	bool isValid_() const final { return this->value_ > 0 && this->value_ <= 6; }

	static constexpr MessageTypes TYPE_ = MessageTypes::DICE;
	static constexpr bool SUPPORT_SEND_ = true;

public:
	DiceMessage() = default;
	DiceMessage(int value)
	{
		if (value > 0 && value <= 6) this->value_ = value;
		else
			this->value_ = -1;
	}

	bool operator==(const DiceMessage& rhs) { return this->value_ == rhs.value_; }

	bool operator!=(const DiceMessage& rhs) { return !(*this == rhs); }

	/// 获取骰子点数
	int GetValue() const { return this->value_; }

	/// 设置骰子点数，不在1~6之间的值将不会被设置
	DiceMessage& SetValue(int value)
	{
		if (value > 0 && value <= 6) this->value_ = value;
		return *this;
	}

	struct Serializable;
};

template<> struct GetType<DiceMessage::GetType()>
{
	using type = DiceMessage;
};

} // namespace Mirai


#endif