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

#ifndef _MIRAI_MEMBER_SPECIAL_TITLE_CHANGE_EVENT_HPP_
#define _MIRAI_MEMBER_SPECIAL_TITLE_CHANGE_EVENT_HPP_

#include <string>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 群成员头衔更改事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberSpecialTitleChangeEvent::_member` | `GroupMember{}`
 * `MemberSpecialTitleChangeEvent::_origin` | `""`
 * `MemberSpecialTitleChangeEvent::_current` | `""`
 */
class MemberSpecialTitleChangeEvent : public EventBase
{
protected:
	GroupMember _member;
	std::string _origin;
	std::string _current;

	void Deserialize(const void*) final;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "MemberSpecialTitleChangeEvent";

	/// 获取群成员资料
	GroupMember GetMember() const { return this->_member; }
	/// 获取更改前的群头衔
	std::string GetOriginal() const { return this->_origin; }
	/// 获取当前群头衔
	std::string GetCurrent() const { return this->_current; }
};

} // namespace Mirai


#endif