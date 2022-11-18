#include <chrono>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include <libmirai/Events/Events.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include <libmirai/Serialization/Events/EventBase.hpp>
#include <libmirai/Serialization/Types/Types.hpp>

#include "JsonData.hpp"

using namespace Mirai;
using json = nlohmann::json;

TEST(EventsTest, BotOnlineEvent)
{
	BotOnlineEvent event{};
	Data::EventData["BotOnlineEvent"].get_to(event);
	EXPECT_EQ(event._TYPE_, "BotOnlineEvent");

	EXPECT_EQ(event.GetQQ(), 123456_qq);
}

TEST(EventsTest, BotOfflineEventActive)
{
	BotOfflineEventActive event{};
	Data::EventData["BotOfflineEventActive"].get_to(event);
	EXPECT_EQ(event._TYPE_, "BotOfflineEventActive");

	EXPECT_EQ(event.GetQQ(), 123456_qq);
}

TEST(EventsTest, BotOfflineEventDropped)
{
	BotOfflineEventDropped event{};
	Data::EventData["BotOfflineEventDropped"].get_to(event);
	EXPECT_EQ(event._TYPE_, "BotOfflineEventDropped");

	EXPECT_EQ(event.GetQQ(), 123456_qq);
}

TEST(EventsTest, FriendInputStatusChangedEvent)
{
	FriendInputStatusChangedEvent event{};
	Data::EventData["FriendInputStatusChangedEvent"].get_to(event);
	EXPECT_EQ(event._TYPE_, "FriendInputStatusChangedEvent");

	EXPECT_TRUE(event.isInputting());
	EXPECT_EQ(event.GetFriend().id, 123123_qq);
	EXPECT_EQ(event.GetFriend().nickname, "nick");
	EXPECT_EQ(event.GetFriend().remark, "remark");
}

TEST(EventsTest, FriendNickChangedEvent)
{
	FriendNickChangedEvent event{};
	Data::EventData["FriendNickChangedEvent"].get_to(event);
	EXPECT_EQ(event._TYPE_, "FriendNickChangedEvent");

	EXPECT_EQ(event.GetFriend().id, 123123_qq);
	EXPECT_EQ(event.GetFriend().nickname, "new nickname");
	EXPECT_EQ(event.GetFriend().remark, "remark");
	EXPECT_EQ(event.GetOriginalNickname(), "origin nickname");
	EXPECT_EQ(event.GetNewNickname(), "new nickname");
}

TEST(EventsTest, BotGroupPermissionChangeEvent)
{
	BotGroupPermissionChangeEvent event{};
	Data::EventData["BotGroupPermissionChangeEvent"].get_to(event);
	EXPECT_EQ(event._TYPE_, "BotGroupPermissionChangeEvent");

	EXPECT_EQ(event.GetOriginal(), PERMISSION::MEMBER);
	EXPECT_EQ(event.GetCurrent(), PERMISSION::ADMINISTRATOR);
	EXPECT_EQ(event.GetGroup().id, 123456789_gid);
	EXPECT_EQ(event.GetGroup().name, "Mirai Technology");
	EXPECT_EQ(event.GetGroup().permission, PERMISSION::ADMINISTRATOR);
}

TEST(EventsTest, BotMuteEvent)
{
	BotMuteEvent event{};
	Data::EventData["BotMuteEvent"].get_to(event);
	EXPECT_EQ(event._TYPE_, "BotMuteEvent");

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
	BotUnmuteEvent event{};
	Data::EventData["BotUnmuteEvent"].get_to(event);
	EXPECT_EQ(event._TYPE_, "BotUnmuteEvent");

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
	BotJoinGroupEvent event{};
	Data::EventData["BotJoinGroupEvent"].get_to(event);
	EXPECT_EQ(event._TYPE_, "BotJoinGroupEvent");

	EXPECT_EQ(event.GetGroup().id, 123456789_gid);
	EXPECT_EQ(event.GetGroup().name, "Mirai Technology");
	EXPECT_EQ(event.GetGroup().permission, PERMISSION::MEMBER);
	EXPECT_FALSE(event.GetInviter());
}

TEST(EventsTest, BotLeaveEventActive)
{
	BotLeaveEventActive event{};
	Data::EventData["BotLeaveEventActive"].get_to(event);
	EXPECT_EQ(event._TYPE_, "BotLeaveEventActive");

	EXPECT_EQ(event.GetGroup().id, 123456789_gid);
	EXPECT_EQ(event.GetGroup().name, "Mirai Technology");
	EXPECT_EQ(event.GetGroup().permission, PERMISSION::MEMBER);
}

TEST(EventsTest, BotLeaveEventKick)
{
	BotLeaveEventKick event{};
	Data::EventData["BotLeaveEventKick"].get_to(event);
	EXPECT_EQ(event._TYPE_, "BotLeaveEventKick");

	EXPECT_EQ(event.GetGroup().id, 123456789_gid);
	EXPECT_EQ(event.GetGroup().name, "Mirai Technology");
	EXPECT_EQ(event.GetGroup().permission, PERMISSION::MEMBER);
	EXPECT_EQ(event.GetOperator().id, 1111_qq);
	EXPECT_EQ(event.GetOperator().MemberName, "Marisa");
	EXPECT_EQ(event.GetOperator().permission, PERMISSION::OWNER);
	EXPECT_EQ(event.GetOperator().SpecialTitle, "xxx");
	EXPECT_EQ(event.GetOperator().JoinTimestamp, 12345678);
	EXPECT_EQ(event.GetOperator().LastSpeakTimestamp, 8765432);
	EXPECT_EQ(event.GetOperator().MuteTimeRemaining.count(), 0);
	EXPECT_EQ(event.GetOperator().group, event.GetGroup());
}

TEST(EventsTest, BotLeaveEventDisband)
{
	BotLeaveEventDisband event{};
	Data::EventData["BotLeaveEventDisband"].get_to(event);
	EXPECT_EQ(event._TYPE_, "BotLeaveEventDisband");

	EXPECT_EQ(event.GetGroup().id, 123456789_gid);
	EXPECT_EQ(event.GetGroup().name, "Mirai Technology");
	EXPECT_EQ(event.GetGroup().permission, PERMISSION::MEMBER);
	EXPECT_EQ(event.GetOperator().id, 1111_qq);
	EXPECT_EQ(event.GetOperator().MemberName, "Marisa");
	EXPECT_EQ(event.GetOperator().permission, PERMISSION::OWNER);
	EXPECT_EQ(event.GetOperator().SpecialTitle, "xxx");
	EXPECT_EQ(event.GetOperator().JoinTimestamp, 12345678);
	EXPECT_EQ(event.GetOperator().LastSpeakTimestamp, 8765432);
	EXPECT_EQ(event.GetOperator().MuteTimeRemaining.count(), 0);
	EXPECT_EQ(event.GetOperator().group, event.GetGroup());
}

TEST(EventsTest, GroupRecallEvent)
{
	GroupRecallEvent event{};
	Data::EventData["GroupRecallEvent"].get_to(event);
	EXPECT_EQ(event._TYPE_, "GroupRecallEvent");

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
	FriendRecallEvent event{};
	Data::EventData["FriendRecallEvent"].get_to(event);
	EXPECT_EQ(event._TYPE_, "FriendRecallEvent");

	EXPECT_EQ(event.GetSender(), 123456_qq);
	EXPECT_EQ(event.GetMessageId(), 123456789);
	EXPECT_EQ(event.GetSendTime(), 1234679);
	EXPECT_EQ(event.GetOperator(), 123456_qq);
}

TEST(NudgeEvent, NudgeEvent)
{
	NudgeEvent event{};
	Data::EventData["NudgeEvent"].get_to(event);
	EXPECT_EQ(event._TYPE_, "NudgeEvent");

	EXPECT_EQ(event.GetSender(), 123456_qq);
	EXPECT_EQ(event.GetTarget().GetNudgeType(), NudgeType::GROUP);
	EXPECT_EQ(event.GetTarget().GetGroup(), 8888_gid);
	EXPECT_EQ(event.GetTarget().GetTarget(), 654321_qq);
	EXPECT_EQ(event.GetAction(), "戳了戳");
	EXPECT_EQ(event.GetSuffix(), "的脸");
}

TEST(EventsTest, GroupNameChangeEvent)
{
	GroupNameChangeEvent event{};
	Data::EventData["GroupNameChangeEvent"].get_to(event);
	EXPECT_EQ(event._TYPE_, "GroupNameChangeEvent");

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
	GroupEntranceAnnouncementChangeEvent event{};
	Data::EventData["GroupEntranceAnnouncementChangeEvent"].get_to(event);
	EXPECT_EQ(event._TYPE_, "GroupEntranceAnnouncementChangeEvent");

	EXPECT_EQ(event.GetOriginal(), "abc");
	EXPECT_EQ(event.GetCurrent(), "cba");
	EXPECT_EQ(event.GetGroup().id, 123456789_gid);
	EXPECT_EQ(event.GetGroup().name, "Mirai Technology");
	EXPECT_EQ(event.GetGroup().permission, PERMISSION::MEMBER);
	EXPECT_FALSE(event.GetOperator());
}

TEST(EventsTest, GroupMuteAllEvent)
{
	GroupMuteAllEvent event{};
	Data::EventData["GroupMuteAllEvent"].get_to(event);
	EXPECT_EQ(event._TYPE_, "GroupMuteAllEvent");

	EXPECT_EQ(event.GetOriginal(), false);
	EXPECT_EQ(event.GetCurrent(), true);
	EXPECT_EQ(event.GetGroup().id, 123456789_gid);
	EXPECT_EQ(event.GetGroup().name, "Mirai Technology");
	EXPECT_EQ(event.GetGroup().permission, PERMISSION::MEMBER);
	EXPECT_FALSE(event.GetOperator());
}

TEST(EventsTest, GroupAllowAnonymousChatEvent)
{
	GroupAllowAnonymousChatEvent event{};
	Data::EventData["GroupAllowAnonymousChatEvent"].get_to(event);
	EXPECT_EQ(event._TYPE_, "GroupAllowAnonymousChatEvent");

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
	GroupAllowConfessTalkEvent event{};
	Data::EventData["GroupAllowConfessTalkEvent"].get_to(event);
	EXPECT_EQ(event._TYPE_, "GroupAllowConfessTalkEvent");

	EXPECT_EQ(event.GetOriginal(), false);
	EXPECT_EQ(event.GetCurrent(), true);
	EXPECT_EQ(event.GetGroup().id, 123456789_gid);
	EXPECT_EQ(event.GetGroup().name, "Mirai Technology");
	EXPECT_EQ(event.GetGroup().permission, PERMISSION::MEMBER);
	EXPECT_FALSE(event.isByBot());
}

TEST(EventsTest, GroupAllowMemberInviteEvent)
{
	GroupAllowMemberInviteEvent event{};
	Data::EventData["GroupAllowMemberInviteEvent"].get_to(event);
	EXPECT_EQ(event._TYPE_, "GroupAllowMemberInviteEvent");

	EXPECT_EQ(event.GetOriginal(), false);
	EXPECT_EQ(event.GetCurrent(), true);
	EXPECT_EQ(event.GetGroup().id, 123456789_gid);
	EXPECT_EQ(event.GetGroup().name, "Mirai Technology");
	EXPECT_EQ(event.GetGroup().permission, PERMISSION::MEMBER);
	EXPECT_FALSE(event.GetOperator());
}

TEST(EventsTest, MemberJoinEvent)
{
	MemberJoinEvent event{};
	Data::EventData["MemberJoinEvent"].get_to(event);
	EXPECT_EQ(event._TYPE_, "MemberJoinEvent");

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
	MemberLeaveEventKick event{};
	Data::EventData["MemberLeaveEventKick"].get_to(event);
	EXPECT_EQ(event._TYPE_, "MemberLeaveEventKick");

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
	MemberLeaveEventQuit event{};
	Data::EventData["MemberLeaveEventQuit"].get_to(event);
	EXPECT_EQ(event._TYPE_, "MemberLeaveEventQuit");

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
	MemberCardChangeEvent event{};
	Data::EventData["MemberCardChangeEvent"].get_to(event);
	EXPECT_EQ(event._TYPE_, "MemberCardChangeEvent");

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
	MemberSpecialTitleChangeEvent event{};
	Data::EventData["MemberSpecialTitleChangeEvent"].get_to(event);
	EXPECT_EQ(event._TYPE_, "MemberSpecialTitleChangeEvent");

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
	MemberPermissionChangeEvent event{};
	Data::EventData["MemberPermissionChangeEvent"].get_to(event);
	EXPECT_EQ(event._TYPE_, "MemberPermissionChangeEvent");

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
	MemberMuteEvent event{};
	Data::EventData["MemberMuteEvent"].get_to(event);
	EXPECT_EQ(event._TYPE_, "MemberMuteEvent");

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
	MemberUnmuteEvent event{};
	Data::EventData["MemberUnmuteEvent"].get_to(event);
	EXPECT_EQ(event._TYPE_, "MemberUnmuteEvent");

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
	MemberHonorChangeEvent event{};
	Data::EventData["MemberHonorChangeEvent"].get_to(event);
	EXPECT_EQ(event._TYPE_, "MemberHonorChangeEvent");

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
	EXPECT_EQ(event.GetAction(), HonorChangeType::ACHIEVE);
	EXPECT_EQ(event.GetHonor(), "龙王");
}

TEST(EventsTest, NewFriendRequestEvent)
{
	NewFriendRequestEvent event{};
	Data::EventData["NewFriendRequestEvent"].get_to(event);
	EXPECT_EQ(event._TYPE_, "NewFriendRequestEvent");

	EXPECT_EQ(event.GetEventId(), 12345678);
	EXPECT_EQ(event.GetUserId(), 123456_qq);
	EXPECT_EQ(event.GetGroupId(), 0_gid);
	EXPECT_EQ(event.GetNickname(), "xxx");
	EXPECT_FALSE(event.isFromGroup());
	EXPECT_EQ(event.GetMessage(), "hehe");
}

TEST(EventsTest, MemberJoinRequestEvent)
{
	MemberJoinRequestEvent event{};
	Data::EventData["MemberJoinRequestEvent"].get_to(event);
	EXPECT_EQ(event._TYPE_, "MemberJoinRequestEvent");

	EXPECT_EQ(event.GetEventId(), 12345678);
	EXPECT_EQ(event.GetUserId(), 123456_qq);
	EXPECT_EQ(event.GetGroupId(), 654321_gid);
	EXPECT_EQ(event.GetGroupName(), "Group");
	EXPECT_EQ(event.GetNickname(), "xxx");
	EXPECT_EQ(event.GetMessage(), "hehe");
}

TEST(EventsTest, BotInvitedJoinGroupRequestEvent)
{
	BotInvitedJoinGroupRequestEvent event{};
	Data::EventData["BotInvitedJoinGroupRequestEvent"].get_to(event);
	EXPECT_EQ(event._TYPE_, "BotInvitedJoinGroupRequestEvent");

	EXPECT_EQ(event.GetEventId(), 12345678);
	EXPECT_EQ(event.GetUserId(), 123456_qq);
	EXPECT_EQ(event.GetGroupId(), 654321_gid);
	EXPECT_EQ(event.GetGroupName(), "Group");
	EXPECT_EQ(event.GetNickname(), "xxx");
	EXPECT_EQ(event.GetMessage(), "hehe");
}

TEST(EventsTest, OtherClientOnlineEvent)
{
	OtherClientOnlineEvent event{};
	Data::EventData["OtherClientOnlineEvent"].get_to(event);
	EXPECT_EQ(event._TYPE_, "OtherClientOnlineEvent");

	EXPECT_EQ(event.GetClient().id, 1);
	EXPECT_EQ(event.GetClient().platform, "WINDOWS");
	EXPECT_EQ(event.GetKind(), 69899);
}

TEST(EventsTest, OtherClientOfflineEvent)
{
	OtherClientOfflineEvent event{};
	Data::EventData["OtherClientOfflineEvent"].get_to(event);
	EXPECT_EQ(event._TYPE_, "OtherClientOfflineEvent");

	EXPECT_EQ(event.GetClient().id, 1);
	EXPECT_EQ(event.GetClient().platform, "WINDOWS");
}

TEST(EventsTest, CommandExecutedEvent)
{
	CommandExecutedEvent event{};
	Data::EventData["CommandExecutedEvent"].get_to(event);
	EXPECT_EQ(event._TYPE_, "CommandExecutedEvent");

	EXPECT_EQ(event.GetName(), "shutdown");
	EXPECT_TRUE(event.GetFriendSender());
	EXPECT_EQ(event.GetFriendSender()->id, 123123_qq);
	EXPECT_EQ(event.GetFriendSender()->nickname, "nick");
	EXPECT_EQ(event.GetFriendSender()->remark, "remark");
	EXPECT_FALSE(event.GetMemberSender());
	EXPECT_EQ(event.GetCommandArgs().size(), 1);
	EXPECT_EQ(event.GetCommandArgs()[0].GetType(), MessageTypes::PLAIN);
	EXPECT_EQ(event.GetCommandArgs().GetAt<PlainMessage>(0).GetText(), "myself");
}

TEST(EventsTest, FriendMessageEvent)
{
	FriendMessageEvent event{};
	Data::EventData["FriendMessage"].get_to(event);
	EXPECT_EQ(event._TYPE_, "FriendMessage");

	EXPECT_EQ(event.GetSender().id, 123_qq);
	EXPECT_EQ(event.GetSender().nickname, "xxx");
	EXPECT_EQ(event.GetSender().remark, "who");
	EXPECT_EQ(event.GetMessage().size(), 1);
	EXPECT_EQ(event.GetMessage()[0].GetType(), MessageTypes::PLAIN);
	EXPECT_EQ(event.GetMessage().GetAt<PlainMessage>(0).GetText(), "hello");
}

TEST(EventsTest, GroupMessageEvent)
{
	GroupMessageEvent event{};
	Data::EventData["GroupMessage"].get_to(event);
	EXPECT_EQ(event._TYPE_, "GroupMessage");

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
	EXPECT_EQ(event.GetMessage()[0].GetType(), MessageTypes::PLAIN);
	EXPECT_EQ(event.GetMessage().GetAt<PlainMessage>(0).GetText(), "hello");
}

TEST(EventsTest, TempMessageEvent)
{
	TempMessageEvent event{};
	Data::EventData["TempMessage"].get_to(event);
	EXPECT_EQ(event._TYPE_, "TempMessage");

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
	EXPECT_EQ(event.GetMessage()[0].GetType(), MessageTypes::PLAIN);
	EXPECT_EQ(event.GetMessage().GetAt<PlainMessage>(0).GetText(), "hello");
}

TEST(EventsTest, StrangerMessageEvent)
{
	StrangerMessageEvent event{};
	Data::EventData["StrangerMessage"].get_to(event);
	EXPECT_EQ(event._TYPE_, "StrangerMessage");

	EXPECT_EQ(event.GetSender().id, 123_qq);
	EXPECT_EQ(event.GetSender().nickname, "xxx");
	EXPECT_EQ(event.GetSender().remark, "who");
	EXPECT_EQ(event.GetMessage().size(), 1);
	EXPECT_EQ(event.GetMessage()[0].GetType(), MessageTypes::PLAIN);
	EXPECT_EQ(event.GetMessage().GetAt<PlainMessage>(0).GetText(), "hello");
}

TEST(EventsTest, OtherClientMessageEvent)
{
	OtherClientMessageEvent event{};
	Data::EventData["OtherClientMessage"].get_to(event);
	EXPECT_EQ(event._TYPE_, "OtherClientMessage");

	EXPECT_EQ(event.GetSender().id, 123);
	EXPECT_EQ(event.GetSender().platform, "MOBILE");
	EXPECT_EQ(event.GetMessage().size(), 1);
	EXPECT_EQ(event.GetMessage()[0].GetType(), MessageTypes::PLAIN);
	EXPECT_EQ(event.GetMessage().GetAt<PlainMessage>(0).GetText(), "hello");
}

TEST(EventsTest, FriendSyncMessageEvent)
{
	FriendSyncMessageEvent event{};
	Data::EventData["FriendSyncMessage"].get_to(event);
	EXPECT_EQ(event._TYPE_, "FriendSyncMessage");

	EXPECT_EQ(event.GetFriend().id, 123_qq);
	EXPECT_EQ(event.GetFriend().nickname, "xxx");
	EXPECT_EQ(event.GetFriend().remark, "who");
	EXPECT_EQ(event.GetMessage().size(), 1);
	EXPECT_EQ(event.GetMessage()[0].GetType(), MessageTypes::PLAIN);
	EXPECT_EQ(event.GetMessage().GetAt<PlainMessage>(0).GetText(), "hello");
}

TEST(EventsTest, GroupSyncMessageEvent)
{
	GroupSyncMessageEvent event{};
	Data::EventData["GroupSyncMessage"].get_to(event);
	EXPECT_EQ(event._TYPE_, "GroupSyncMessage");

	EXPECT_EQ(event.GetGroup().id, 1234567890_gid);
	EXPECT_EQ(event.GetGroup().name, "bbb");
	EXPECT_EQ(event.GetGroup().permission, PERMISSION::MEMBER);
	EXPECT_EQ(event.GetMessage().size(), 1);
	EXPECT_EQ(event.GetMessage()[0].GetType(), MessageTypes::PLAIN);
	EXPECT_EQ(event.GetMessage().GetAt<PlainMessage>(0).GetText(), "hello");
}

TEST(EventsTest, TempSyncMessageEvent)
{
	TempSyncMessageEvent event{};
	Data::EventData["TempSyncMessage"].get_to(event);
	EXPECT_EQ(event._TYPE_, "TempSyncMessage");

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
	EXPECT_EQ(event.GetMessage()[0].GetType(), MessageTypes::PLAIN);
	EXPECT_EQ(event.GetMessage().GetAt<PlainMessage>(0).GetText(), "hello");
}

TEST(EventsTest, StrangerSyncMessageEvent)
{
	StrangerSyncMessageEvent event{};
	Data::EventData["StrangerSyncMessage"].get_to(event);
	EXPECT_EQ(event._TYPE_, "StrangerSyncMessage");

	EXPECT_EQ(event.GetStranger().id, 123_qq);
	EXPECT_EQ(event.GetStranger().nickname, "xxx");
	EXPECT_EQ(event.GetStranger().remark, "who");
	EXPECT_EQ(event.GetMessage().size(), 1);
	EXPECT_EQ(event.GetMessage()[0].GetType(), MessageTypes::PLAIN);
	EXPECT_EQ(event.GetMessage().GetAt<PlainMessage>(0).GetText(), "hello");
}
