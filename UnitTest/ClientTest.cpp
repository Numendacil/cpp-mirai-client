#include <chrono>
#include <condition_variable>
#include <mutex>
#include <stdexcept>
#include <memory>
#include <string>
#include <thread>
#include <type_traits>

#include <gtest/gtest.h>

#include <nlohmann/json.hpp>
#include <httplib.h>
#include <ixwebsocket/IXWebSocketServer.h>

#include <libmirai/Exceptions/Exceptions.hpp>
#include <libmirai/Types/Types.hpp>
#include <libmirai/Utils/Common.hpp>
#include <libmirai/mirai.hpp>

#include "JsonData.hpp"

using json = nlohmann::json;
using namespace Mirai;
using namespace std::chrono_literals;

class ClientTest : public testing::Test
{
protected:
	static std::unique_ptr<httplib::Server> HttpSvr;
	static std::unique_ptr<ix::WebSocketServer> WsSvr;
	static std::thread th;

	static void SetUpTestSuite()
	{
		if (!HttpSvr)
		{
			HttpSvr = std::make_unique<httplib::Server>();

			HttpSvr->Get(R"(/sessionInfo)", [](const httplib::Request& req, httplib::Response& res) {
				json param = req.params;
				if (!param.contains("sessionKey") || param.at("sessionKey").get<std::string>() != "aaabbbccc")
				{
					json resp = 
					{
						{"code", 3},
						{"msg", "Unknown session " + param.dump()}
					};
					res.set_content(resp.dump(), "application/json;charset=UTF-8");
				}
				else
				{
					json resp = 
					{
						{"code", 0},
						{"msg", ""},
						{"data", {
							{"sessionKey", "aaabbbccc"},
							{"qq", User{123456_qq, "xxx", "xx"}}
						}}
					};
					res.set_content(resp.dump(), "application/json;charset=UTF-8");
				}
			});

			th = std::thread([]{HttpSvr->listen("localhost", 1234);});
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		if (!WsSvr)
		{
			WsSvr = std::make_unique<ix::WebSocketServer>(4321);
			WsSvr->disablePerMessageDeflate();

			WsSvr->setOnClientMessageCallback([](
				std::shared_ptr<ix::ConnectionState>,
				ix::WebSocket& websocket, 
				const ix::WebSocketMessagePtr& msg
			)
			{
				if (msg->type == ix::WebSocketMessageType::Open)
				{
					if (msg->openInfo.uri != "/all?verifyKey=xxx&qq=111" && msg->openInfo.uri != "/all?qq=111&verifyKey=xxx")
					{
						json resp = {
							{"syncId", ""},
							{"data", {{"code", -1}, {"msg", "Invalid uri: " + msg->openInfo.uri}}}
						}; 
						websocket.send(resp.dump());
					}
					else
					{
						json resp = {
							{"syncId", ""},
							{"data", {{"code", 0}, {"session", "aaabbbccc"}}}
						}; 
						websocket.send(resp.dump());

						std::random_device dev;
						std::mt19937 rng(dev());
						json data;
						for (const auto& p : Data::EventData.items())
						{
							data += p.value();
						}
						std::uniform_int_distribution<unsigned long> randidx(0, data.size() - 1);
						for (std::size_t i = 0; i < 10000; i++)
						{
							auto idx = randidx(rng);
							json event = {
								{"syncId", ""},
								{"data", data[idx]}
							};
							websocket.send(event.dump());
						}
						std::this_thread::sleep_for(100ms);
						websocket.close();
					}
				}
			}
			);
			
			if (!WsSvr->listenAndStart())
				throw std::runtime_error("Can't start server");
		}
	}

	static void TearDownTestSuite()
	{
		if (HttpSvr)
		{
			if (HttpSvr->is_running())
				HttpSvr->stop();
			if (th.joinable())
				th.join();
			HttpSvr = nullptr;
		}

		if (WsSvr)
		{
			WsSvr->stop();
			WsSvr = nullptr;
		}
	}

};

std::unique_ptr<httplib::Server> ClientTest::HttpSvr;
std::unique_ptr<ix::WebSocketServer> ClientTest::WsSvr;
std::thread ClientTest::th;

TEST_F(ClientTest, ApplicationTest)
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
		std::unique_lock<std::mutex> lk(mtx);
		EXPECT_TRUE(cv.wait_for(lk, 30s, [&]{ return !start; }));
	}
	EXPECT_FALSE(client.isConnected());
	EXPECT_EQ(count, 10000);
}