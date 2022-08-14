#include <chrono>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include <libmirai/Events/Events.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "JsonData.hpp"

using namespace Mirai;
using json = nlohmann::json;

TEST(EventsTest, BotOnlineEvent)
{
	BotOnlineEvent event(nullptr);
	event.FromJson(Data::EventData["BotOnlineEvent"]);
	EXPECT_EQ(event.GetType(), "BotOnlineEvent");

	EXPECT_EQ(event.GetQQ(), 123456_qq);
}

TEST(EventsTest, BotOfflineEventActive)
{
	BotOfflineEventActive event(nullptr);
	event.FromJson(Data::EventData["BotOfflineEventActive"]);
	EXPECT_EQ(event.GetType(), "BotOfflineEventActive");

	EXPECT_EQ(event.GetQQ(), 123456_qq);
}

TEST(EventsTest, BotOfflineEventDropped)
{
	BotOfflineEventDropped event(nullptr);
	event.FromJson(Data::EventData["BotOfflineEventDropped"]);
	EXPECT_EQ(event.GetType(), "BotOfflineEventDropped");

	EXPECT_EQ(event.GetQQ(), 123456_qq);
}

TEST(EventsTest, FriendInputStatusChangedEvent)
{
	FriendInputStatusChangedEvent event(nullptr);
	event.FromJson(Data::EventData["FriendInputStatusChangedEvent"]);
	EXPECT_EQ(event.GetType(), "FriendInputStatusChangedEvent");

	EXPECT_TRUE(event.isInputting());
	EXPECT_EQ(event.GetFriend().id, 123123_qq);
	EXPECT_EQ(event.GetFriend().nickname, "nick");
	EXPECT_EQ(event.GetFriend().remark, "remark");
}

TEST(EventsTest, FriendNickChangedEvent)
{
	FriendNickChangedEvent event(nullptr);
	event.FromJson(Data::EventData["FriendNickChangedEvent"]);
	EXPECT_EQ(event.GetType(), "FriendNickChangedEvent");

	EXPECT_EQ(event.GetFriend().id, 123123_qq);
	EXPECT_EQ(event.GetFriend().nickname, "new nickname");
	EXPECT_EQ(event.GetFriend().remark, "remark");
	EXPECT_EQ(event.GetOriginalNickname(), "origin nickname");
	EXPECT_EQ(event.GetNewNickname(), "new nickname");
}

TEST(EventsTest, BotGroupPermissionChangeEvent)
{
	BotGroupPermissionChangeEvent event(nullptr);
	event.FromJson(Data::EventData["BotGroupPermissionChangeEvent"]);
	EXPECT_EQ(event.GetType(), "BotGroupPermissionChangeEvent");

	EXPECT_EQ(event.GetOriginal(), PERMISSION::MEMBER);
	EXPECT_EQ(event.GetCurrent(), PERMISSION::ADMINISTRATOR);
	EXPECT_EQ(event.GetGroup().id, 123456789_gid);
	EXPECT_EQ(event.GetGroup().name, "Mirai Technology");
	EXPECT_EQ(event.GetGroup().permission, PERMISSION::ADMINISTRATOR);
}

TEST(EventsTest, BotMuteEvent)
{
	BotMuteEvent event(nullptr);
	event.FromJson(Data::EventData["BotMuteEvent"]);
	EXPECT_EQ(event.GetType(), "BotMuteEvent");

	EXPECT_EQ(event.GetMuteTime(), std::chrono::seconds(600));
	EXPECT_EQ(event.GetOperator().id, 123456789_qq);
	EXPECT_EQ(event.GetOperator().MemberName, "aaa");
	EXPECT_EQ(event.GetOperator().permission, PERMISSION::ADMINISTRATOR);
	EXPECT_EQ(event.GetOperator().SpecialTitle, "xxx");
	EXPECT_EQ(event.GetOperator().JoinTimestamp, 12345678);
	EXPECT_EQ(event.GetOperator().LastSpeakTimestamp, 8765432);
	EXPECT_EQ(event.GetOperator().MuteTimeRemaining.count(), 0);
	EXPECT_EQ(event.GetOperator().group.id, 123456789_gid);
	EXPECT_EQ(event.GetOperator().group.name, "Mirai Technology");
	EXPECT_EQ(event.GetOperator().group.permission, PERMISSION::MEMBER);
}

TEST(EventsTest, BotUnmuteEvent)
{
	BotUnmuteEvent event(nullptr);
	event.FromJson(Data::EventData["BotUnmuteEvent"]);
	EXPECT_EQ(event.GetType(), "BotUnmuteEvent");

	EXPECT_EQ(event.GetOperator().id, 123456789_qq);
	EXPECT_EQ(event.GetOperator().MemberName, "aaa");
	EXPECT_EQ(event.GetOperator().permission, PERMISSION::ADMINISTRATOR);
	EXPECT_EQ(event.GetOperator().SpecialTitle, "xxx");
	EXPECT_EQ(event.GetOperator().JoinTimestamp, 12345678);
	EXPECT_EQ(event.GetOperator().LastSpeakTimestamp, 8765432);
	EXPECT_EQ(event.GetOperator().MuteTimeRemaining.count(), 0);
	EXPECT_EQ(event.GetOperator().group.id, 123456789_gid);
	EXPECT_EQ(event.GetOperator().group.name, "Mirai Technology");
	EXPECT_EQ(event.GetOperator().group.permission, PERMISSION::MEMBER);
}

TEST(EventsTest, BotJoinGroupEvent)
{
	BotJoinGroupEvent event(nullptr);
	event.FromJson(Data::EventData["BotJoinGroupEvent"]);
	EXPECT_EQ(event.GetType(), "BotJoinGroupEvent");

	EXPECT_EQ(event.GetGroup().id, 123456789_gid);
	EXPECT_EQ(event.GetGroup().name, "Mirai Technology");
	EXPECT_EQ(event.GetGroup().permission, PERMISSION::MEMBER);
	EXPECT_FALSE(event.GetInviter());
}

TEST(EventsTest, BotLeaveEventActive)
{
	BotLeaveEventActive event(nullptr);
	event.FromJson(Data::EventData["BotLeaveEventActive"]);
	EXPECT_EQ(event.GetType(), "BotLeaveEventActive");

	EXPECT_EQ(event.GetGroup().id, 123456789_gid);
	EXPECT_EQ(event.GetGroup().name, "Mirai Technology");
	EXPECT_EQ(event.GetGroup().permission, PERMISSION::MEMBER);
}

TEST(EventsTest, BotLeaveEventKick)
{
	BotLeaveEventKick event(nullptr);
	event.FromJson(Data::EventData["BotLeaveEventKick"]);
	EXPECT_EQ(event.GetType(), "BotLeaveEventKick");

	EXPECT_EQ(event.GetGroup().id, 123456789_gid);
	EXPECT_EQ(event.GetGroup().name, "Mirai Technology");
	EXPECT_EQ(event.GetGroup().permission, PERMISSION::MEMBER);
	EXPECT_TRUE(event.GetOperator());
	EXPECT_EQ(event.GetOperator()->id, 1111_qq);
	EXPECT_EQ(event.GetOperator()->MemberName, "Marisa");
	EXPECT_EQ(event.GetOperator()->permission, PERMISSION::OWNER);
	EXPECT_EQ(event.GetOperator()->SpecialTitle, "xxx");
	EXPECT_EQ(event.GetOperator()->JoinTimestamp, 12345678);
	EXPECT_EQ(event.GetOperator()->LastSpeakTimestamp, 8765432);
	EXPECT_EQ(event.GetOperator()->MuteTimeRemaining.count(), 0);
	EXPECT_EQ(event.GetOperator()->group, event.GetGroup());
}

TEST(EventsTest, BotLeaveEventDisband)
{
	BotLeaveEventDisband event(nullptr);
	event.FromJson(Data::EventData["BotLeaveEventDisband"]);
	EXPECT_EQ(event.GetType(), "BotLeaveEventDisband");

	EXPECT_EQ(event.GetGroup().id, 123456789_gid);
	EXPECT_EQ(event.GetGroup().name, "Mirai Technology");
	EXPECT_EQ(event.GetGroup().permission, PERMISSION::MEMBER);
	EXPECT_TRUE(event.GetOperator());
	EXPECT_EQ(event.GetOperator()->id, 1111_qq);
	EXPECT_EQ(event.GetOperator()->MemberName, "Marisa");
	EXPECT_EQ(event.GetOperator()->permission, PERMISSION::OWNER);
	EXPECT_EQ(event.GetOperator()->SpecialTitle, "xxx");
	EXPECT_EQ(event.GetOperator()->JoinTimestamp, 12345678);
	EXPECT_EQ(event.GetOperator()->LastSpeakTimestamp, 8765432);
	EXPECT_EQ(event.GetOperator()->MuteTimeRemaining.count(), 0);
	EXPECT_EQ(event.GetOperator()->group, event.GetGroup());
}

TEST(EventsTest, GroupRecallEvent)
{
	GroupRecallEvent event(nullptr);
	event.FromJson(Data::EventData["GroupRecallEvent"]);
	EXPECT_EQ(event.GetType(), "GroupRecallEvent");

	EXPECT_EQ(event.GetSender(), 123456_qq);
	EXPECT_EQ(event.GetMessageId(), 123456789);
	EXPECT_EQ(event.GetSendTime(), 1234679);
	EXPECT_EQ(event.GetGroup().id, 123456789_gid);
	EXPECT_EQ(event.GetGroup().name, "Mirai Technology");
	EXPECT_EQ(event.GetGroup().permission, PERMISSION::ADMINISTRATOR);
	EXPECT_FALSE(event.GetOperator());
}

TEST(EventsTest, FriendRecallEvent)
{
	FriendRecallEvent event(nullptr);
	event.FromJson(Data::EventData["FriendRecallEvent"]);
	EXPECT_EQ(event.GetType(), "FriendRecallEvent");

	EXPECT_EQ(event.GetSender(), 123456_qq);
	EXPECT_EQ(event.GetMessageId(), 123456789);
	EXPECT_EQ(event.GetSendTime(), 1234679);
	EXPECT_EQ(event.GetOperator(), 123456_qq);
}

TEST(NudgeEvent, NudgeEvent)
{
	NudgeEvent event(nullptr);
	event.FromJson(Data::EventData["NudgeEvent"]);
	EXPECT_EQ(event.GetType(), "NudgeEvent");

	EXPECT_EQ(event.GetSender(), 123456_qq);
	EXPECT_EQ(event.GetTarget().GetTargetKind(), NudgeTarget::GROUP);
	EXPECT_EQ(event.GetTarget().GetGroup(), 8888_gid);
	EXPECT_EQ(event.GetTarget().GetTarget(), 654321_qq);
	EXPECT_EQ(event.GetAction(), "戳了戳");
	EXPECT_EQ(event.GetSuffix(), "的脸");
}

TEST(EventsTest, GroupNameChangeEvent)
{
	GroupNameChangeEvent event(nullptr);
	event.FromJson(Data::EventData["GroupNameChangeEvent"]);
	EXPECT_EQ(event.GetType(), "GroupNameChangeEvent");

	EXPECT_EQ(event.GetOriginal(), "mirai technology");
	EXPECT_EQ(event.GetCurrent(), "MIRAI TECHNOLOGY");
	EXPECT_EQ(event.GetGroup().id, 123456789_gid);
	EXPECT_EQ(event.GetGroup().name, "MIRAI TECHNOLOGY");
	EXPECT_EQ(event.GetGroup().permission, PERMISSION::MEMBER);
	EXPECT_TRUE(event.GetOperator());
	EXPECT_EQ(event.GetOperator()->id, 123456_qq);
	EXPECT_EQ(event.GetOperator()->MemberName, "xxx");
	EXPECT_EQ(event.GetOperator()->permission, PERMISSION::ADMINISTRATOR);
	EXPECT_EQ(event.GetOperator()->SpecialTitle, "xxxx");
	EXPECT_EQ(event.GetOperator()->JoinTimestamp, 12345678);
	EXPECT_EQ(event.GetOperator()->LastSpeakTimestamp, 8765432);
	EXPECT_EQ(event.GetOperator()->MuteTimeRemaining.count(), 0);
	EXPECT_EQ(event.GetOperator()->group, event.GetGroup());
}

TEST(EventsTest, GroupEntranceAnnouncementChangeEvent)
{
	GroupEntranceAnnouncementChangeEvent event(nullptr);
	event.FromJson(Data::EventData["GroupEntranceAnnouncementChangeEvent"]);
	EXPECT_EQ(event.GetType(), "GroupEntranceAnnouncementChangeEvent");

	EXPECT_EQ(event.GetOriginal(), "abc");
	EXPECT_EQ(event.GetCurrent(), "cba");
	EXPECT_EQ(event.GetGroup().id, 123456789_gid);
	EXPECT_EQ(event.GetGroup().name, "Mirai Technology");
	EXPECT_EQ(event.GetGroup().permission, PERMISSION::MEMBER);
	EXPECT_FALSE(event.GetOperator());
}

TEST(EventsTest, GroupMuteAllEvent)
{
	GroupMuteAllEvent event(nullptr);
	event.FromJson(Data::EventData["GroupMuteAllEvent"]);
	EXPECT_EQ(event.GetType(), "GroupMuteAllEvent");

	EXPECT_EQ(event.GetOriginal(), false);
	EXPECT_EQ(event.GetCurrent(), true);
	EXPECT_EQ(event.GetGroup().id, 123456789_gid);
	EXPECT_EQ(event.GetGroup().name, "Mirai Technology");
	EXPECT_EQ(event.GetGroup().permission, PERMISSION::MEMBER);
	EXPECT_FALSE(event.GetOperator());
}

TEST(EventsTest, GroupAllowAnonymousChatEvent)
{
	GroupAllowAnonymousChatEvent event(nullptr);
	event.FromJson(Data::EventData["GroupAllowAnonymousChatEvent"]);
	EXPECT_EQ(event.GetType(), "GroupAllowAnonymousChatEvent");

	EXPECT_EQ(event.GetOriginal(), false);
	EXPECT_EQ(event.GetCurrent(), true);
	EXPECT_EQ(event.GetGroup().id, 123456789_gid);
	EXPECT_EQ(event.GetGroup().name, "Mirai Technology");
	EXPECT_EQ(event.GetGroup().permission, PERMISSION::MEMBER);
	EXPECT_TRUE(event.GetOperator());
	EXPECT_EQ(event.GetOperator()->id, 1234567890_qq);
	EXPECT_EQ(event.GetOperator()->MemberName, "x");
	EXPECT_EQ(event.GetOperator()->permission, PERMISSION::ADMINISTRATOR);
	EXPECT_EQ(event.GetOperator()->SpecialTitle, "xx");
	EXPECT_EQ(event.GetOperator()->JoinTimestamp, 12345678);
	EXPECT_EQ(event.GetOperator()->LastSpeakTimestamp, 8765432);
	EXPECT_EQ(event.GetOperator()->MuteTimeRemaining.count(), 0);
	EXPECT_EQ(event.GetOperator()->group, event.GetGroup());
}

TEST(EventsTest, GroupAllowConfessTalkEvent)
{
	GroupAllowConfessTalkEvent event(nullptr);
	event.FromJson(Data::EventData["GroupAllowConfessTalkEvent"]);
	EXPECT_EQ(event.GetType(), "GroupAllowConfessTalkEvent");

	EXPECT_EQ(event.GetOriginal(), false);
	EXPECT_EQ(event.GetCurrent(), true);
	EXPECT_EQ(event.GetGroup().id, 123456789_gid);
	EXPECT_EQ(event.GetGroup().name, "Mirai Technology");
	EXPECT_EQ(event.GetGroup().permission, PERMISSION::MEMBER);
	EXPECT_FALSE(event.isByBot());
}

TEST(EventsTest, GroupAllowMemberInviteEvent)
{
	GroupAllowMemberInviteEvent event(nullptr);
	event.FromJson(Data::EventData["GroupAllowMemberInviteEvent"]);
	EXPECT_EQ(event.GetType(), "GroupAllowMemberInviteEvent");

	EXPECT_EQ(event.GetOriginal(), false);
	EXPECT_EQ(event.GetCurrent(), true);
	EXPECT_EQ(event.GetGroup().id, 123456789_gid);
	EXPECT_EQ(event.GetGroup().name, "Mirai Technology");
	EXPECT_EQ(event.GetGroup().permission, PERMISSION::MEMBER);
	EXPECT_FALSE(event.GetOperator());
}

TEST(EventsTest, MemberJoinEvent)
{
	MemberJoinEvent event(nullptr);
	event.FromJson(Data::EventData["MemberJoinEvent"]);
	EXPECT_EQ(event.GetType(), "MemberJoinEvent");

	EXPECT_EQ(event.GetMember().id, 1234567890_qq);
	EXPECT_EQ(event.GetMember().MemberName, "aaa");
	EXPECT_EQ(event.GetMember().permission, PERMISSION::MEMBER);
	EXPECT_EQ(event.GetMember().SpecialTitle, "xxx");
	EXPECT_EQ(event.GetMember().JoinTimestamp, 12345678);
	EXPECT_EQ(event.GetMember().LastSpeakTimestamp, 8765432);
	EXPECT_EQ(event.GetMember().MuteTimeRemaining.count(), 0);
	EXPECT_EQ(event.GetMember().group.id, 12345_gid);
	EXPECT_EQ(event.GetMember().group.name, "xxx");
	EXPECT_EQ(event.GetMember().group.permission, PERMISSION::ADMINISTRATOR);
	EXPECT_FALSE(event.GetInviter());
}

TEST(EventsTest, MemberLeaveEventKick)
{
	MemberLeaveEventKick event(nullptr);
	event.FromJson(Data::EventData["MemberLeaveEventKick"]);
	EXPECT_EQ(event.GetType(), "MemberLeaveEventKick");

	EXPECT_EQ(event.GetMember().id, 1234567890_qq);
	EXPECT_EQ(event.GetMember().MemberName, "aaa");
	EXPECT_EQ(event.GetMember().permission, PERMISSION::MEMBER);
	EXPECT_EQ(event.GetMember().SpecialTitle, "");
	EXPECT_EQ(event.GetMember().JoinTimestamp, 12345678);
	EXPECT_EQ(event.GetMember().LastSpeakTimestamp, 8765432);
	EXPECT_EQ(event.GetMember().MuteTimeRemaining.count(), 0);
	EXPECT_EQ(event.GetMember().group.id, 12345_gid);
	EXPECT_EQ(event.GetMember().group.name, "xxx");
	EXPECT_EQ(event.GetMember().group.permission, PERMISSION::MEMBER);
	EXPECT_TRUE(event.GetOperator());
	EXPECT_EQ(event.GetOperator()->id, 88888888_qq);
	EXPECT_EQ(event.GetOperator()->MemberName, "bbb");
	EXPECT_EQ(event.GetOperator()->permission, PERMISSION::OWNER);
	EXPECT_EQ(event.GetOperator()->SpecialTitle, "ccc");
	EXPECT_EQ(event.GetOperator()->JoinTimestamp, 0);
	EXPECT_EQ(event.GetOperator()->LastSpeakTimestamp, 6666666);
	EXPECT_EQ(event.GetOperator()->MuteTimeRemaining.count(), 0);
	EXPECT_EQ(event.GetOperator()->group, event.GetMember().group);
}

TEST(EventsTest, MemberLeaveEventQuit)
{
	MemberLeaveEventQuit event(nullptr);
	event.FromJson(Data::EventData["MemberLeaveEventQuit"]);
	EXPECT_EQ(event.GetType(), "MemberLeaveEventQuit");

	EXPECT_EQ(event.GetMember().id, 1234_qq);
	EXPECT_EQ(event.GetMember().MemberName, "run");
	EXPECT_EQ(event.GetMember().permission, PERMISSION::MEMBER);
	EXPECT_EQ(event.GetMember().SpecialTitle, "");
	EXPECT_EQ(event.GetMember().JoinTimestamp, 0);
	EXPECT_EQ(event.GetMember().LastSpeakTimestamp, 0);
	EXPECT_EQ(event.GetMember().MuteTimeRemaining.count(), 0);
	EXPECT_EQ(event.GetMember().group.id, 123456789_gid);
	EXPECT_EQ(event.GetMember().group.name, "Mirai Technology");
	EXPECT_EQ(event.GetMember().group.permission, PERMISSION::MEMBER);
}

TEST(EventsTest, MemberCardChangeEvent)
{
	MemberCardChangeEvent event(nullptr);
	event.FromJson(Data::EventData["MemberCardChangeEvent"]);
	EXPECT_EQ(event.GetType(), "MemberCardChangeEvent");

	EXPECT_EQ(event.GetOriginal(), "old");
	EXPECT_EQ(event.GetCurrent(), "new");
	EXPECT_EQ(event.GetMember().id, 1234567890_qq);
	EXPECT_EQ(event.GetMember().MemberName, "aaa");
	EXPECT_EQ(event.GetMember().permission, PERMISSION::MEMBER);
	EXPECT_EQ(event.GetMember().SpecialTitle, "xxx");
	EXPECT_EQ(event.GetMember().JoinTimestamp, 12345678);
	EXPECT_EQ(event.GetMember().LastSpeakTimestamp, 8765432);
	EXPECT_EQ(event.GetMember().MuteTimeRemaining.count(), 0);
	EXPECT_EQ(event.GetMember().group.id, 12345_gid);
	EXPECT_EQ(event.GetMember().group.name, "xxx");
	EXPECT_EQ(event.GetMember().group.permission, PERMISSION::MEMBER);
}

TEST(EventsTest, MemberSpecialTitleChangeEvent)
{
	MemberSpecialTitleChangeEvent event(nullptr);
	event.FromJson(Data::EventData["MemberSpecialTitleChangeEvent"]);
	EXPECT_EQ(event.GetType(), "MemberSpecialTitleChangeEvent");

	EXPECT_EQ(event.GetOriginal(), "origin");
	EXPECT_EQ(event.GetCurrent(), "new");
	EXPECT_EQ(event.GetMember().id, 1234_qq);
	EXPECT_EQ(event.GetMember().MemberName, "bbb");
	EXPECT_EQ(event.GetMember().permission, PERMISSION::MEMBER);
	EXPECT_EQ(event.GetMember().SpecialTitle, "");
	EXPECT_EQ(event.GetMember().JoinTimestamp, 0);
	EXPECT_EQ(event.GetMember().LastSpeakTimestamp, 0);
	EXPECT_EQ(event.GetMember().MuteTimeRemaining.count(), 0);
	EXPECT_EQ(event.GetMember().group.id, 123456789_gid);
	EXPECT_EQ(event.GetMember().group.name, "Mirai Technology");
	EXPECT_EQ(event.GetMember().group.permission, PERMISSION::MEMBER);
}

TEST(EventsTest, MemberPermissionChangeEvent)
{
	MemberPermissionChangeEvent event(nullptr);
	event.FromJson(Data::EventData["MemberPermissionChangeEvent"]);
	EXPECT_EQ(event.GetType(), "MemberPermissionChangeEvent");

	EXPECT_EQ(event.GetOriginal(), PERMISSION::MEMBER);
	EXPECT_EQ(event.GetCurrent(), PERMISSION::ADMINISTRATOR);
	EXPECT_EQ(event.GetMember().id, 1234_qq);
	EXPECT_EQ(event.GetMember().MemberName, "bbb");
	EXPECT_EQ(event.GetMember().permission, PERMISSION::MEMBER);
	EXPECT_EQ(event.GetMember().SpecialTitle, "");
	EXPECT_EQ(event.GetMember().JoinTimestamp, 0);
	EXPECT_EQ(event.GetMember().LastSpeakTimestamp, 0);
	EXPECT_EQ(event.GetMember().MuteTimeRemaining.count(), 0);
	EXPECT_EQ(event.GetMember().group.id, 123456789_gid);
	EXPECT_EQ(event.GetMember().group.name, "Mirai Technology");
	EXPECT_EQ(event.GetMember().group.permission, PERMISSION::MEMBER);
}

TEST(EventsTest, MemberMuteEvent)
{
	MemberMuteEvent event(nullptr);
	event.FromJson(Data::EventData["MemberMuteEvent"]);
	EXPECT_EQ(event.GetType(), "MemberMuteEvent");

	EXPECT_EQ(event.GetDuration(), std::chrono::milliseconds(1000 * 600));
	EXPECT_EQ(event.GetMember().id, 1234_qq);
	EXPECT_EQ(event.GetMember().MemberName, "aaa");
	EXPECT_EQ(event.GetMember().permission, PERMISSION::MEMBER);
	EXPECT_EQ(event.GetMember().SpecialTitle, "xxx");
	EXPECT_EQ(event.GetMember().JoinTimestamp, 12345678);
	EXPECT_EQ(event.GetMember().LastSpeakTimestamp, 8765432);
	EXPECT_EQ(event.GetMember().MuteTimeRemaining.count(), 600);
	EXPECT_EQ(event.GetMember().group.id, 123456789_gid);
	EXPECT_EQ(event.GetMember().group.name, "Mirai Technology");
	EXPECT_EQ(event.GetMember().group.permission, PERMISSION::MEMBER);
	EXPECT_TRUE(event.GetOperator());
	EXPECT_EQ(event.GetOperator()->id, 4321_qq);
	EXPECT_EQ(event.GetOperator()->MemberName, "bbb");
	EXPECT_EQ(event.GetOperator()->permission, PERMISSION::OWNER);
	EXPECT_EQ(event.GetOperator()->SpecialTitle, "xxx");
	EXPECT_EQ(event.GetOperator()->JoinTimestamp, 1619334595);
	EXPECT_EQ(event.GetOperator()->LastSpeakTimestamp, 1660370799);
	EXPECT_EQ(event.GetOperator()->MuteTimeRemaining.count(), 0);
	EXPECT_EQ(event.GetOperator()->group, event.GetMember().group);
}

TEST(EventsTest, MemberUnmuteEvent)
{
	MemberUnmuteEvent event(nullptr);
	event.FromJson(Data::EventData["MemberUnmuteEvent"]);
	EXPECT_EQ(event.GetType(), "MemberUnmuteEvent");

	EXPECT_EQ(event.GetMember().id, 1234_qq);
	EXPECT_EQ(event.GetMember().MemberName, "aaa");
	EXPECT_EQ(event.GetMember().permission, PERMISSION::MEMBER);
	EXPECT_EQ(event.GetMember().SpecialTitle, "xxx");
	EXPECT_EQ(event.GetMember().JoinTimestamp, 12345678);
	EXPECT_EQ(event.GetMember().LastSpeakTimestamp, 8765432);
	EXPECT_EQ(event.GetMember().MuteTimeRemaining.count(), 0);
	EXPECT_EQ(event.GetMember().group.id, 123456789_gid);
	EXPECT_EQ(event.GetMember().group.name, "Mirai Technology");
	EXPECT_EQ(event.GetMember().group.permission, PERMISSION::OWNER);
	EXPECT_FALSE(event.GetOperator());
}

TEST(EventsTest, MemberHonorChangeEvent)
{
	MemberHonorChangeEvent event(nullptr);
	event.FromJson(Data::EventData["MemberHonorChangeEvent"]);
	EXPECT_EQ(event.GetType(), "MemberHonorChangeEvent");

	EXPECT_EQ(event.GetMember().id, 1234_qq);
	EXPECT_EQ(event.GetMember().MemberName, "aaa");
	EXPECT_EQ(event.GetMember().permission, PERMISSION::MEMBER);
	EXPECT_EQ(event.GetMember().SpecialTitle, "xxx");
	EXPECT_EQ(event.GetMember().JoinTimestamp, 12345678);
	EXPECT_EQ(event.GetMember().LastSpeakTimestamp, 8765432);
	EXPECT_EQ(event.GetMember().MuteTimeRemaining.count(), 0);
	EXPECT_EQ(event.GetMember().group.id, 123456789_gid);
	EXPECT_EQ(event.GetMember().group.name, "Mirai Technology");
	EXPECT_EQ(event.GetMember().group.permission, PERMISSION::MEMBER);
	EXPECT_EQ(event.GetAction(), MemberHonorChangeEvent::ACHIEVE);
	EXPECT_EQ(event.GetHonor(), "龙王");
}

TEST(EventsTest, NewFriendRequestEvent)
{
	NewFriendRequestEvent event(nullptr);
	event.FromJson(Data::EventData["NewFriendRequestEvent"]);
	EXPECT_EQ(event.GetType(), "NewFriendRequestEvent");

	EXPECT_EQ(event.GetEventId(), 12345678);
	EXPECT_EQ(event.GetUserId(), 123456_qq);
	EXPECT_EQ(event.GetGroupId(), 0_gid);
	EXPECT_EQ(event.GetNickname(), "xxx");
	EXPECT_FALSE(event.isFromGroup());
	EXPECT_EQ(event.GetMessage(), "hehe");
}

TEST(EventsTest, MemberJoinRequestEvent)
{
	MemberJoinRequestEvent event(nullptr);
	event.FromJson(Data::EventData["MemberJoinRequestEvent"]);
	EXPECT_EQ(event.GetType(), "MemberJoinRequestEvent");

	EXPECT_EQ(event.GetEventId(), 12345678);
	EXPECT_EQ(event.GetUserId(), 123456_qq);
	EXPECT_EQ(event.GetGroupId(), 654321_gid);
	EXPECT_EQ(event.GetGroupName(), "Group");
	EXPECT_EQ(event.GetNickname(), "xxx");
	EXPECT_EQ(event.GetMessage(), "hehe");
}

TEST(EventsTest, BotInvitedJoinGroupRequestEvent)
{
	BotInvitedJoinGroupRequestEvent event(nullptr);
	event.FromJson(Data::EventData["BotInvitedJoinGroupRequestEvent"]);
	EXPECT_EQ(event.GetType(), "BotInvitedJoinGroupRequestEvent");

	EXPECT_EQ(event.GetEventId(), 12345678);
	EXPECT_EQ(event.GetUserId(), 123456_qq);
	EXPECT_EQ(event.GetGroupId(), 654321_gid);
	EXPECT_EQ(event.GetGroupName(), "Group");
	EXPECT_EQ(event.GetNickname(), "xxx");
	EXPECT_EQ(event.GetMessage(), "hehe");
}

TEST(EventsTest, OtherClientOnlineEvent)
{
	OtherClientOnlineEvent event(nullptr);
	event.FromJson(Data::EventData["OtherClientOnlineEvent"]);
	EXPECT_EQ(event.GetType(), "OtherClientOnlineEvent");

	EXPECT_EQ(event.GetClient().id, 1);
	EXPECT_EQ(event.GetClient().platform, "WINDOWS");
	EXPECT_EQ(event.GetKind(), 69899);
}

TEST(EventsTest, OtherClientOfflineEvent)
{
	OtherClientOfflineEvent event(nullptr);
	event.FromJson(Data::EventData["OtherClientOfflineEvent"]);
	EXPECT_EQ(event.GetType(), "OtherClientOfflineEvent");

	EXPECT_EQ(event.GetClient().id, 1);
	EXPECT_EQ(event.GetClient().platform, "WINDOWS");
}

TEST(EventsTest, CommandExecutedEvent)
{
	CommandExecutedEvent event(nullptr);
	event.FromJson(Data::EventData["CommandExecutedEvent"]);
	EXPECT_EQ(event.GetType(), "CommandExecutedEvent");

	EXPECT_EQ(event.GetName(), "shutdown");
	EXPECT_TRUE(event.GetFriendSender());
	EXPECT_EQ(event.GetFriendSender()->id, 123123_qq);
	EXPECT_EQ(event.GetFriendSender()->nickname, "nick");
	EXPECT_EQ(event.GetFriendSender()->remark, "remark");
	EXPECT_FALSE(event.GetMemberSender());
	EXPECT_EQ(event.GetCommandArgs().size(), 1);
	EXPECT_EQ(event.GetCommandArgs()[0]->GetType(), "Plain");
	EXPECT_EQ(event.GetCommandArgs().GetAt<PlainMessage>(0).GetText(), "myself");
}

TEST(EventsTest, FriendMessageEvent)
{
	FriendMessageEvent event(nullptr);
	event.FromJson(Data::EventData["FriendMessage"]);
	EXPECT_EQ(event.GetType(), "FriendMessage");

	EXPECT_EQ(event.GetSender().id, 123_qq);
	EXPECT_EQ(event.GetSender().nickname, "xxx");
	EXPECT_EQ(event.GetSender().remark, "who");
	EXPECT_EQ(event.GetMessage().size(), 1);
	EXPECT_EQ(event.GetMessage()[0]->GetType(), "Plain");
	EXPECT_EQ(event.GetMessage().GetAt<PlainMessage>(0).GetText(), "hello");
}

TEST(EventsTest, GroupMessageEvent)
{
	GroupMessageEvent event(nullptr);
	event.FromJson(Data::EventData["GroupMessage"]);
	EXPECT_EQ(event.GetType(), "GroupMessage");

	EXPECT_EQ(event.GetSender().id, 123_qq);
	EXPECT_EQ(event.GetSender().MemberName, "aaa");
	EXPECT_EQ(event.GetSender().SpecialTitle, "xxx");
	EXPECT_EQ(event.GetSender().permission, PERMISSION::MEMBER);
	EXPECT_EQ(event.GetSender().JoinTimestamp, 123);
	EXPECT_EQ(event.GetSender().LastSpeakTimestamp, 456);
	EXPECT_EQ(event.GetSender().MuteTimeRemaining.count(), 0);
	EXPECT_EQ(event.GetSender().group.id, 1234567890_qq);
	EXPECT_EQ(event.GetSender().group.name, "bbb");
	EXPECT_EQ(event.GetSender().group.permission, PERMISSION::ADMINISTRATOR);
	EXPECT_EQ(event.GetMessage().size(), 1);
	EXPECT_EQ(event.GetMessage()[0]->GetType(), "Plain");
	EXPECT_EQ(event.GetMessage().GetAt<PlainMessage>(0).GetText(), "hello");
}

TEST(EventsTest, TempMessageEvent)
{
	TempMessageEvent event(nullptr);
	event.FromJson(Data::EventData["TempMessage"]);
	EXPECT_EQ(event.GetType(), "TempMessage");

	EXPECT_EQ(event.GetSender().id, 123_qq);
	EXPECT_EQ(event.GetSender().MemberName, "aaa");
	EXPECT_EQ(event.GetSender().SpecialTitle, "xxx");
	EXPECT_EQ(event.GetSender().permission, PERMISSION::MEMBER);
	EXPECT_EQ(event.GetSender().JoinTimestamp, 123);
	EXPECT_EQ(event.GetSender().LastSpeakTimestamp, 456);
	EXPECT_EQ(event.GetSender().MuteTimeRemaining.count(), 0);
	EXPECT_EQ(event.GetSender().group.id, 1234567890_qq);
	EXPECT_EQ(event.GetSender().group.name, "bbb");
	EXPECT_EQ(event.GetSender().group.permission, PERMISSION::ADMINISTRATOR);
	EXPECT_EQ(event.GetMessage().size(), 1);
	EXPECT_EQ(event.GetMessage()[0]->GetType(), "Plain");
	EXPECT_EQ(event.GetMessage().GetAt<PlainMessage>(0).GetText(), "hello");
}

TEST(EventsTest, StrangerMessageEvent)
{
	StrangerMessageEvent event(nullptr);
	event.FromJson(Data::EventData["StrangerMessage"]);
	EXPECT_EQ(event.GetType(), "StrangerMessage");

	EXPECT_EQ(event.GetSender().id, 123_qq);
	EXPECT_EQ(event.GetSender().nickname, "xxx");
	EXPECT_EQ(event.GetSender().remark, "who");
	EXPECT_EQ(event.GetMessage().size(), 1);
	EXPECT_EQ(event.GetMessage()[0]->GetType(), "Plain");
	EXPECT_EQ(event.GetMessage().GetAt<PlainMessage>(0).GetText(), "hello");
}

TEST(EventsTest, OtherClientMessageEvent)
{
	OtherClientMessageEvent event(nullptr);
	event.FromJson(Data::EventData["OtherClientMessage"]);
	EXPECT_EQ(event.GetType(), "OtherClientMessage");

	EXPECT_EQ(event.GetSender().id, 123);
	EXPECT_EQ(event.GetSender().platform, "MOBILE");
	EXPECT_EQ(event.GetMessage().size(), 1);
	EXPECT_EQ(event.GetMessage()[0]->GetType(), "Plain");
	EXPECT_EQ(event.GetMessage().GetAt<PlainMessage>(0).GetText(), "hello");
}

TEST(EventsTest, FriendSyncMessageEvent)
{
	FriendSyncMessageEvent event(nullptr);
	event.FromJson(Data::EventData["FriendSyncMessage"]);
	EXPECT_EQ(event.GetType(), "FriendSyncMessage");

	EXPECT_EQ(event.GetFriend().id, 123_qq);
	EXPECT_EQ(event.GetFriend().nickname, "xxx");
	EXPECT_EQ(event.GetFriend().remark, "who");
	EXPECT_EQ(event.GetMessage().size(), 1);
	EXPECT_EQ(event.GetMessage()[0]->GetType(), "Plain");
	EXPECT_EQ(event.GetMessage().GetAt<PlainMessage>(0).GetText(), "hello");
}

TEST(EventsTest, GroupSyncMessageEvent)
{
	GroupSyncMessageEvent event(nullptr);
	event.FromJson(Data::EventData["GroupSyncMessage"]);
	EXPECT_EQ(event.GetType(), "GroupSyncMessage");

	EXPECT_EQ(event.GetGroup().id, 1234567890_gid);
	EXPECT_EQ(event.GetGroup().name, "bbb");
	EXPECT_EQ(event.GetGroup().permission, PERMISSION::MEMBER);
	EXPECT_EQ(event.GetMessage().size(), 1);
	EXPECT_EQ(event.GetMessage()[0]->GetType(), "Plain");
	EXPECT_EQ(event.GetMessage().GetAt<PlainMessage>(0).GetText(), "hello");
}

TEST(EventsTest, TempSyncMessageEvent)
{
	TempSyncMessageEvent event(nullptr);
	event.FromJson(Data::EventData["TempSyncMessage"]);
	EXPECT_EQ(event.GetType(), "TempSyncMessage");

	EXPECT_EQ(event.GetGroupMember().id, 123_qq);
	EXPECT_EQ(event.GetGroupMember().MemberName, "aaa");
	EXPECT_EQ(event.GetGroupMember().SpecialTitle, "xxx");
	EXPECT_EQ(event.GetGroupMember().permission, PERMISSION::MEMBER);
	EXPECT_EQ(event.GetGroupMember().JoinTimestamp, 123);
	EXPECT_EQ(event.GetGroupMember().LastSpeakTimestamp, 456);
	EXPECT_EQ(event.GetGroupMember().MuteTimeRemaining.count(), 0);
	EXPECT_EQ(event.GetGroupMember().group.id, 1234567890_qq);
	EXPECT_EQ(event.GetGroupMember().group.name, "bbb");
	EXPECT_EQ(event.GetGroupMember().group.permission, PERMISSION::ADMINISTRATOR);
	EXPECT_EQ(event.GetMessage().size(), 1);
	EXPECT_EQ(event.GetMessage()[0]->GetType(), "Plain");
	EXPECT_EQ(event.GetMessage().GetAt<PlainMessage>(0).GetText(), "hello");
}

TEST(EventsTest, StrangerSyncMessageEvent)
{
	StrangerSyncMessageEvent event(nullptr);
	event.FromJson(Data::EventData["StrangerSyncMessage"]);
	EXPECT_EQ(event.GetType(), "StrangerSyncMessage");

	EXPECT_EQ(event.GetStranger().id, 123_qq);
	EXPECT_EQ(event.GetStranger().nickname, "xxx");
	EXPECT_EQ(event.GetStranger().remark, "who");
	EXPECT_EQ(event.GetMessage().size(), 1);
	EXPECT_EQ(event.GetMessage()[0]->GetType(), "Plain");
	EXPECT_EQ(event.GetMessage().GetAt<PlainMessage>(0).GetText(), "hello");
}
