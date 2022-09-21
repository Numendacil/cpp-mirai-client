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

#ifndef _MIRAI_GROUP_MUTE_ALL_EVENT_HPP_
#define _MIRAI_GROUP_MUTE_ALL_EVENT_HPP_

#include <optional>
#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 群聊开启/关闭全体禁言事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `BotGroupPermissionChangeEvent::_group` | `Group{}`
 * `BotGroupPermissionChangeEvent::_origin` | `false`
 * `BotGroupPermissionChangeEvent::_current` | `false`
 * `BotGroupPermissionChangeEvent::_operator` | `std::nullopt`
 */
class GroupMuteAllEvent : public EventBase
{
protected:
	Group _group;
	bool _origin = false;
	bool _current = false;
	std::optional<GroupMember> _operator = std::nullopt;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "GroupMuteAllEvent";

	std::string_view GetType() const override { return _TYPE_; }

	// GroupMuteAllEvent* Clone() const override
	// {
	//	return new GroupMuteAllEvent(*this);
	// }

	void FromJson(const nlohmann::json& data) override;

	/// 获取群聊信息
	Group GetGroup() const { return this->_group; }
	/// 原本是否处于全员禁言状态
	bool GetOriginal() const { return this->_origin; }
	/// 现在是否处于全员禁言
	bool GetCurrent() const { return this->_current; }
	/// 获取操作员信息，为Bot时返回 `std::nullopt`
	std::optional<GroupMember> GetOperator() const { return this->_operator; }
};

} // namespace Mirai


#endif