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

#ifndef MIRAI_NEW_FRIEND_REQUEST_EVENT_HPP_
#define MIRAI_NEW_FRIEND_REQUEST_EVENT_HPP_

#include <string>

#include <libmirai/Types/BasicTypes.hpp>

#include "IEvent.hpp"

namespace Mirai
{

/**
 * @brief 用户申请添加Bot好友事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `NewFriendRequestEvent::EventId_` | `0`
 * `NewFriendRequestEvent::FromId_` | `0_qq`
 * `NewFriendRequestEvent::GroupId_` | `0_gid`
 * `NewFriendRequestEvent::nickname_` | `""`
 * `NewFriendRequestEvent::message_` | `""`
 */
class NewFriendRequestEvent final : public IEvent<NewFriendRequestEvent>
{
	friend IEvent<NewFriendRequestEvent>;
private:
	int64_t EventId_ = 0;
	QQ_t FromId_;
	GID_t GroupId_;
	std::string nickname_;
	std::string message_;

	static constexpr EventTypes TYPE_ = EventTypes::NewFriendRequest;

public:
	/// 获取事件id，唯一标识符
	int64_t GetEventId() const { return this->EventId_; }
	/// 获取申请人QQ
	QQ_t GetUserId() const { return this->FromId_; }
	/// 获取申请人昵称
	std::string GetNickname() const { return this->nickname_; }
	/// 获取申请人来自的群聊，若无则返回 `0_gid`
	GID_t GetGroupId() const { return this->GroupId_; }
	/// 申请人是否通过群聊找到Bot
	bool isFromGroup() const { return this->GroupId_ != (GID_t)0; }
	/// 获取申请信息
	std::string GetMessage() const { return this->message_; }

	// TODO: add helper methods for quick response

	struct Serializable;
};

template<>
struct GetEventType<NewFriendRequestEvent::GetType()>
{
	using type = NewFriendRequestEvent;
};

} // namespace Mirai


#endif