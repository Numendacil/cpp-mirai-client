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

#include <string>
#include <optional>
#include <nlohmann/json_fwd.hpp>

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

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "BotJoinGroupEvent";

	std::string_view GetType() const override
	{
		return _TYPE_;
	}

	// BotJoinGroupEvent* Clone() const override
	// {
	//	return new BotJoinGroupEvent(*this);
	// }

	void FromJson(const nlohmann::json& data) override;

	/// 获取群聊信息
	Group GetGroup() const { return this->_group; }
	/// 获取邀请人信息，若无则返回 `std::nullopt`
	std::optional<GroupMember> GetInviter() const { return this->_inviter; }
};

}


#endif