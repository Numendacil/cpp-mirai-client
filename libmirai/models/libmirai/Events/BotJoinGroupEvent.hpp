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

#ifndef _MIRAI_BOT_JOIN_GROUP_EVENT_HPP_
#define _MIRAI_BOT_JOIN_GROUP_EVENT_HPP_

#include <optional>
#include <string>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief Bot加入群聊事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `BotJoinGroupEvent::_group` | `Group{}`
 * `BotJoinGroupEvent::_inviter` | `std::nullopt`
 */
class BotJoinGroupEvent : public EventBase
{
protected:
	Group _group;
	std::optional<GroupMember> _inviter = std::nullopt;

	void Deserialize(const void*) final;

public:
	using EventBase::EventBase;

	static constexpr std::string_view _TYPE_ = "BotJoinGroupEvent";

	/// 获取群聊信息
	Group GetGroup() const { return this->_group; }
	/// 获取邀请人信息，若无则返回 `std::nullopt`
	std::optional<GroupMember> GetInviter() const { return this->_inviter; }
};

} // namespace Mirai


#endif