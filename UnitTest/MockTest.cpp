#include <chrono>
#include <condition_variable>
#include <mutex>
#include <random>
#include <stdexcept>
#include <memory>
#include <string>
#include <thread>
#include <type_traits>
#include <vector>

#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include <libmirai/Exceptions/Exceptions.hpp>
#include <libmirai/Types/Types.hpp>
#include <libmirai/Events/Events.hpp>
#include <libmirai/MockApi/MockClient.hpp>

#include "JsonData.hpp"

using json = nlohmann::json;
using namespace Mirai;
using namespace std::chrono_literals;

TEST(MockClientTest, FunctionTest)
{
	SessionConfigs opts;
	opts.HttpUrl = "http://localhost:1234";
	opts.WebsocketUrl = "ws://localhost:4321";
	opts.AutoReconnect = false;
	opts.VerifyKey = "xxx";
	opts.BotQQ = 111_qq;

	std::mutex mtx;
	std::condition_variable cv;
	bool start = false;

	std::atomic<int> count = 0;

	MiraiClient client(opts);

	client.On<ClientParseErrorEvent>([](ClientParseErrorEvent event){
		throw event.error;
	});

	client.On<ClientConnectionEstablishedEvent>([&](ClientConnectionEstablishedEvent event){
		std::lock_guard<std::mutex> lk(mtx);
		start = true;
		cv.notify_one();
	});

	client.On<ClientConnectionClosedEvent>([&](ClientConnectionClosedEvent event){
		std::lock_guard<std::mutex> lk(mtx);
		start = false;
		cv.notify_one();
	});


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

	client.Connect();
	EXPECT_TRUE(client.isConnected());
	
	{
		std::random_device dev;
		std::mt19937 rng(dev());
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
			client.Received(event.dump());
		}
		client.Disconnect();
	}

	EXPECT_FALSE(client.isConnected());
	EXPECT_EQ(count, 1000);
}

void foo(MiraiClient& client)
{
	auto list = client.GetMemberList(1234_gid);
	for (const auto& member : list)
		client.SendGroupMessage(1234_gid, MessageChain().At(member.id));
}

TEST(MockClientTest, MockTest)
{
	class MockClient : public MiraiClient
	{
	public:
		std::vector<GroupMember> list = { GroupMember(1_qq), GroupMember(2_qq), GroupMember(3_qq) };
		int64_t count = 0;

		std::vector<GroupMember> GetMemberList(GID_t GroupId) override
		{
			return this->list;
		}

		MessageId_t SendGroupMessage(GID_t GroupId, const MessageChain& message,
	                                     std::optional<MessageId_t> QuoteId = std::nullopt, bool ignoreInvalid = false) override
		{
			auto At = message.GetAll<AtMessage>();
			for (const auto& m : At)
				count += (int64_t)m.GetTarget();
			return 0;
		}
	};

	MockClient client;
	foo(client);
	EXPECT_EQ(client.count, [&]() -> int64_t { 
		int64_t result{};
		for (const auto& m : client.list)
			result += (int64_t)m.id;
		return result;
	}() );
}