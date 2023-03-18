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

#ifndef MIRAI_MEMBER_EVENTS_HPP_
#define MIRAI_MEMBER_EVENTS_HPP_

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
 * `MemberCardChangeEvent::member_` | `GroupMember{}`
 * `MemberCardChangeEvent::origin_` | `""`
 * `MemberCardChangeEvent::current_` | `""`
 *
 * 由于服务器并不会告知名片变动, 此事件只能由 mirai 在发现变动时才广播
 */
class MemberCardChangeEvent final : public IEvent<MemberCardChangeEvent>
{
	friend IEvent<MemberCardChangeEvent>;
private:
	GroupMember member_;
	std::string origin_;
	std::string current_;

	static constexpr EventTypes TYPE_ = EventTypes::MemberCardChange;

public:
	/// 获取群成员资料
	GroupMember GetMember() const { return this->member_; }
	/// 获取更改前的群名片
	std::string GetOriginal() const { return this->origin_; }
	/// 获取当前的群名片
	std::string GetCurrent() const { return this->current_; }

	struct Serializable;
};



/**
 * @brief 群成员荣誉改变事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberHonorChangeEvent::member_` | `GroupMember{}`
 * `MemberHonorChangeEvent::action_` | `HonorChangeType::ENUM_END`
 * `MemberHonorChangeEvent::honor_` | `""`
 *
 * 目前只支持龙王
 */
class MemberHonorChangeEvent final : public IEvent<MemberHonorChangeEvent>
{
	friend IEvent<MemberHonorChangeEvent>;
private:
	GroupMember member_;
	HonorChangeType action_ = HonorChangeType::ENUM_END; // {achieve, lose}
	std::string honor_;

	static constexpr EventTypes TYPE_ = EventTypes::MemberHonorChange;

public:
	/// 获取群成员资料
	GroupMember GetMember() const { return this->member_; }
	/// 获取群荣誉变化行为
	HonorChangeType GetAction() const { return this->action_; }
	/// 获取群荣耀名称
	std::string GetHonor() const { return this->honor_; }

	struct Serializable;
};



/**
 * @brief 群成员权限更改事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberPermissionChangeEvent::member_` | `GroupMember{}`
 * `MemberPermissionChangeEvent::origin_` | `PERMISSION::ENUM_END`
 * `MemberPermissionChangeEvent::current_` | `PERMISSION::ENUM_END`
 *
 * 该成员不是Bot自己
 */
class MemberPermissionChangeEvent final : public IEvent<MemberPermissionChangeEvent>
{
	friend IEvent<MemberPermissionChangeEvent>;
private:
	GroupMember member_;
	PERMISSION origin_ = PERMISSION::ENUM_END;
	PERMISSION current_ = PERMISSION::ENUM_END;

	static constexpr EventTypes TYPE_ = EventTypes::MemberPermissionChange;

public:
	/// 获取群成员资料
	GroupMember GetMember() const { return this->member_; }
	/// 获取更改前的权限
	PERMISSION GetOriginal() const { return this->origin_; }
	/// 获取当前权限
	PERMISSION GetCurrent() const { return this->current_; }

	struct Serializable;
};



/**
 * @brief 群成员头衔更改事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberSpecialTitleChangeEvent::member_` | `GroupMember{}`
 * `MemberSpecialTitleChangeEvent::origin_` | `""`
 * `MemberSpecialTitleChangeEvent::current_` | `""`
 */
class MemberSpecialTitleChangeEvent final : public IEvent<MemberSpecialTitleChangeEvent>
{
	friend IEvent<MemberSpecialTitleChangeEvent>;
private:
	GroupMember member_;
	std::string origin_;
	std::string current_;

	static constexpr EventTypes TYPE_ = EventTypes::MemberSpecialTitleChange;

public:
	/// 获取群成员资料
	GroupMember GetMember() const { return this->member_; }
	/// 获取更改前的群头衔
	std::string GetOriginal() const { return this->origin_; }
	/// 获取当前群头衔
	std::string GetCurrent() const { return this->current_; }

	struct Serializable;
};



/**
 * @brief 新成员加入群聊事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberJoinEvent::member_` | `GroupMember{}`
 * `MemberJoinEvent::inviter_` | `std::nullopt`
 */
class MemberJoinEvent final : public IEvent<MemberJoinEvent>
{
	friend IEvent<MemberJoinEvent>;
private:
	GroupMember member_;
	std::optional<GroupMember> inviter_ = std::nullopt;

	static constexpr EventTypes TYPE_ = EventTypes::MemberJoin;

public:
	/// 获取群成员资料
	GroupMember GetMember() const { return this->member_; }
	/// 获取邀请人信息，若无则返回 `std::nullopt`
	std::optional<GroupMember> GetInviter() const { return this->inviter_; }

	struct Serializable;
};



/**
 * @brief 用户申请入群事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberJoinRequestEvent::EventId_` | `0`
 * `MemberJoinRequestEvent::FromId_` | `0_qq`
 * `MemberJoinRequestEvent::GroupId_` | `0_gid`
 * `MemberJoinRequestEvent::GroupName_` | `""`
 * `MemberJoinRequestEvent::nickname_` | `""`
 * `MemberJoinRequestEvent::message_` | `""`
 * `MemberJoinRequestEvent::inviter_` | `std::nullopt`
 *
 * 需要Bot为管理员或群主
 */
class MemberJoinRequestEvent final : public IEvent<MemberJoinRequestEvent>
{
	friend IEvent<MemberJoinRequestEvent>;

private:
	int64_t EventId_ = 0;
	QQ_t FromId_;
	GID_t GroupId_;
	std::string GroupName_;
	std::string nickname_;
	std::string message_;
	std::optional<QQ_t> InviterId_ = std::nullopt;

	static constexpr EventTypes TYPE_ = EventTypes::MemberJoinRequest;

public:
	/// 获取事件id，唯一标识符
	int64_t GetEventId() const { return this->EventId_; }
	/// 获取申请人QQ
	QQ_t GetUserId() const { return this->FromId_; }
	/// 获取申请人昵称
	std::string GetNickname() const { return this->nickname_; }
	/// 获取群聊id
	GID_t GetGroupId() const { return this->GroupId_; }
	/// 获取群聊名称
	std::string GetGroupName() const { return this->GroupName_; }
	/// 获取申请信息
	std::string GetMessage() const { return this->message_; }
	
	/// 获取邀请人信息，若无则返回 `std::nullopt`
	std::optional<QQ_t> GetInviter() const { return this->InviterId_; }

	// TODO: add helper methods for quick response

	struct Serializable;
};



/**
 * @brief 成员退出群聊相关事件
 * 
 * Middleware for all leave group related events
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberLeaveEvent::member_` | `GroupMember{}`
 */
template <class Event>
class MemberLeaveEvent : public IEvent<Event>
{
private:
	GroupMember member_;

public:
	/// 获取群成员资料
	GroupMember GetMember() const { return this->member_; }

	struct Serializable;
};

/**
 * @brief 成员被踢出群聊事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberLeaveEventKick::member_` | `GroupMember{}`
 * `MemberLeaveEventKick::operator_` | `std::nullopt`
 *
 * 该成员不是Bot自己
 */
class MemberLeaveEventKick : public MemberLeaveEvent<MemberLeaveEventKick>
{
	friend IEvent<MemberLeaveEventKick>;
private:
	std::optional<GroupMember> operator_ = std::nullopt;

	static constexpr EventTypes TYPE_ = EventTypes::MemberLeaveKick;

public:
	/// 获取操作员信息，为Bot时返回 `std::nullopt`
	std::optional<GroupMember> GetOperator() const { return this->operator_; }

	struct Serializable;
};

/**
 * @brief 成员主动退出群聊事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberLeaveEventQuit::member_` | `GroupMember{}`
 *
 * 该成员不是Bot自己
 */
class MemberLeaveEventQuit : public MemberLeaveEvent<MemberLeaveEventQuit>
{
	friend IEvent<MemberLeaveEventQuit>;
private:
	static constexpr EventTypes TYPE_ = EventTypes::MemberLeaveQuit;
};



/**
 * @brief 群成员被禁言事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberMuteEvent::member_` | `GroupMember{}`
 * `MemberMuteEvent::duration_` | `0`
 * `MemberMuteEvent::operator_` | `std::nullopt`
 *
 * 该成员不是Bot自己
 */
class MemberMuteEvent final : public IEvent<MemberMuteEvent>
{
	friend IEvent<MemberMuteEvent>;
private:
	GroupMember member_;
	std::time_t duration_ = 0;
	std::optional<GroupMember> operator_ = std::nullopt;

	static constexpr EventTypes TYPE_ = EventTypes::MemberMute;

public:
	/// 获取群成员资料
	GroupMember GetMember() const { return this->member_; }
	/// 获取禁言时长
	std::chrono::seconds GetDuration() const { return std::chrono::seconds(this->duration_); }
	/// 获取操作员信息，为Bot时返回 `std::nullopt`
	std::optional<GroupMember> GetOperator() const { return this->operator_; }

	struct Serializable;
};



/**
 * @brief 群成员被取消禁言事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberUnmuteEvent::member_` | `GroupMember{}`
 * `MemberUnmuteEvent::operator_` | `std::nullopt`
 *
 * 该成员不是Bot自己
 */
class MemberUnmuteEvent final : public IEvent<MemberUnmuteEvent>
{
	friend IEvent<MemberUnmuteEvent>;
private:
	GroupMember member_;
	std::optional<GroupMember> operator_ = std::nullopt;

	static constexpr EventTypes TYPE_ = EventTypes::MemberUnmute;

public:
	/// 获取群成员资料
	GroupMember GetMember() const { return this->member_; }
	/// 获取操作员信息，为Bot时返回 `std::nullopt`
	std::optional<GroupMember> GetOperator() const { return this->operator_; }

	struct Serializable;
};



#define DECLARE_TYPE_ENUM(event_type)                                                                                      \
	template<> struct GetEventType<event_type::GetType()>                                                                  \
	{                                                                                                                  \
		using type = event_type;                                                                                           \
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