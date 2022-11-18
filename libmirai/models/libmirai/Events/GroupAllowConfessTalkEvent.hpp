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

#ifndef _MIRAI_GROUP_ALLOW_CONFESS_TALK_EVENT_HPP_
#define _MIRAI_GROUP_ALLOW_CONFESS_TALK_EVENT_HPP_

#include <string>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

// TODO: add a base class for all GroupSettingChangeEvent

/**
 * @brief 群聊开启/关闭坦白说事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `GroupAllowConfessTalkEvent::_group` | `Group{}`
 * `GroupAllowConfessTalkEvent::_origin` | `false`
 * `GroupAllowConfessTalkEvent::_current` | `false`
 * `GroupAllowConfessTalkEvent::_ByBot` | `false`
 */
class GroupAllowConfessTalkEvent : public EventBase
{
protected:
	Group _group;
	bool _origin = false;
	bool _current = false;
	bool _ByBot = false; // 无法获得操作员

	void Deserialize(const void *) final;
public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "GroupAllowConfessTalkEvent";

	/// 获取群聊信息
	Group GetGroup() const { return this->_group; }
	/// 原本坦白说是否开启
	bool GetOriginal() const { return this->_origin; }
	/// 现在坦白说是否开启
	bool GetCurrent() const { return this->_current; }
	/// 操作员是否为Bot自己（无法获得具体操作员信息）
	bool isByBot() const { return this->_ByBot; }
};

} // namespace Mirai


#endif