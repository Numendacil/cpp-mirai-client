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

#ifndef _MIRAI_BOT_GROUP_PERMISSION_CHANGED_EVENT_HPP_
#define _MIRAI_BOT_GROUP_PERMISSION_CHANGED_EVENT_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief Bot群聊权限改变事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `BotGroupPermissionChangeEvent::_group` | `Group{}`
 * `BotGroupPermissionChangeEvent::_origin` | `PERMISSION::UNKNOWN`
 * `BotGroupPermissionChangeEvent::_current` | `PERMISSION::UNKNOWN`
 */
class BotGroupPermissionChangeEvent : public EventBase
{
protected:
	Group _group;
	PERMISSION _origin = PERMISSION::UNKNOWN;
	PERMISSION _current = PERMISSION::UNKNOWN;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "BotGroupPermissionChangeEvent";

	std::string_view GetType() const override { return _TYPE_; }

	// BotGroupPermissionChangeEvent* Clone() const override
	// {
	//	return new BotGroupPermissionChangeEvent(*this);
	// }

	void FromJson(const nlohmann::json& data) override;

	/// 获取群聊信息
	Group GetGroup() const { return this->_group; }
	/// 获取Bot更改前的权限
	PERMISSION GetOriginal() const { return this->_origin; }
	/// 获取Bot当前权限
	PERMISSION GetCurrent() const { return this->_current; }
};

} // namespace Mirai


#endif