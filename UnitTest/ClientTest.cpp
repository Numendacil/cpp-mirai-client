#include <atomic>
#include <memory>
#include <random>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <libmirai/Client/Client.hpp>
#include <libmirai/Adaptors/GMockAdaptor.hpp>

#include "JsonData.hpp"
#include "CustomAdaptor.hpp"
#include "libmirai/Exceptions/Exceptions.hpp"

using namespace Mirai;
using json = nlohmann::json;

// NOLINTBEGIN(*)

TEST(ClientTest, GMockAdaptorTest)
{
	auto adaptor = MakeGMockAdaptor([](GMockAdaptor& adaptor)
	{
		EXPECT_CALL(adaptor, MessageFromId)
		.Times(4)
		.WillOnce(testing::Return(FriendMessageEvent()))
		.WillOnce(testing::Return(GroupMessageEvent()))
		.WillOnce(testing::Return(TempMessageEvent()))
		.WillOnce(testing::Return(StrangerMessageEvent()));
	});

	auto* ptr = static_cast<GMockAdaptor*>(adaptor.get());

	MiraiClient client(1, std::move(adaptor));

	size_t count = 0;

	{
		client.On<BotOfflineEventActive>([&count](BotOfflineEventActive event) { count++; });
		client.On<BotOfflineEventDropped>([&count](BotOfflineEventDropped event) { count++; });
		client.On<BotOfflineEventForce>([&count](BotOfflineEventForce event) { count++; });
		client.On<BotOnlineEvent>([&count](BotOnlineEvent event) { count++; });
		client.On<BotReloginEvent>([&count](BotReloginEvent event) { count++; });
		client.On<BotGroupPermissionChangeEvent>([&count](BotGroupPermissionChangeEvent event) { count++; });
		client.On<BotInvitedJoinGroupRequestEvent>([&count](BotInvitedJoinGroupRequestEvent event) { count++; });
		client.On<BotJoinGroupEvent>([&count](BotJoinGroupEvent event) { count++; });
		client.On<BotLeaveEventActive>([&count](BotLeaveEventActive event) { count++; });
		client.On<BotLeaveEventDisband>([&count](BotLeaveEventDisband event) { count++; });
		client.On<BotLeaveEventKick>([&count](BotLeaveEventKick event) { count++; });
		client.On<BotMuteEvent>([&count](BotMuteEvent event) { count++; });
		client.On<BotUnmuteEvent>([&count](BotUnmuteEvent event) { count++; });
		client.On<CommandExecutedEvent>([&count](CommandExecutedEvent event) { count++; });
		client.On<FriendInputStatusChangedEvent>([&count](FriendInputStatusChangedEvent event) { count++; });
		client.On<FriendMessageEvent>([&count](FriendMessageEvent event) { count++; });
		client.On<FriendNickChangedEvent>([&count](FriendNickChangedEvent event) { count++; });
		client.On<FriendRecallEvent>([&count](FriendRecallEvent event) { count++; });
		client.On<FriendSyncMessageEvent>([&count](FriendSyncMessageEvent event) { count++; });
		client.On<GroupAllowAnonymousChatEvent>([&count](GroupAllowAnonymousChatEvent event) { count++; });
		client.On<GroupAllowConfessTalkEvent>([&count](GroupAllowConfessTalkEvent event) { count++; });
		client.On<GroupAllowMemberInviteEvent>([&count](GroupAllowMemberInviteEvent event) { count++; });
		client.On<GroupEntranceAnnouncementChangeEvent>([&count](GroupEntranceAnnouncementChangeEvent event)
		                                                { count++; });
		client.On<GroupMessageEvent>([&count](GroupMessageEvent event) { count++; });
		client.On<GroupMuteAllEvent>([&count](GroupMuteAllEvent event) { count++; });
		client.On<GroupNameChangeEvent>([&count](GroupNameChangeEvent event) { count++; });
		client.On<GroupRecallEvent>([&count](GroupRecallEvent event) { count++; });
		client.On<GroupSyncMessageEvent>([&count](GroupSyncMessageEvent event) { count++; });
		client.On<MemberCardChangeEvent>([&count](MemberCardChangeEvent event) { count++; });
		client.On<MemberHonorChangeEvent>([&count](MemberHonorChangeEvent event) { count++; });
		client.On<MemberJoinEvent>([&count](MemberJoinEvent event) { count++; });
		client.On<MemberJoinRequestEvent>([&count](MemberJoinRequestEvent event) { count++; });
		client.On<MemberLeaveEventKick>([&count](MemberLeaveEventKick event) { count++; });
		client.On<MemberLeaveEventQuit>([&count](MemberLeaveEventQuit event) { count++; });
		client.On<MemberMuteEvent>([&count](MemberMuteEvent event) { count++; });
		client.On<MemberPermissionChangeEvent>([&count](MemberPermissionChangeEvent event) { count++; });
		client.On<MemberSpecialTitleChangeEvent>([&count](MemberSpecialTitleChangeEvent event) { count++; });
		client.On<MemberUnmuteEvent>([&count](MemberUnmuteEvent event) { count++; });
		client.On<NewFriendRequestEvent>([&count](NewFriendRequestEvent event) { count++; });
		client.On<NudgeEvent>([&count](NudgeEvent event) { count++; });
		client.On<OtherClientMessageEvent>([&count](OtherClientMessageEvent event) { count++; });
		client.On<OtherClientOfflineEvent>([&count](OtherClientOfflineEvent event) { count++; });
		client.On<OtherClientOnlineEvent>([&count](OtherClientOnlineEvent event) { count++; });
		client.On<StrangerMessageEvent>([&count](StrangerMessageEvent event) { count++; });
		client.On<StrangerSyncMessageEvent>([&count](StrangerSyncMessageEvent event) { count++; });
		client.On<TempMessageEvent>([&count](TempMessageEvent event) { count++; });
		client.On<TempSyncMessageEvent>([&count](TempSyncMessageEvent event) { count++; });
	}

	EXPECT_NO_THROW(client.Connect());

	EXPECT_NO_THROW(client.GetFriendMessage(12345, 111_qq));
	EXPECT_NO_THROW(client.GetGroupMessage(12345, 111_gid));
	EXPECT_NO_THROW(client.GetTempMessage(12345, 111_gid));
	EXPECT_THROW(client.GetTempMessage(12345, 111_gid), TypeDismatchError);

	std::mt19937 rng(1234);
	json data;
	for (const auto& p : Data::EventData.items())
	{
		data += p.value();
	}
	std::uniform_int_distribution<unsigned long> randidx(0, data.size() - 1);
	for (std::size_t i = 0; i < 1000; i++)
	{
		auto idx = randidx(rng);
		json event = {
			{"syncId", ""},
			{"data", data[idx]}
		};
		
		EXPECT_NO_THROW(ptr->TextCallback(event.dump()));
	}

	EXPECT_NO_THROW(client.Disconnect());

	EXPECT_EQ(count, 1000);

}

TEST(ClientTest, CustomAdaptorTest)
{
	MiraiClient client(std::make_unique<CustomAdaptor>());

	EXPECT_NO_THROW((void)client.GetCompatibleVersion());
	EXPECT_NO_THROW((void)client.GetMiraiApiHttpVersion());
	EXPECT_NO_THROW((void)client.GetBotQQ());
	EXPECT_NO_THROW((void)client.GetBotList());

	EXPECT_NO_THROW((void)client.GetFriendMessage({}, {}));
	EXPECT_THROW((void)client.GetGroupMessage({}, {}), TypeDismatchError);
	EXPECT_THROW((void)client.GetTempMessage({}, {}), TypeDismatchError);
	EXPECT_THROW((void)client.GetStrangerMessage({}, {}), TypeDismatchError);

	EXPECT_NO_THROW((void)client.GetFriendList());
	EXPECT_NO_THROW((void)client.GetGroupList());
	EXPECT_NO_THROW((void)client.GetMemberList({}));
	EXPECT_NO_THROW((void)client.GetLatestMemberList({}));

	EXPECT_NO_THROW((void)client.GetBotProfile());
	EXPECT_NO_THROW((void)client.GetFriendProfile({}));
	EXPECT_NO_THROW((void)client.GetMemberProfile({}, {}));
	EXPECT_NO_THROW((void)client.GetUserProfile({}));

	MessageChain m;
	EXPECT_NO_THROW((void)client.SendFriendMessage({}, m));
	EXPECT_NO_THROW((void)client.SendFriendMessage({}, {}));
	EXPECT_NO_THROW((void)client.SendGroupMessage({}, m));
	EXPECT_NO_THROW((void)client.SendGroupMessage({}, {}));
	EXPECT_NO_THROW((void)client.SendTempMessage({}, {}, m));
	EXPECT_NO_THROW((void)client.SendTempMessage({}, {}, {}));

	EXPECT_NO_THROW((void)client.SendNudge({}));
	EXPECT_NO_THROW((void)client.NudgeFriend({}));
	EXPECT_NO_THROW((void)client.NudgeGroup({}, {}));
	EXPECT_NO_THROW((void)client.NudgeStranger({}));

	EXPECT_NO_THROW((void)client.RecallFriendMessage({}, {}));
	EXPECT_NO_THROW((void)client.RecallGroupMessage({}, {}));
	EXPECT_NO_THROW((void)client.GetRoamingFriendMessage({}, {}));

	EXPECT_NO_THROW((void)client.GetGroupFileList({}));
	EXPECT_NO_THROW((void)client.GetGroupFileInfo({}, {}));
	GroupFileInfo file;
	EXPECT_NO_THROW((void)client.GetGroupFileInfo({}, file));
	EXPECT_NO_THROW((void)client.CreateGroupFileDirectory({}, {}));
	EXPECT_NO_THROW((void)client.RemoveGroupFile({}, {}));
	EXPECT_NO_THROW((void)client.MoveGroupFile({}, {}, {}));
	EXPECT_NO_THROW((void)client.RenameGroupFile({}, {}, {}));

	EXPECT_NO_THROW((void)client.UploadGroupFile({}, {}, {}, ""));
	std::istringstream ifile("ss");
	EXPECT_NO_THROW((void)client.UploadGroupFile({}, {}, {}, ifile));
	auto ContentProvider = [](size_t offset, std::ostream& sink, bool& finish) { return false; };
	EXPECT_NO_THROW((void)client.UploadGroupFile({}, {}, {}, ContentProvider));
	EXPECT_NO_THROW((void)client.UploadFriendImage(""));
	EXPECT_NO_THROW((void)client.UploadFriendImage(ifile));
	EXPECT_NO_THROW((void)client.UploadFriendImage(ContentProvider));
	EXPECT_NO_THROW((void)client.UploadGroupImage(""));
	EXPECT_NO_THROW((void)client.UploadGroupImage(ifile));
	EXPECT_NO_THROW((void)client.UploadGroupImage(ContentProvider));
	EXPECT_NO_THROW((void)client.UploadTempImage(""));
	EXPECT_NO_THROW((void)client.UploadTempImage(ifile));
	EXPECT_NO_THROW((void)client.UploadTempImage(ContentProvider));
	EXPECT_NO_THROW((void)client.UploadGroupAudio(""));
	EXPECT_NO_THROW((void)client.UploadGroupAudio(ifile));
	EXPECT_NO_THROW((void)client.UploadGroupAudio(ContentProvider));
	
	EXPECT_NO_THROW((void)client.DeleteFriend({}));
	EXPECT_NO_THROW((void)client.Mute({}, {}, {}));
	EXPECT_NO_THROW((void)client.Mute({}, {}));
	EXPECT_NO_THROW((void)client.Unmute({}, {}));
	EXPECT_NO_THROW((void)client.Unmute({}));
	EXPECT_NO_THROW((void)client.Kick({}, {}, {}, false));
	EXPECT_NO_THROW((void)client.Kick({}, {}));
	EXPECT_NO_THROW((void)client.LeaveGroup({}));
	EXPECT_NO_THROW((void)client.MuteAll({}));
	EXPECT_NO_THROW((void)client.UnmuteAll({}));
	EXPECT_NO_THROW((void)client.SetEssence({}, {}));
	EXPECT_NO_THROW((void)client.GetGroupConfig({}));
	EXPECT_NO_THROW((void)client.SetGroupConfig({}));
	EXPECT_NO_THROW((void)client.GetMemberInfo({}, {}));
	EXPECT_NO_THROW((void)client.SetMemberInfo({}, {}));
	EXPECT_NO_THROW((void)client.SetGroupAdmin({}, {}));
	EXPECT_NO_THROW((void)client.GetAnnouncementList({}));
	EXPECT_NO_THROW((void)client.PublishAnnouncement({}, {}));
	EXPECT_NO_THROW((void)client.DeleteAnnouncement({}, {}));
	EXPECT_NO_THROW((void)client.DeleteAnnouncement({}));

	EXPECT_NO_THROW((void)client.RespNewFriendRequestEvent({}, {}, {}, {}, {}));
	EXPECT_NO_THROW((void)client.RespNewFriendRequestEvent({}, {}, {}));
	EXPECT_NO_THROW((void)client.RespMemberJoinRequestEvent({}, {}, {}, {}, {}));
	EXPECT_NO_THROW((void)client.RespMemberJoinRequestEvent({}, {}, {}));
	EXPECT_NO_THROW((void)client.RespBotInvitedJoinGroupRequestEvent({}, {}, {}, {}, {}));
	EXPECT_NO_THROW((void)client.RespBotInvitedJoinGroupRequestEvent({}, {}, {}));

	EXPECT_NO_THROW((void)client.RegisterCommand({}, {}, {}, {}));
	EXPECT_NO_THROW((void)client.ExecuteCommand(m));
	EXPECT_NO_THROW((void)client.ExecuteCommand(std::move(m)));
	
	EXPECT_THROW((void)client.CallAPI({}, {}, {}), NotImplementedError);
}

// NOLINTEND(*)