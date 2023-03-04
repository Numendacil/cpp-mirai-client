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

#ifndef _MIRAI_MEMBER_EVENTS_HPP_
#define _MIRAI_MEMBER_EVENTS_HPP_

#include <optional>
#include <string>

#include <libmirai/Types/BasicTypes.hpp>

#include "IEvent.hpp"

namespace Mirai
{

/**
 * @brief 群成员名片更改事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberCardChangeEvent::_member` | `GroupMember{}`
 * `MemberCardChangeEvent::_origin` | `""`
 * `MemberCardChangeEvent::_current` | `""`
 *
 * 由于服务器并不会告知名片变动, 此事件只能由 mirai 在发现变动时才广播
 */
class MemberCardChangeEvent final : public IEvent<MemberCardChangeEvent>
{
	friend IEvent<MemberCardChangeEvent>;
private:
	GroupMember _member;
	std::string _origin;
	std::string _current;

	static constexpr EventTypes _TYPE_ = EventTypes::MemberCardChange;

public:
	/// 获取群成员资料
	GroupMember GetMember() const { return this->_member; }
	/// 获取更改前的群名片
	std::string GetOriginal() const { return this->_origin; }
	/// 获取当前的群名片
	std::string GetCurrent() const { return this->_current; }

	struct Serializable;
};



/**
 * @brief 群成员荣誉改变事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberHonorChangeEvent::_member` | `GroupMember{}`
 * `MemberHonorChangeEvent::_action` | `HonorChangeType::ENUM_END`
 * `MemberHonorChangeEvent::_honor` | `""`
 *
 * 目前只支持龙王
 */
class MemberHonorChangeEvent final : public IEvent<MemberHonorChangeEvent>
{
	friend IEvent<MemberHonorChangeEvent>;
private:
	GroupMember _member;
	HonorChangeType _action = HonorChangeType::ENUM_END; // {achieve, lose}
	std::string _honor;

	static constexpr EventTypes _TYPE_ = EventTypes::MemberHonorChange;

public:
	/// 获取群成员资料
	GroupMember GetMember() const { return this->_member; }
	/// 获取群荣誉变化行为
	HonorChangeType GetAction() const { return this->_action; }
	/// 获取群荣耀名称
	std::string GetHonor() const { return this->_honor; }

	struct Serializable;
};



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
class MemberPermissionChangeEvent final : public IEvent<MemberPermissionChangeEvent>
{
	friend IEvent<MemberPermissionChangeEvent>;
private:
	GroupMember _member;
	PERMISSION _origin = PERMISSION::ENUM_END;
	PERMISSION _current = PERMISSION::ENUM_END;

	static constexpr EventTypes _TYPE_ = EventTypes::MemberPermissionChange;

public:
	/// 获取群成员资料
	GroupMember GetMember() const { return this->_member; }
	/// 获取更改前的权限
	PERMISSION GetOriginal() const { return this->_origin; }
	/// 获取当前权限
	PERMISSION GetCurrent() const { return this->_current; }

	struct Serializable;
};



/**
 * @brief 群成员头衔更改事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberSpecialTitleChangeEvent::_member` | `GroupMember{}`
 * `MemberSpecialTitleChangeEvent::_origin` | `""`
 * `MemberSpecialTitleChangeEvent::_current` | `""`
 */
class MemberSpecialTitleChangeEvent final : public IEvent<MemberSpecialTitleChangeEvent>
{
	friend IEvent<MemberSpecialTitleChangeEvent>;
private:
	GroupMember _member;
	std::string _origin;
	std::string _current;

	static constexpr EventTypes _TYPE_ = EventTypes::MemberSpecialTitleChange;

public:
	/// 获取群成员资料
	GroupMember GetMember() const { return this->_member; }
	/// 获取更改前的群头衔
	std::string GetOriginal() const { return this->_origin; }
	/// 获取当前群头衔
	std::string GetCurrent() const { return this->_current; }

	struct Serializable;
};



/**
 * @brief 新成员加入群聊事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberJoinEvent::_member` | `GroupMember{}`
 * `MemberJoinEvent::_inviter` | `std::nullopt`
 */
class MemberJoinEvent final : public IEvent<MemberJoinEvent>
{
	friend IEvent<MemberJoinEvent>;
private:
	GroupMember _member;
	std::optional<GroupMember> _inviter = std::nullopt;

	static constexpr EventTypes _TYPE_ = EventTypes::MemberJoin;

public:
	/// 获取群成员资料
	GroupMember GetMember() const { return this->_member; }
	/// 获取邀请人信息，若无则返回 `std::nullopt`
	std::optional<GroupMember> GetInviter() const { return this->_inviter; }

	struct Serializable;
};



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
class MemberJoinRequestEvent final : public IEvent<MemberJoinRequestEvent>
{
	friend IEvent<MemberJoinRequestEvent>;

private:
	int64_t _EventId = 0;
	QQ_t _FromId;
	GID_t _GroupId;
	std::string _GroupName;
	std::string _nickname;
	std::string _message;

	static constexpr EventTypes _TYPE_ = EventTypes::MemberJoinRequest;

public:
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

	struct Serializable;
};



/**
 * @brief 成员退出群聊相关事件
 * 
 * Middleware for all leave group related events
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberLeaveEvent::_member` | `GroupMember{}`
 */
template <class Event>
class MemberLeaveEvent : public IEvent<Event>
{
private:
	GroupMember _member;

public:
	/// 获取群成员资料
	GroupMember GetMember() const { return this->_member; }

	struct Serializable;
};

/**
 * @brief 成员被踢出群聊事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberLeaveEventKick::_member` | `GroupMember{}`
 * `MemberLeaveEventKick::_operator` | `std::nullopt`
 *
 * 该成员不是Bot自己
 */
class MemberLeaveEventKick : public MemberLeaveEvent<MemberLeaveEventKick>
{
	friend IEvent<MemberLeaveEventKick>;
private:
	std::optional<GroupMember> _operator = std::nullopt;

	static constexpr EventTypes _TYPE_ = EventTypes::MemberLeaveKick;

public:
	/// 获取操作员信息，为Bot时返回 `std::nullopt`
	std::optional<GroupMember> GetOperator() const { return this->_operator; }

	struct Serializable;
};

/**
 * @brief 成员主动退出群聊事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberLeaveEventQuit::_member` | `GroupMember{}`
 *
 * 该成员不是Bot自己
 */
class MemberLeaveEventQuit : public MemberLeaveEvent<MemberLeaveEventQuit>
{
	friend IEvent<MemberLeaveEventQuit>;
private:
	static constexpr EventTypes _TYPE_ = EventTypes::MemberLeaveQuit;
};



/**
 * @brief 群成员被禁言事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberMuteEvent::_member` | `GroupMember{}`
 * `MemberMuteEvent::_duration` | `0`
 * `MemberMuteEvent::_operator` | `std::nullopt`
 *
 * 该成员不是Bot自己
 */
class MemberMuteEvent final : public IEvent<MemberMuteEvent>
{
	friend IEvent<MemberMuteEvent>;
private:
	GroupMember _member;
	std::time_t _duration = 0;
	std::optional<GroupMember> _operator = std::nullopt;

	static constexpr EventTypes _TYPE_ = EventTypes::MemberMute;

public:
	/// 获取群成员资料
	GroupMember GetMember() const { return this->_member; }
	/// 获取禁言时长
	std::chrono::seconds GetDuration() const { return std::chrono::seconds(this->_duration); }
	/// 获取操作员信息，为Bot时返回 `std::nullopt`
	std::optional<GroupMember> GetOperator() const { return this->_operator; }

	struct Serializable;
};



/**
 * @brief 群成员被取消禁言事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberUnmuteEvent::_member` | `GroupMember{}`
 * `MemberUnmuteEvent::_operator` | `std::nullopt`
 *
 * 该成员不是Bot自己
 */
class MemberUnmuteEvent final : public IEvent<MemberUnmuteEvent>
{
	friend IEvent<MemberUnmuteEvent>;
private:
	GroupMember _member;
	std::optional<GroupMember> _operator = std::nullopt;

	static constexpr EventTypes _TYPE_ = EventTypes::MemberUnmute;

public:
	/// 获取群成员资料
	GroupMember GetMember() const { return this->_member; }
	/// 获取操作员信息，为Bot时返回 `std::nullopt`
	std::optional<GroupMember> GetOperator() const { return this->_operator; }

	struct Serializable;
};



#define DECLARE_TYPE_ENUM(_type_)                                                                                      \
	template<> struct GetEventType<_type_::GetType()>                                                                  \
	{                                                                                                                  \
		using type = _type_;                                                                                           \
	}

DECLARE_TYPE_ENUM(MemberCardChangeEvent);
DECLARE_TYPE_ENUM(MemberHonorChangeEvent);
DECLARE_TYPE_ENUM(MemberPermissionChangeEvent);
DECLARE_TYPE_ENUM(MemberSpecialTitleChangeEvent);
DECLARE_TYPE_ENUM(MemberJoinEvent);
DECLARE_TYPE_ENUM(MemberJoinRequestEvent);
DECLARE_TYPE_ENUM(MemberLeaveEventKick);
DECLARE_TYPE_ENUM(MemberLeaveEventQuit);
DECLARE_TYPE_ENUM(MemberMuteEvent);
DECLARE_TYPE_ENUM(MemberUnmuteEvent);

#undef DECLARE_TYPE_ENUM

} // namespace Mirai


#endif