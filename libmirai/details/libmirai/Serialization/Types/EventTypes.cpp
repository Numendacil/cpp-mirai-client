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

#include "EventTypes.hpp"

#include <nlohmann/json.hpp>

namespace Mirai
{

using json = nlohmann::json;

namespace
{

template <typename T>
using TypesList = std::array<T, static_cast<std::size_t>(EventTypes::ENUM_END)>;

constexpr void DeclareName(TypesList<std::string_view>& list, EventTypes type, std::string_view name)
{
	list.at(static_cast<std::size_t>(type)) = name;
}

constexpr TypesList<std::string_view> GetNames()
{
	TypesList<std::string_view> names;
	
	DeclareName(names, EventTypes::BotOfflineActive, "BotOfflineEventActive");
	DeclareName(names, EventTypes::BotOfflineDropped, "BotOfflineEventDropped");
	DeclareName(names, EventTypes::BotOfflineForce, "BotOfflineEventForce");
	DeclareName(names, EventTypes::BotOnline, "BotOnlineEvent");
	DeclareName(names, EventTypes::BotRelogin, "BotReloginEvent");
	DeclareName(names, EventTypes::BotGroupPermissionChange, "BotGroupPermissionChangeEvent");
	DeclareName(names, EventTypes::BotInvitedJoinGroupRequest, "BotInvitedJoinGroupRequestEvent");
	DeclareName(names, EventTypes::BotJoinGroup, "BotJoinGroupEvent");
	DeclareName(names, EventTypes::BotLeaveActive, "BotLeaveEventActive");
	DeclareName(names, EventTypes::BotLeaveDisband, "BotLeaveEventDisband");
	DeclareName(names, EventTypes::BotLeaveKick, "BotLeaveEventKick");
	DeclareName(names, EventTypes::BotMute, "BotMuteEvent");
	DeclareName(names, EventTypes::BotUnmute, "BotUnmuteEvent");

	DeclareName(names, EventTypes::CommandExecuted, "CommandExecutedEvent");

	DeclareName(names, EventTypes::FriendInputStatusChanged, "FriendInputStatusChangedEvent");
	DeclareName(names, EventTypes::FriendMessage, "FriendMessage");
	DeclareName(names, EventTypes::FriendNickChanged, "FriendNickChangedEvent");
	DeclareName(names, EventTypes::FriendRecall, "FriendRecallEvent");
	DeclareName(names, EventTypes::FriendSyncMessage, "FriendSyncMessage");

	DeclareName(names, EventTypes::GroupAllowAnonymousChat, "GroupAllowAnonymousChatEvent");
	DeclareName(names, EventTypes::GroupAllowConfessTalk, "GroupAllowConfessTalkEvent");
	DeclareName(names, EventTypes::GroupAllowMemberInvite, "GroupAllowMemberInviteEvent");
	DeclareName(names, EventTypes::GroupEntranceAnnouncementChange, "GroupEntranceAnnouncementChangeEvent");
	DeclareName(names, EventTypes::GroupNameChange, "GroupNameChangeEvent");
	DeclareName(names, EventTypes::GroupMessage, "GroupMessage");
	DeclareName(names, EventTypes::GroupMuteAll, "GroupMuteAllEvent");
	DeclareName(names, EventTypes::GroupRecall, "GroupRecallEvent");
	DeclareName(names, EventTypes::GroupSyncMessage, "GroupSyncMessage");

	DeclareName(names, EventTypes::MemberCardChange, "MemberCardChangeEvent");
	DeclareName(names, EventTypes::MemberHonorChange, "MemberHonorChangeEvent");
	DeclareName(names, EventTypes::MemberPermissionChange, "MemberPermissionChangeEvent");
	DeclareName(names, EventTypes::MemberSpecialTitleChange, "MemberSpecialTitleChangeEvent");
	DeclareName(names, EventTypes::MemberJoin, "MemberJoinEvent");
	DeclareName(names, EventTypes::MemberJoinRequest, "MemberJoinRequestEvent");
	DeclareName(names, EventTypes::MemberLeaveKick, "MemberLeaveEventKick");
	DeclareName(names, EventTypes::MemberLeaveQuit, "MemberLeaveEventQuit");
	DeclareName(names, EventTypes::MemberMute, "MemberMuteEvent");
	DeclareName(names, EventTypes::MemberUnmute, "MemberUnmuteEvent");

	DeclareName(names, EventTypes::ClientConnectionEstablished, "ClientConnectionEstablishedEvent");
	DeclareName(names, EventTypes::ClientConnectionError, "ClientConnectionErrorEvent");
	DeclareName(names, EventTypes::ClientConnectionClosed, "ClientConnectionClosedEvent");
	DeclareName(names, EventTypes::ClientParseError, "ClientParseErrorEvent");

	DeclareName(names, EventTypes::NewFriendRequest, "NewFriendRequestEvent");

	DeclareName(names, EventTypes::Nudge, "NudgeEvent");

	DeclareName(names, EventTypes::OtherClientMessage, "OtherClientMessage");
	DeclareName(names, EventTypes::OtherClientOffline, "OtherClientOfflineEvent");
	DeclareName(names, EventTypes::OtherClientOnline, "OtherClientOnlineEvent");

	DeclareName(names, EventTypes::StrangerMessage, "StrangerMessage");
	DeclareName(names, EventTypes::StrangerSyncMessage, "StrangerSyncMessage");

	DeclareName(names, EventTypes::TempMessage, "TempMessage");
	DeclareName(names, EventTypes::TempSyncMessage, "TempSyncMessage");

	return names;
}

constexpr auto names = GetNames();

constexpr std::string_view enum_to_str(const EventTypes& m)
{
	auto i = static_cast<std::size_t>(m);
	if (i < names.size()) return names.at(i);
	else
		return "";
}

constexpr EventTypes str_to_enum(std::string_view s)
{
	for (std::size_t i = 0; i < names.size(); i++)
		if (names.at(i) == s) return static_cast<EventTypes>(i);

	return EventTypes::ENUM_END;
}

} // namespace

void from_json(const json& j, EventTypes& p)
{
	MIRAI_PARSE_GUARD_BEGIN(j);

	p = str_to_enum(j.get<std::string>());

	MIRAI_PARSE_GUARD_END(j);
}
void to_json(json& j, const EventTypes& p)
{
	j = enum_to_str(p);
}

std::string to_string(EventTypes type)
{
	return std::string{enum_to_str(type)};
}

} // namespace Mirai