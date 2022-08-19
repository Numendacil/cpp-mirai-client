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

#include <nlohmann/json_fwd.hpp>

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
	MessageId_t _id;
	std::time_t _timestamp;

public:
	SourceMessage() : _id(-1), _timestamp(0) {}
	SourceMessage(const SourceMessage&) = default;
	SourceMessage& operator=(const SourceMessage&) = default;
	SourceMessage(SourceMessage&&) noexcept = default;
	SourceMessage& operator=(SourceMessage&&) noexcept = default;

	static constexpr std::string_view _TYPE_ = "Source";

	virtual std::string_view GetType() const override { return _TYPE_; }

	virtual SourceMessage* Clone() const override { return new SourceMessage(*this); }

	virtual bool isValid() const override;

	virtual void FromJson(const nlohmann::json& data) override;
	virtual nlohmann::json ToJson() const override;

	/// 获取消息id
	MessageId_t GetMessageId() const { return this->_id; }
	/// 获取消息时间戳
	std::time_t GetTimestamp() const { return this->_timestamp; }
};

} // namespace Mirai


#endif