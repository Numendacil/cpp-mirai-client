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

#ifndef _MIRAI_GROUP_NAME_CHANGE_EVENT_HPP_
#define _MIRAI_GROUP_NAME_CHANGE_EVENT_HPP_

#include <optional>
#include <string>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

// TODO: add a base class for all GroupSettingChangeEvent

/**
 * @brief 群聊名称更改事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `GroupNameChangeEvent::_group` | `Group{}`
 * `GroupNameChangeEvent::_origin` | `""`
 * `GroupNameChangeEvent::_current` | `""`
 * `GroupNameChangeEvent::_operator` | `std::nullopt`
 */
class GroupNameChangeEvent : public EventBase
{
protected:
	Group _group;
	std::string _origin;
	std::string _current;
	std::optional<GroupMember> _operator = std::nullopt;

	void Deserialize(const void*) final;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "GroupNameChangeEvent";

	/// 获取群聊信息
	Group GetGroup() const { return this->_group; }
	/// 获取更改前的群聊名称
	std::string GetOriginal() const { return this->_origin; }
	/// 获取当前的群聊名称
	std::string GetCurrent() const { return this->_current; }
	/// 获取操作员信息，为Bot时返回 `std::nullopt`
	std::optional<GroupMember> GetOperator() const { return this->_operator; }
};

} // namespace Mirai


#endif