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

#ifndef _MIRAI_BOT_EVENTS_HPP_
#define _MIRAI_BOT_EVENTS_HPP_

#include <optional>
#include <string>

#include <libmirai/Types/BasicTypes.hpp>

#include "IEvent.hpp"

namespace Mirai
{

/**
 * @brief 与Bot账号相关的事件
 * 
 * Middleware for bot account related events
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `BotAccountEvent::_qq` | `0_qq`
 */
template <class Event>
class BotAccountEvent : public IEvent<Event>
{
protected:
	QQ_t _qq{};

public:
	/// 获取BotQQ
	QQ_t GetQQ() const { return this->_qq; }
	
	struct Serializable;
};

/**
 * @brief Bot主动下线事件
 * 
 */
class BotOfflineEventActive final : public BotAccountEvent<BotOfflineEventActive>
{
	friend IEvent<BotOfflineEventActive>;
private:
	static constexpr EventTypes _TYPE_ = EventTypes::BotOfflineActive;
};

/**
 * @brief Bot掉线事件
 * 
 * 网络原因或服务器原因导致的掉线
 */
class BotOfflineEventDropped final : public BotAccountEvent<BotOfflineEventDropped>
{
	friend IEvent<BotOfflineEventDropped>;
private:
	static constexpr EventTypes _TYPE_ = EventTypes::BotOfflineDropped;
};

/**
 * @brief Bot强制下线事件
 * 
 * 通常是因为在相同平台的其它设备登录了Bot被挤下线
 */
class BotOfflineEventForce final : public BotAccountEvent<BotOfflineEventForce>
{
	friend IEvent<BotOfflineEventForce>;
private:
	static constexpr EventTypes _TYPE_ = EventTypes::BotOfflineForce;
};

/**
 * @brief Bot上线事件
 * 
 */
class BotOnlineEvent final : public BotAccountEvent<BotOnlineEvent>
{
	friend IEvent<BotOnlineEvent>;
private:
	static constexpr EventTypes _TYPE_ = EventTypes::BotOnline;
};

/**
 * @brief Bot重新登录事件
 * 
 */
class BotReloginEvent final : public BotAccountEvent<BotReloginEvent>
{
	friend IEvent<BotReloginEvent>;
private:
	static constexpr EventTypes _TYPE_ = EventTypes::BotRelogin;
};



/**
 * @brief Bot群聊权限改变事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `BotGroupPermissionChangeEvent::_group` | `Group{}`
 * `BotGroupPermissionChangeEvent::_origin` | `PERMISSION::ENUM_END`
 * `BotGroupPermissionChangeEvent::_current` | `PERMISSION::ENUM_END`
 */
class BotGroupPermissionChangeEvent final : public IEvent<BotGroupPermissionChangeEvent>
{
	friend IEvent<BotGroupPermissionChangeEvent>;
private:
	Group _group{};
	PERMISSION _origin = PERMISSION::ENUM_END;
	PERMISSION _current = PERMISSION::ENUM_END;

	static constexpr EventTypes _TYPE_ = EventTypes::BotGroupPermissionChange;

public:
	/// 获取群聊信息
	Group GetGroup() const { return this->_group; }
	/// 获取Bot更改前的权限
	PERMISSION GetOriginal() const { return this->_origin; }
	/// 获取Bot当前权限
	PERMISSION GetCurrent() const { return this->_current; }

	struct Serializable;
};



/**
 * @brief Bot被邀请加群事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `BotInvitedJoinGroupRequestEvent::_EventId` | `0`
 * `BotInvitedJoinGroupRequestEvent::_FromId` | `0_qq`
 * `BotInvitedJoinGroupRequestEvent::_GroupId` | `0_gid`
 * `BotInvitedJoinGroupRequestEvent::_GroupName` | `""`
 * `BotInvitedJoinGroupRequestEvent::_nickname` | `""`
 * `BotInvitedJoinGroupRequestEvent::_message` | `""`
 */
class BotInvitedJoinGroupRequestEvent final : public IEvent<BotInvitedJoinGroupRequestEvent>
{
	friend IEvent<BotInvitedJoinGroupRequestEvent>;

private:
	int64_t _EventId = 0;
	QQ_t _FromId{};
	GID_t _GroupId{};
	std::string _GroupName{};
	std::string _nickname{};
	std::string _message{};

	static constexpr EventTypes _TYPE_ = EventTypes::BotInvitedJoinGroupRequest;

public:
	/// 获取事件id，唯一标识符
	int64_t GetEventId() const { return this->_EventId; }
	/// 获取邀请人QQ
	QQ_t GetUserId() const { return this->_FromId; }
	/// 获取邀请人昵称
	std::string GetNickname() const { return this->_nickname; }
	/// 获取群聊id
	GID_t GetGroupId() const { return this->_GroupId; }
	/// 获取群聊名称
	std::string GetGroupName() const { return this->_GroupName; }
	/// 获取邀请信息
	std::string GetMessage() const { return this->_message; }

	// TODO: add helper methods for quick response

	struct Serializable;
};



/**
 * @brief Bot加入群聊事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `BotJoinGroupEvent::_group` | `Group{}`
 * `BotJoinGroupEvent::_inviter` | `std::nullopt`
 */
class BotJoinGroupEvent final : public IEvent<BotJoinGroupEvent>
{
	friend IEvent<BotJoinGroupEvent>;
private:
	Group _group{};
	std::optional<GroupMember> _inviter = std::nullopt;

	static constexpr EventTypes _TYPE_ = EventTypes::BotJoinGroup;

public:
	/// 获取群聊信息
	Group GetGroup() const { return this->_group; }
	/// 获取邀请人信息，若无则返回 `std::nullopt`
	std::optional<GroupMember> GetInviter() const { return this->_inviter; }

	struct Serializable;
};



/**
 * @brief Bot退群相关事件
 * 
 * Middleware for all bot leave group events
 * Member Variable | Default Value
 * --------------- | -------------
 * `BotLeaveEvent::_group` | `Group{}`
 */
template <class Event>
class BotLeaveEvent : public IEvent<Event>
{
protected:
	Group _group{};

public:
	/// 获取群聊信息
	Group GetGroup() const { return this->_group; }

	struct Serializable;
};

/**
 * @brief Bot主动退群事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `BotLeaveEventActive::_group` | `Group{}`
 */
class BotLeaveEventActive final : public BotLeaveEvent<BotLeaveEventActive>
{
	friend IEvent<BotLeaveEventActive>;
private:
	static constexpr EventTypes _TYPE_ = EventTypes::BotLeaveActive;
};

/**
 * @brief 群聊解散导致Bot退群事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `BotLeaveEventDisband::_group` | `Group{}`
 * `BotLeaveEventDisband::_operator` | `GroupMember{}`
 */
class BotLeaveEventDisband final : public BotLeaveEvent<BotLeaveEventDisband>
{
	friend IEvent<BotLeaveEventDisband>;
private:
	GroupMember _operator;

	static constexpr EventTypes _TYPE_ = EventTypes::BotLeaveDisband;

public:
	/// 获取操作员（群主）信息
	GroupMember GetOperator() const { return this->_operator; }

	struct Serializable;
};

/**
 * @brief Bot被踢出群聊事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `BotLeaveEventKick::_group` | `Group{}`
 * `BotLeaveEventKick::_operator` | `std::optional<GroupMember>{}`
 */
class BotLeaveEventKick final : public BotLeaveEvent<BotLeaveEventKick>
{
	friend IEvent<BotLeaveEventKick>;
private:
	std::optional<GroupMember> _operator = std::nullopt;

	static constexpr EventTypes _TYPE_ = EventTypes::BotLeaveKick;

public:
	/// 获取操作员信息，若无则返回 `std::nullopt`
	std::optional<GroupMember> GetOperator() const { return this->_operator; }

	struct Serializable;
};



/**
 * @brief Bot被禁言事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `BotMuteEvent::_operator` | `GroupMember{}`
 * `BotMuteEvent::_duration` | `0`
 */
class BotMuteEvent final : public IEvent<BotMuteEvent>
{
	friend IEvent<BotMuteEvent>;
protected:
	GroupMember _operator;
	std::time_t _duration = 0;

	static constexpr EventTypes _TYPE_ = EventTypes::BotMute;

public:
	/// 获取操作员信息
	GroupMember GetOperator() const { return this->_operator; }
	/// 获取禁言时间
	std::chrono::seconds GetMuteTime() const { return std::chrono::seconds(this->_duration); }

	struct Serializable;
};



/**
 * @brief Bot被取消禁言事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `BotUnmuteEvent::_operator` | `GroupMember{}`
 */
class BotUnmuteEvent final : public IEvent<BotUnmuteEvent>
{
	friend IEvent<BotUnmuteEvent>;
private:
	GroupMember _operator;
	static constexpr EventTypes _TYPE_ = EventTypes::BotUnmute;

public:
	/// 获取操作员信息
	GroupMember GetOperator() const { return this->_operator; }

	struct Serializable;
};


#define DECLARE_TYPE_ENUM(_type_)                                                                                      \
	template<> struct GetEventType<_type_::GetType()>                                                                  \
	{                                                                                                                  \
		using type = _type_;                                                                                           \
	}

DECLARE_TYPE_ENUM(BotOfflineEventActive);
DECLARE_TYPE_ENUM(BotOfflineEventDropped);
DECLARE_TYPE_ENUM(BotOfflineEventForce);
DECLARE_TYPE_ENUM(BotOnlineEvent);
DECLARE_TYPE_ENUM(BotReloginEvent);
DECLARE_TYPE_ENUM(BotGroupPermissionChangeEvent);
DECLARE_TYPE_ENUM(BotInvitedJoinGroupRequestEvent);
DECLARE_TYPE_ENUM(BotJoinGroupEvent);
DECLARE_TYPE_ENUM(BotLeaveEventActive);
DECLARE_TYPE_ENUM(BotLeaveEventDisband);
DECLARE_TYPE_ENUM(BotLeaveEventKick);
DECLARE_TYPE_ENUM(BotMuteEvent);
DECLARE_TYPE_ENUM(BotUnmuteEvent);

#undef DECLARE_TYPE_ENUM


} // namespace Mirai


#endif