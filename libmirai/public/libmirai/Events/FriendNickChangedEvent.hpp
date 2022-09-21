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

#ifndef _MIRAI_FRIEND_NICK_CHANGED_EVENT_HPP_
#define _MIRAI_FRIEND_NICK_CHANGED_EVENT_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 好友昵称改变事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `FriendNickChangedEvent::_friend` | `User{}`
 * `FriendNickChangedEvent::_from` | `""`
 * `FriendNickChangedEvent::_to` | `""`
 */
class FriendNickChangedEvent : public EventBase
{
protected:
	User _friend;
	std::string _from;
	std::string _to;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "FriendNickChangedEvent";

	std::string_view GetType() const override { return _TYPE_; }

	// FriendNickChangedEvent* Clone() const override
	// {
	//	return new FriendNickChangedEvent(*this);
	// }

	void FromJson(const nlohmann::json& data) override;

	/// 获取好友信息
	User GetFriend() const { return this->_friend; }
	/// 获取更改前的昵称
	std::string GetOriginalNickname() const { return this->_from; }
	/// 获取当前昵称
	std::string GetNewNickname() const { return this->_to; }
};

} // namespace Mirai


#endif