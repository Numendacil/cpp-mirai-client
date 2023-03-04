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

#ifndef _MIRAI_GROUP_EVENTS_HPP_
#define _MIRAI_GROUP_EVENTS_HPP_

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
 * `GroupAllowAnonymousChatEvent::_group` | `Group{}`
 * `GroupAllowAnonymousChatEvent::_origin` | `false`
 * `GroupAllowAnonymousChatEvent::_current` | `false`
 * `GroupAllowAnonymousChatEvent::_operator` | `std::nullopt`
 */
class GroupAllowAnonymousChatEvent final : public IEvent<GroupAllowAnonymousChatEvent>
{
	friend IEvent<GroupAllowAnonymousChatEvent>;
private:
	Group _group;
	bool _origin = false;
	bool _current = false;
	std::optional<GroupMember> _operator = std::nullopt;

	static constexpr EventTypes _TYPE_ = EventTypes::GroupAllowAnonymousChat;

public:
	/// 获取群聊信息
	Group GetGroup() const { return this->_group; }
	/// 原本匿名聊天是否开启
	bool GetOriginal() const { return this->_origin; }
	/// 现在匿名聊天是否开启
	bool GetCurrent() const { return this->_current; }
	/// 获取操作员信息，为Bot时返回 `std::nullopt`
	std::optional<GroupMember> GetOperator() const { return this->_operator; }

	struct Serializable;
};

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
class GroupAllowConfessTalkEvent final : public IEvent<GroupAllowConfessTalkEvent>
{
	friend IEvent<GroupAllowConfessTalkEvent>;
private:
	Group _group;
	bool _origin = false;
	bool _current = false;
	bool _ByBot = false; // 无法获得操作员

	static constexpr EventTypes _TYPE_ = EventTypes::GroupAllowConfessTalk;

public:
	/// 获取群聊信息
	Group GetGroup() const { return this->_group; }
	/// 原本坦白说是否开启
	bool GetOriginal() const { return this->_origin; }
	/// 现在坦白说是否开启
	bool GetCurrent() const { return this->_current; }
	/// 操作员是否为Bot自己（无法获得具体操作员信息）
	bool isByBot() const { return this->_ByBot; }

	struct Serializable;
};

/**
 * @brief 群聊开启/关闭允许群成员邀请好友加群事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `GroupAllowMemberInviteEvent::_group` | `Group{}`
 * `GroupAllowMemberInviteEvent::_origin` | `false`
 * `GroupAllowMemberInviteEvent::_current` | `false`
 * `GroupAllowMemberInviteEvent::_operator` | `std::nullopt`
 */
class GroupAllowMemberInviteEvent final : public IEvent<GroupAllowMemberInviteEvent>
{
	friend IEvent<GroupAllowMemberInviteEvent>;
private:
	Group _group;
	bool _origin = false;
	bool _current = false;
	std::optional<GroupMember> _operator = std::nullopt;

	static constexpr EventTypes _TYPE_ = EventTypes::GroupAllowMemberInvite;

public:
	/// 获取群聊信息
	Group GetGroup() const { return this->_group; }
	/// 原本是否允许群成员邀请好友加群
	bool GetOriginal() const { return this->_origin; }
	/// 现在是否允许群成员邀请好友加群
	bool GetCurrent() const { return this->_current; }
	/// 获取操作员信息，为Bot时返回 `std::nullopt`
	std::optional<GroupMember> GetOperator() const { return this->_operator; }

	struct Serializable;
};

/**
 * @brief 入群公告更改事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `GroupEntranceAnnouncementChangeEvent::_group` | `Group{}`
 * `GroupEntranceAnnouncementChangeEvent::_origin` | `""`
 * `GroupEntranceAnnouncementChangeEvent::_current` | `""`
 * `GroupEntranceAnnouncementChangeEvent::_operator` | `std::nullopt`
 *
 * @attention DEPRECATED, mirai 2.12 后该事件将不会被触发 (https://github.com/mamoe/mirai/issues/1873)
 */
class GroupEntranceAnnouncementChangeEvent final : public IEvent<GroupEntranceAnnouncementChangeEvent>
{
	friend IEvent<GroupEntranceAnnouncementChangeEvent>;
private:
	Group _group;
	std::string _origin;
	std::string _current;
	std::optional<GroupMember> _operator = std::nullopt;

	static constexpr EventTypes _TYPE_ = EventTypes::GroupEntranceAnnouncementChange;

public:
	/// 获取群聊信息
	Group GetGroup() const { return this->_group; }
	/// 获取更改前的入群公告
	std::string GetOriginal() const { return this->_origin; }
	/// 获取当前入群公告
	std::string GetCurrent() const { return this->_current; }
	/// 获取操作员信息，为Bot时返回 `std::nullopt`
	std::optional<GroupMember> GetOperator() const { return this->_operator; }

	struct Serializable;
};

/**
 * @brief 群聊名称更改事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `GroupNameChangeEvent::_group` | `Group{}`
 * `GroupNameChangeEvent::_origin` | `""`
 * `GroupNameChangeEvent::_current` | `""`
 * `GroupNameChangeEvent::_operator` | `std::nullopt`
 */
class GroupNameChangeEvent final : public IEvent<GroupNameChangeEvent>
{
	friend IEvent<GroupNameChangeEvent>;
private:
	Group _group;
	std::string _origin;
	std::string _current;
	std::optional<GroupMember> _operator = std::nullopt;

	static constexpr EventTypes _TYPE_ = EventTypes::GroupNameChange;

public:
	/// 获取群聊信息
	Group GetGroup() const { return this->_group; }
	/// 获取更改前的群聊名称
	std::string GetOriginal() const { return this->_origin; }
	/// 获取当前的群聊名称
	std::string GetCurrent() const { return this->_current; }
	/// 获取操作员信息，为Bot时返回 `std::nullopt`
	std::optional<GroupMember> GetOperator() const { return this->_operator; }

	struct Serializable;
};



/**
 * @brief 群聊消息事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `GroupMessageEvent::_sender` | `GroupMember{}`
 * `GroupMessageEvent::_message` | `MessageChain{}`
 */
class GroupMessageEvent final : public IEvent<GroupMessageEvent>
{
	friend IEvent<GroupMessageEvent>;

private:
	GroupMember _sender;
	MessageChain _message;

	static constexpr EventTypes _TYPE_ = EventTypes::GroupMessage;

public:
	/// 获取发送者资料
	GroupMember GetSender() const { return this->_sender; }
	/// 获取消息内容
	MessageChain GetMessage() const { return this->_message; }

	// TODO: add helper methods for quick reply

	struct Serializable;
};



/**
 * @brief 群聊开启/关闭全体禁言事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `BotGroupPermissionChangeEvent::_group` | `Group{}`
 * `BotGroupPermissionChangeEvent::_origin` | `false`
 * `BotGroupPermissionChangeEvent::_current` | `false`
 * `BotGroupPermissionChangeEvent::_operator` | `std::nullopt`
 */
class GroupMuteAllEvent final : public IEvent<GroupMuteAllEvent>
{
	friend IEvent<GroupMuteAllEvent>;
private:
	Group _group;
	bool _origin = false;
	bool _current = false;
	std::optional<GroupMember> _operator = std::nullopt;

	static constexpr EventTypes _TYPE_ = EventTypes::GroupMuteAll;

public:
	/// 获取群聊信息
	Group GetGroup() const { return this->_group; }
	/// 原本是否处于全员禁言状态
	bool GetOriginal() const { return this->_origin; }
	/// 现在是否处于全员禁言
	bool GetCurrent() const { return this->_current; }
	/// 获取操作员信息，为Bot时返回 `std::nullopt`
	std::optional<GroupMember> GetOperator() const { return this->_operator; }

	struct Serializable;
};



/**
 * @brief 群消息撤回事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `GroupRecallEvent::_AuthorId` | `0_qq`
 * `GroupRecallEvent::_MessageId` | `0`
 * `GroupRecallEvent::_time` | `0`
 * `GroupRecallEvent::_group` | `Group{}`
 * `GroupRecallEvent::_operator` | `std::nullopt`
 */
class GroupRecallEvent final : public IEvent<GroupRecallEvent>
{
	friend IEvent<GroupRecallEvent>;
private:
	QQ_t _AuthorId;
	MessageId_t _MessageId = 0;
	std::time_t _time = 0;
	Group _group;
	std::optional<GroupMember> _operator = std::nullopt;

	static constexpr EventTypes _TYPE_ = EventTypes::GroupRecall;

public:
	/// 获取被撤回消息的发送者QQ
	QQ_t GetSender() const { return this->_AuthorId; }
	/// 获取被撤回消息的消息id
	MessageId_t GetMessageId() const { return this->_MessageId; }
	/// 获取被撤回消息的发送时间
	std::time_t GetSendTime() const { return this->_time; }
	/// 获取群聊信息
	Group GetGroup() const { return this->_group; }
	/// 获取操作员信息，为Bot时返回 `std::nullopt`
	std::optional<GroupMember> GetOperator() const { return this->_operator; }

	struct Serializable;
};



/**
 * @brief 群聊消息同步事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `GroupSyncMessageEvent::_subject` | `Group{}`
 * `GroupSyncMessageEvent::_message` | `MessageChain{}`
 */
class GroupSyncMessageEvent final : public IEvent<GroupSyncMessageEvent>
{
	friend IEvent<GroupSyncMessageEvent>;

private:
	Group _subject;
	MessageChain _message;

	static constexpr EventTypes _TYPE_ = EventTypes::GroupSyncMessage;

public:
	/// 获取目标群聊信息
	Group GetGroup() const { return this->_subject; }
	/// 获取消息内容
	MessageChain GetMessage() const { return this->_message; }

	struct Serializable;
};



#define DECLARE_TYPE_ENUM(_type_)                                                                                      \
	template<> struct GetEventType<_type_::GetType()>                                                                  \
	{                                                                                                                  \
		using type = _type_;                                                                                           \
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