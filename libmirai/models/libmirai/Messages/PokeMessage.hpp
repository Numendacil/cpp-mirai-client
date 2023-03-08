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

#ifndef MIRAI_POKE_MESSAGE_HPP_
#define MIRAI_POKE_MESSAGE_HPP_

#include <array>
#include <string>

#include <libmirai/Types/BasicTypes.hpp>

#include "IMessage.hpp"

namespace Mirai
{

/**
 * @brief 戳一戳消息
 *
 * 原PC版窗口抖动消息，与头像戳一戳消息不同
 *
 * Member Variable | Default Value
 * --------------- | -------------
 * `PokeMessage::kind_` | `PokeType::ENUM_END`
 */
class PokeMessage final : public IMessageImpl<PokeMessage>
{
	friend IMessageImpl<PokeMessage>;

protected:
	PokeType kind_ = PokeType::ENUM_END;

	static constexpr MessageTypes TYPE_ = MessageTypes::POKE;
	static constexpr bool SUPPORT_SEND_ = true;

	bool isValid_() const final { return this->kind_ != PokeType::ENUM_END; }

public:
	PokeMessage() = default;
	PokeMessage(PokeType kind) : kind_(kind) {}

	bool operator==(const PokeMessage& rhs) { return this->kind_ == rhs.kind_; }

	bool operator!=(const PokeMessage& rhs) { return !(*this == rhs); }

	/// 获取戳一戳类型
	PokeType GetPokeKind() const { return this->kind_; }

	/// 设置戳一戳类型
	PokeMessage& SetPokeKind(PokeType kind)
	{
		this->kind_ = kind;
		return *this;
	}

	struct Serializable;
};

template<> struct GetType<PokeMessage::GetType()>
{
	using type = PokeMessage;
};

} // namespace Mirai


#endif