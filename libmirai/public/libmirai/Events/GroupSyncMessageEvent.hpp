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

#ifndef _MIRAI_GROUP_SYNC_MESSAGE_EVENT_HPP_
#define _MIRAI_GROUP_SYNC_MESSAGE_EVENT_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Messages/MessageChain.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 群聊消息同步事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `GroupSyncMessageEvent::_subject` | `Group{}`
 * `GroupSyncMessageEvent::_message` | `MessageChain{}`
 */
class GroupSyncMessageEvent : public EventBase
{

protected:
	Group _subject;
	MessageChain _message;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "GroupSyncMessage";

	std::string_view GetType() const override { return _TYPE_; }

	// GroupSyncMessageEvent* Clone() const override
	// {
	//	return new GroupSyncMessageEvent(*this);
	// }

	void FromJson(const nlohmann::json& data) override;

	/// 获取目标群聊信息
	Group GetGroup() const { return this->_subject; }
	/// 获取消息内容
	MessageChain GetMessage() const { return this->_message; }
};

} // namespace Mirai


#endif