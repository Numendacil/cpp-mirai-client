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

#ifndef _MIRAI_MIRAICODE_MESSAGE_HPP_
#define _MIRAI_MIRAICODE_MESSAGE_HPP_

#include <string>


#include "MessageBase.hpp"

/**
 * @brief  Mirai码消息
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MiraiCodeMessage::_code` | `""`
 */
namespace Mirai
{

class MiraiCodeMessage : public MessageBase
{
protected:
	std::string _code{};

	void Serialize(void*) const final;
	void Deserialize(const void*) final;

public:
	static constexpr MessageTypes _TYPE_ = MessageTypes::MIRAI_CODE;

	MiraiCodeMessage() : MessageBase(_TYPE_) {}

	std::unique_ptr<MessageBase> CloneUnique() const final { return std::make_unique<MiraiCodeMessage>(*this); }

	bool isValid() const final
	{
		return !this->_code.empty();
	}


	bool operator==(const MiraiCodeMessage& rhs) { return this->_code == rhs._code; }

	bool operator!=(const MiraiCodeMessage& rhs) { return !(*this == rhs); }

	/// 获取mirai码
	std::string GetCode() const { return this->_code; }

	/// 设置mirai码
	MiraiCodeMessage& SetCode(const std::string& code)
	{
		this->_code = code;
		return *this;
	}
};

template <>
struct GetType<MiraiCodeMessage::_TYPE_>
{
	using type = MiraiCodeMessage;
};

} // namespace Mirai

#endif