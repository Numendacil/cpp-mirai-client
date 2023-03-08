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

#ifndef MIRAI_SOURCE_MESSAGE_HPP_
#define MIRAI_SOURCE_MESSAGE_HPP_

#include <ctime>
#include <string>

#include <libmirai/Types/BasicTypes.hpp>

#include "IMessage.hpp"

namespace Mirai
{

/**
 * @brief 来源消息
 * 
 * 用于标识消息的发送时间与id，仅可用于接收。
 *
 * Member Variable | Default Value
 * --------------- | -------------
 * `SourceMessage::id_` | `-1`
 * `SourceMessage::timestamp_` | `0`
 */
class SourceMessage final : public IMessageImpl<SourceMessage>
{
	friend IMessageImpl<SourceMessage>;

protected:
	MessageId_t id_ = -1;
	std::time_t timestamp_ = 0;

	static constexpr MessageTypes TYPE_ = MessageTypes::SOURCE;
	static constexpr bool SUPPORT_SEND_ = false;

	bool isValid_() const final { return true; }

public:
	SourceMessage() = default;

	/// 获取消息id
	MessageId_t GetMessageId() const { return this->id_; }
	/// 获取消息时间戳
	std::time_t GetTimestamp() const { return this->timestamp_; }

	struct Serializable;
};

template<> struct GetType<SourceMessage::GetType()>
{
	using type = SourceMessage;
};

} // namespace Mirai


#endif