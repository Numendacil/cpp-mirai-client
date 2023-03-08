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

#ifndef MIRAI_GROUP_EVENTS_HPP_
#define MIRAI_GROUP_EVENTS_HPP_

#include <optional>
#include <string>

#include <libmirai/Messages/MessageChain.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "IEvent.hpp"

namespace Mirai
{

// TODO: add a base class for all GroupSettingChangeEvent

/**
 * @brief 群聊开启/关闭匿名聊天事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `GroupAllowAnonymousChatEvent::group_` | `Group{}`
 * `GroupAllowAnonymousChatEvent::origin_` | `false`
 * `GroupAllowAnonymousChatEvent::current_` | `false`
 * `GroupAllowAnonymousChatEvent::operator_` | `std::nullopt`
 */
class GroupAllowAnonymousChatEvent final : public IEvent<GroupAllowAnonymousChatEvent>
{
	friend IEvent<GroupAllowAnonymousChatEvent>;
private:
	Group group_;
	bool origin_ = false;
	bool current_ = false;
	std::optional<GroupMember> operator_ = std::nullopt;

	static constexpr EventTypes TYPE_ = EventTypes::GroupAllowAnonymousChat;

public:
	/// 获取群聊信息
	Group GetGroup() const { return this->group_; }
	/// 原本匿名聊天是否开启
	bool GetOriginal() const { return this->origin_; }
	/// 现在匿名聊天是否开启
	bool GetCurrent() const { return this->current_; }
	/// 获取操作员信息，为Bot时返回 `std::nullopt`
	std::optional<GroupMember> GetOperator() const { return this->operator_; }

	struct Serializable;
};

/**
 * @brief 群聊开启/关闭坦白说事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `GroupAllowConfessTalkEvent::group_` | `Group{}`
 * `GroupAllowConfessTalkEvent::origin_` | `false`
 * `GroupAllowConfessTalkEvent::current_` | `false`
 * `GroupAllowConfessTalkEvent::ByBot_` | `false`
 */
class GroupAllowConfessTalkEvent final : public IEvent<GroupAllowConfessTalkEvent>
{
	friend IEvent<GroupAllowConfessTalkEvent>;
private:
	Group group_;
	bool origin_ = false;
	bool current_ = false;
	bool ByBot_ = false; // 无法获得操作员

	static constexpr EventTypes TYPE_ = EventTypes::GroupAllowConfessTalk;

public:
	/// 获取群聊信息
	Group GetGroup() const { return this->group_; }
	/// 原本坦白说是否开启
	bool GetOriginal() const { return this->origin_; }
	/// 现在坦白说是否开启
	bool GetCurrent() const { return this->current_; }
	/// 操作员是否为Bot自己（无法获得具体操作员信息）
	bool isByBot() const { return this->ByBot_; }

	struct Serializable;
};

/**
 * @brief 群聊开启/关闭允许群成员邀请好友加群事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `GroupAllowMemberInviteEvent::group_` | `Group{}`
 * `GroupAllowMemberInviteEvent::origin_` | `false`
 * `GroupAllowMemberInviteEvent::current_` | `false`
 * `GroupAllowMemberInviteEvent::operator_` | `std::nullopt`
 */
class GroupAllowMemberInviteEvent final : public IEvent<GroupAllowMemberInviteEvent>
{
	friend IEvent<GroupAllowMemberInviteEvent>;
private:
	Group group_;
	bool origin_ = false;
	bool current_ = false;
	std::optional<GroupMember> operator_ = std::nullopt;

	static constexpr EventTypes TYPE_ = EventTypes::GroupAllowMemberInvite;

public:
	/// 获取群聊信息
	Group GetGroup() const { return this->group_; }
	/// 原本是否允许群成员邀请好友加群
	bool GetOriginal() const { return this->origin_; }
	/// 现在是否允许群成员邀请好友加群
	bool GetCurrent() const { return this->current_; }
	/// 获取操作员信息，为Bot时返回 `std::nullopt`
	std::optional<GroupMember> GetOperator() const { return this->operator_; }

	struct Serializable;
};

/**
 * @brief 入群公告更改事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `GroupEntranceAnnouncementChangeEvent::group_` | `Group{}`
 * `GroupEntranceAnnouncementChangeEvent::origin_` | `""`
 * `GroupEntranceAnnouncementChangeEvent::current_` | `""`
 * `GroupEntranceAnnouncementChangeEvent::operator_` | `std::nullopt`
 *
 * @attention DEPRECATED, mirai 2.12 后该事件将不会被触发 (https://github.com/mamoe/mirai/issues/1873)
 */
class GroupEntranceAnnouncementChangeEvent final : public IEvent<GroupEntranceAnnouncementChangeEvent>
{
	friend IEvent<GroupEntranceAnnouncementChangeEvent>;
private:
	Group group_;
	std::string origin_;
	std::string current_;
	std::optional<GroupMember> operator_ = std::nullopt;

	static constexpr EventTypes TYPE_ = EventTypes::GroupEntranceAnnouncementChange;

public:
	/// 获取群聊信息
	Group GetGroup() const { return this->group_; }
	/// 获取更改前的入群公告
	std::string GetOriginal() const { return this->origin_; }
	/// 获取当前入群公告
	std::string GetCurrent() const { return this->current_; }
	/// 获取操作员信息，为Bot时返回 `std::nullopt`
	std::optional<GroupMember> GetOperator() const { return this->operator_; }

	struct Serializable;
};

/**
 * @brief 群聊名称更改事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `GroupNameChangeEvent::group_` | `Group{}`
 * `GroupNameChangeEvent::origin_` | `""`
 * `GroupNameChangeEvent::current_` | `""`
 * `GroupNameChangeEvent::operator_` | `std::nullopt`
 */
class GroupNameChangeEvent final : public IEvent<GroupNameChangeEvent>
{
	friend IEvent<GroupNameChangeEvent>;
private:
	Group group_;
	std::string origin_;
	std::string current_;
	std::optional<GroupMember> operator_ = std::nullopt;

	static constexpr EventTypes TYPE_ = EventTypes::GroupNameChange;

public:
	/// 获取群聊信息
	Group GetGroup() const { return this->group_; }
	/// 获取更改前的群聊名称
	std::string GetOriginal() const { return this->origin_; }
	/// 获取当前的群聊名称
	std::string GetCurrent() const { return this->current_; }
	/// 获取操作员信息，为Bot时返回 `std::nullopt`
	std::optional<GroupMember> GetOperator() const { return this->operator_; }

	struct Serializable;
};



/**
 * @brief 群聊消息事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `GroupMessageEvent::sender_` | `GroupMember{}`
 * `GroupMessageEvent::message_` | `MessageChain{}`
 */
class GroupMessageEvent final : public IEvent<GroupMessageEvent>
{
	friend IEvent<GroupMessageEvent>;

private:
	GroupMember sender_;
	MessageChain message_;

	static constexpr EventTypes TYPE_ = EventTypes::GroupMessage;

public:
	/// 获取发送者资料
	GroupMember GetSender() const { return this->sender_; }
	/// 获取消息内容
	MessageChain GetMessage() const { return this->message_; }

	// TODO: add helper methods for quick reply

	struct Serializable;
};



/**
 * @brief 群聊开启/关闭全体禁言事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `BotGroupPermissionChangeEvent::group_` | `Group{}`
 * `BotGroupPermissionChangeEvent::origin_` | `false`
 * `BotGroupPermissionChangeEvent::current_` | `false`
 * `BotGroupPermissionChangeEvent::operator_` | `std::nullopt`
 */
class GroupMuteAllEvent final : public IEvent<GroupMuteAllEvent>
{
	friend IEvent<GroupMuteAllEvent>;
private:
	Group group_;
	bool origin_ = false;
	bool current_ = false;
	std::optional<GroupMember> operator_ = std::nullopt;

	static constexpr EventTypes TYPE_ = EventTypes::GroupMuteAll;

public:
	/// 获取群聊信息
	Group GetGroup() const { return this->group_; }
	/// 原本是否处于全员禁言状态
	bool GetOriginal() const { return this->origin_; }
	/// 现在是否处于全员禁言
	bool GetCurrent() const { return this->current_; }
	/// 获取操作员信息，为Bot时返回 `std::nullopt`
	std::optional<GroupMember> GetOperator() const { return this->operator_; }

	struct Serializable;
};



/**
 * @brief 群消息撤回事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `GroupRecallEvent::AuthorId_` | `0_qq`
 * `GroupRecallEvent::MessageId_` | `0`
 * `GroupRecallEvent::time_` | `0`
 * `GroupRecallEvent::group_` | `Group{}`
 * `GroupRecallEvent::operator_` | `std::nullopt`
 */
class GroupRecallEvent final : public IEvent<GroupRecallEvent>
{
	friend IEvent<GroupRecallEvent>;
private:
	QQ_t AuthorId_;
	MessageId_t MessageId_ = 0;
	std::time_t time_ = 0;
	Group group_;
	std::optional<GroupMember> operator_ = std::nullopt;

	static constexpr EventTypes TYPE_ = EventTypes::GroupRecall;

public:
	/// 获取被撤回消息的发送者QQ
	QQ_t GetSender() const { return this->AuthorId_; }
	/// 获取被撤回消息的消息id
	MessageId_t GetMessageId() const { return this->MessageId_; }
	/// 获取被撤回消息的发送时间
	std::time_t GetSendTime() const { return this->time_; }
	/// 获取群聊信息
	Group GetGroup() const { return this->group_; }
	/// 获取操作员信息，为Bot时返回 `std::nullopt`
	std::optional<GroupMember> GetOperator() const { return this->operator_; }

	struct Serializable;
};



/**
 * @brief 群聊消息同步事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `GroupSyncMessageEvent::subject_` | `Group{}`
 * `GroupSyncMessageEvent::message_` | `MessageChain{}`
 */
class GroupSyncMessageEvent final : public IEvent<GroupSyncMessageEvent>
{
	friend IEvent<GroupSyncMessageEvent>;

private:
	Group subject_;
	MessageChain message_;

	static constexpr EventTypes TYPE_ = EventTypes::GroupSyncMessage;

public:
	/// 获取目标群聊信息
	Group GetGroup() const { return this->subject_; }
	/// 获取消息内容
	MessageChain GetMessage() const { return this->message_; }

	struct Serializable;
};



#define DECLARE_TYPE_ENUM(event_type)                                                                                      \
	template<> struct GetEventType<event_type::GetType()>                                                                  \
	{                                                                                                                  \
		using type = event_type;                                                                                           \
	}

DECLARE_TYPE_ENUM(GroupAllowAnonymousChatEvent);
DECLARE_TYPE_ENUM(GroupAllowConfessTalkEvent);
DECLARE_TYPE_ENUM(GroupAllowMemberInviteEvent);
DECLARE_TYPE_ENUM(GroupEntranceAnnouncementChangeEvent);
DECLARE_TYPE_ENUM(GroupNameChangeEvent);
DECLARE_TYPE_ENUM(GroupMessageEvent);
DECLARE_TYPE_ENUM(GroupMuteAllEvent);
DECLARE_TYPE_ENUM(GroupRecallEvent);
DECLARE_TYPE_ENUM(GroupSyncMessageEvent);

#undef DECLARE_TYPE_ENUM

} // namespace Mirai


#endif