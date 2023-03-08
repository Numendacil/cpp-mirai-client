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

#ifndef MIRAI_ATALL_MESSAGE_HPP_
#define MIRAI_ATALL_MESSAGE_HPP_

#include <string>

#include "IMessage.hpp"

namespace Mirai
{

/**
 * @brief At全体成员消息
 * 
 */
class AtAllMessage final : public IMessageImpl<AtAllMessage>
{
	friend class IMessageImpl<AtAllMessage>;

private:
	bool isValid_() const final { return true; }

	static constexpr MessageTypes TYPE_ = MessageTypes::AT_ALL;
	static constexpr bool SUPPORT_SEND_ = true;

public:
	struct Serializable;
};

template<> struct GetType<AtAllMessage::GetType()>
{
	using type = AtAllMessage;
};

} // namespace Mirai


#endif