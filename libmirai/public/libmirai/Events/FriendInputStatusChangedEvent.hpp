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

#ifndef _MIRAI_FRIEND_INPUT_STATUS_CHANGED_EVENT_HPP_
#define _MIRAI_FRIEND_INPUT_STATUS_CHANGED_EVENT_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 好友输入状态改变事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `FriendInputStatusChangedEvent::_friend` | `User{}`
 * `FriendInputStatusChangedEvent::_inputting` | `false`
 */
class FriendInputStatusChangedEvent : public EventBase
{
protected:
	User _friend;
	bool _inputting = false;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "FriendInputStatusChangedEvent";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual FriendInputStatusChangedEvent* Clone() const override
	// {
	//	return new FriendInputStatusChangedEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取好友信息
	User GetFriend() const { return this->_friend; }
	/// 是否正在输入
	bool isInputting() const { return this->_inputting; }
};

} // namespace Mirai


#endif