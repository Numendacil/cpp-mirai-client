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

#ifndef _MIRAI_QUOTE_MESSAGE_HPP_
#define _MIRAI_QUOTE_MESSAGE_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "MessageBase.hpp"
#include "MessageChain.hpp"

namespace Mirai
{

/**
 * @brief 引用回复消息
 *
 * 仅用于接收，发送时将会被无视。发送引用回复消息请在消息发送函数中设置 `QuoteId`
 *
 * Member Variable | Default Value
 * --------------- | -------------
 * `QuoteMessage::_QuoteId` | `-1`
 * `QuoteMessage::_GroupId` | `0_gid`
 * `QuoteMessage::_SenderId` | `0_qq`
 * `QuoteMessage::_TargetId` | `0`
 * `QuoteMessage::_origin` | `MessageChain{}`
 */
class QuoteMessage : public MessageBase
{

protected:
	MessageId_t _QuoteId = -1;
	GID_t _GroupId{};
	QQ_t _SenderId{};
	int64_t _TargetId = 0;
	MessageChain _origin{};

public:
	QuoteMessage() = default;

	static constexpr std::string_view _TYPE_ = "Quote";

	std::string_view GetType() const override { return _TYPE_; }

	std::unique_ptr<MessageBase> CloneUnique() const override { return std::make_unique<QuoteMessage>(*this); }

	void FromJson(const nlohmann::json& data) override;
	nlohmann::json ToJson() const override;

	bool isValid() const override;

	/// 获取被引用消息id
	MessageId_t GetQuoteId() const { return this->_QuoteId; }
	/// 获取被引用消息所在群聊id，若为好友消息则为 `0`
	GID_t GetGroupId() const { return this->_GroupId; }
	/// 获取引用消息的发送者QQ
	QQ_t GetSenderId() const { return this->_SenderId; }
	/// 获取引用消息的接收者QQ，仅当好友消息时有效
	QQ_t GetTargetId() const
	{
		if (this->_GroupId == (GID_t)0) return QQ_t(this->_TargetId);
		else
			return QQ_t(0);
	}
	/// 获取被引用消息
	MessageChain GetOriginMessage() const { return this->_origin; }
};

} // namespace Mirai


#endif