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

#ifndef _MIRAI_SOURCE_MESSAGE_HPP_
#define _MIRAI_SOURCE_MESSAGE_HPP_

#include <ctime>
#include <string>


#include <libmirai/Types/BasicTypes.hpp>

#include "MessageBase.hpp"

namespace Mirai
{

/**
 * @brief 来源消息
 * 
 * 用于标识消息的发送时间与id，仅可用于接收。
 *
 * Member Variable | Default Value
 * --------------- | -------------
 * `SourceMessage::_id` | `-1`
 * `SourceMessage::_timestamp` | `0`
 */
class SourceMessage : public MessageBase
{
protected:
	MessageId_t _id = -1;
	std::time_t _timestamp = 0;

	void Serialize(void*) const final;
	void Deserialize(const void*) final;

public:
	static constexpr MessageTypes _TYPE_ = MessageTypes::SOURCE;

	SourceMessage() : MessageBase(_TYPE_, false) {}

	std::unique_ptr<MessageBase> CloneUnique() const final { return std::make_unique<SourceMessage>(*this); }

	bool isValid() const final
	{
		return true;
	}

	/// 获取消息id
	MessageId_t GetMessageId() const { return this->_id; }
	/// 获取消息时间戳
	std::time_t GetTimestamp() const { return this->_timestamp; }
};

template <>
struct GetType<SourceMessage::_TYPE_>
{
	using type = SourceMessage;
};

} // namespace Mirai


#endif