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

#ifndef _MIRAI_FRIEND_MESSAGE_EVENT_HPP_
#define _MIRAI_FRIEND_MESSAGE_EVENT_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Messages/MessageChain.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 好友消息事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `FriendMessageEvent::_sender` | `User{}`
 * `FriendMessageEvent::_message` | `MessageChain{}`
 */
class FriendMessageEvent : public EventBase
{

protected:
	User _sender;
	MessageChain _message;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "FriendMessage";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual FriendMessageEvent* Clone() const override
	// {
	//	return new FriendMessageEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取发送者资料
	User GetSender() const { return this->_sender; }
	/// 获取消息内容
	MessageChain GetMessage() const { return this->_message; }

	// TODO: add helper methods for quick reply
};

} // namespace Mirai


#endif