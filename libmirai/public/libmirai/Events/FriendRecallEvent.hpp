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

#ifndef _MIRAI_FRIEND_RECALL_EVENT_HPP_
#define _MIRAI_FRIEND_RECALL_EVENT_HPP_

#include <ctime>
#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 好友消息撤回事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `FriendRecallEvent::_AuthorId` | `0_qq`
 * `FriendRecallEvent::_MessageId` | `0`
 * `FriendRecallEvent::_time` | `0`
 * `FriendRecallEvent::_operator` | `0_qq`
 */
class FriendRecallEvent : public EventBase
{
protected:
	QQ_t _AuthorId;
	MessageId_t _MessageId = 0;
	std::time_t _time = 0;
	QQ_t _operator;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "FriendRecallEvent";

	std::string_view GetType() const override { return _TYPE_; }

	// FriendRecallEvent* Clone() const override
	// {
	//	return new FriendRecallEvent(*this);
	// }

	void FromJson(const nlohmann::json& data) override;

	/// 获取被撤回消息的发送者QQ
	QQ_t GetSender() const { return this->_AuthorId; }
	/// 获取被撤回消息的消息id
	MessageId_t GetMessageId() const { return this->_MessageId; }
	/// 获取被撤回消息的发送时间
	std::time_t GetSendTime() const { return this->_time; }
	/// 获取操作员QQ
	QQ_t GetOperator() const { return this->_operator; }
};

} // namespace Mirai


#endif