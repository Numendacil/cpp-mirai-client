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

#ifndef _MIRAI_MEMBER_JOIN_REQUEST_EVENT_HPP_
#define _MIRAI_MEMBER_JOIN_REQUEST_EVENT_HPP_

#include <string>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 用户申请入群事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberJoinRequestEvent::_EventId` | `0`
 * `MemberJoinRequestEvent::_FromId` | `0_qq`
 * `MemberJoinRequestEvent::_GroupId` | `0_gid`
 * `MemberJoinRequestEvent::_GroupName` | `""`
 * `MemberJoinRequestEvent::_nickname` | `""`
 * `MemberJoinRequestEvent::_message` | `""`
 *
 * 需要Bot为管理员或群主
 */
class MemberJoinRequestEvent : public EventBase
{

protected:
	int64_t _EventId = 0;
	QQ_t _FromId;
	GID_t _GroupId;
	std::string _GroupName;
	std::string _nickname;
	std::string _message;

	void Deserialize(const void*) final;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "MemberJoinRequestEvent";

	/// 获取事件id，唯一标识符
	int64_t GetEventId() const { return this->_EventId; }
	/// 获取申请人QQ
	QQ_t GetUserId() const { return this->_FromId; }
	/// 获取申请人昵称
	std::string GetNickname() const { return this->_nickname; }
	/// 获取群聊id
	GID_t GetGroupId() const { return this->_GroupId; }
	/// 获取群聊名称
	std::string GetGroupName() const { return this->_GroupName; }
	/// 获取申请信息
	std::string GetMessage() const { return this->_message; }

	// TODO: add helper methods for quick response
};

} // namespace Mirai


#endif