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

#ifndef _MIRAI_EVENT_TYPES_HPP_
#define _MIRAI_EVENT_TYPES_HPP_

#include <array>
#include <string>

namespace Mirai
{

enum class EventTypes
{
	BotOfflineActive = 0,
	BotOfflineDropped,
	BotOfflineForce,
	BotOnline,
	BotRelogin,
	BotGroupPermissionChange,
	BotInvitedJoinGroupRequest,
	BotJoinGroup,
	BotLeaveActive,
	BotLeaveDisband,
	BotLeaveKick,
	BotMute,
	BotUnmute,

	CommandExecuted,

	FriendInputStatusChanged,
	FriendMessage,
	FriendNickChanged,
	FriendRecall,
	FriendSyncMessage,

	GroupAllowAnonymousChat,
	GroupAllowConfessTalk,
	GroupAllowMemberInvite,
	GroupEntranceAnnouncementChange,
	GroupNameChange,
	GroupMessage,
	GroupMuteAll,
	GroupRecall,
	GroupSyncMessage,

	MemberCardChange,
	MemberHonorChange,
	MemberPermissionChange,
	MemberSpecialTitleChange,
	MemberJoin,
	MemberJoinRequest,
	MemberLeaveKick,
	MemberLeaveQuit,
	MemberMute,
	MemberUnmute,

	ClientConnectionEstablished,
	ClientConnectionError,
	ClientConnectionClosed,
	ClientParseError,

	NewFriendRequest,

	Nudge,

	OtherClientMessage,
	OtherClientOffline,
	OtherClientOnline,

	StrangerMessage,
	StrangerSyncMessage,

	TempMessage,
	TempSyncMessage,

	// Used for counting
	ENUM_END
};

std::string to_string(EventTypes type);

constexpr std::array<EventTypes, static_cast<size_t>(EventTypes::ENUM_END)> EventTypesList = []()
{
	std::array<EventTypes, static_cast<size_t>(EventTypes::ENUM_END)> arr{};
	for (size_t i = 0; i < arr.size(); i++)
		arr.at(i) = static_cast<EventTypes>(i);
	return arr;
}();

/**
 * @brief 用于类型之间转换的辅助模板
 * 
 */
template<EventTypes> struct GetEventType;

template<EventTypes type> using GetEventType_t = typename GetEventType<type>::type;

} // namespace Mirai

#endif