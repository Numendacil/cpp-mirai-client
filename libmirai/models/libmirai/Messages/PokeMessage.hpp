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

#ifndef _MIRAI_POKE_MESSAGE_HPP_
#define _MIRAI_POKE_MESSAGE_HPP_

#include <array>
#include <string>

#include <libmirai/Types/BasicTypes.hpp>

#include "MessageBase.hpp"

namespace Mirai
{

/**
 * @brief 戳一戳消息
 *
 * 原PC版窗口抖动消息，与头像戳一戳消息不同
 *
 * Member Variable | Default Value
 * --------------- | -------------
 * `PokeMessage::_kind` | `PokeType::ENUM_END`
 */
class PokeMessage : public MessageBase
{
protected:
	PokeType _kind = PokeType::ENUM_END;

	void Serialize(void*) const final;
	void Deserialize(const void*) final;

public:
	static constexpr MessageTypes _TYPE_ = MessageTypes::POKE;

	PokeMessage() : MessageBase(_TYPE_) {}
	PokeMessage(PokeType kind) : _kind(kind), MessageBase(_TYPE_) {}

	std::unique_ptr<MessageBase> CloneUnique() const final { return std::make_unique<PokeMessage>(*this); }

	bool isValid() const final { return this->_kind != PokeType::ENUM_END; }

	bool operator==(const PokeMessage& rhs) { return this->_kind == rhs._kind; }

	bool operator!=(const PokeMessage& rhs) { return !(*this == rhs); }

	/// 获取戳一戳类型
	PokeType GetPokeKind() const { return this->_kind; }

	/// 设置戳一戳类型
	PokeMessage& SetPokeKind(PokeType kind)
	{
		this->_kind = kind;
		return *this;
	}
};

template<> struct GetType<PokeMessage::_TYPE_>
{
	using type = PokeMessage;
};

} // namespace Mirai


#endif