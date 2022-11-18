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

#ifndef _MIRAI_MEMBER_PERMISSION_CHANGE_EVENT_HPP_
#define _MIRAI_MEMBER_PERMISSION_CHANGE_EVENT_HPP_

#include <string>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 群成员权限更改事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberPermissionChangeEvent::_member` | `GroupMember{}`
 * `MemberPermissionChangeEvent::_origin` | `PERMISSION::ENUM_END`
 * `MemberPermissionChangeEvent::_current` | `PERMISSION::ENUM_END`
 *
 * 该成员不是Bot自己
 */
class MemberPermissionChangeEvent : public EventBase
{
protected:
	GroupMember _member;
	PERMISSION _origin = PERMISSION::ENUM_END;
	PERMISSION _current = PERMISSION::ENUM_END;

	void Deserialize(const void *) final;
public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "MemberPermissionChangeEvent";

	/// 获取群成员资料
	GroupMember GetMember() const { return this->_member; }
	/// 获取更改前的权限
	PERMISSION GetOriginal() const { return this->_origin; }
	/// 获取当前权限
	PERMISSION GetCurrent() const { return this->_current; }
};

} // namespace Mirai


#endif