
#include <chrono>
#include <stdexcept>
#include <memory>
#include <string>
#include <thread>
#include <type_traits>

#include <gtest/gtest.h>

#include <nlohmann/json.hpp>
#include <httplib.h>

#include <libmirai/Exceptions/Exceptions.hpp>
#include <libmirai/Types/Types.hpp>
#include <libmirai/Utils/Common.hpp>
#include <libmirai/Impl/HttpClientImpl.hpp>

using json = nlohmann::json;
using namespace Mirai;
using namespace std::chrono_literals;

class HttpClientTest : public testing::Test
{
protected:
	static std::unique_ptr<httplib::Server> svr;
	static std::thread th;

	static void SetUpTestSuite()
	{
		if (!svr)
		{
			svr = std::make_unique<httplib::Server>();

			svr->Get(R"((.+))", [](const httplib::Request& req, httplib::Response& res) {
				json query;
				for (const auto& q : req.params)
					query.emplace(q);
				std::string path = req.matches[1];
				res.set_content(json{{"code", 0}, {"msg", ""}, {"data", {{"path", path}, {"query", query}, {"method", "get"}}}}.dump(), "application/json;charset=UTF-8");
			});

			svr->Post(R"((.+))", [](const httplib::Request& req, httplib::Response& res) {
				json content;
				std::string path = req.matches[1];
				try
				{
					content = json::parse(req.body);
				}
				catch(json::parse_error&)
				{
					res.set_content(json{{"code", 0}, {"msg", ""}, {"data", {{"path", path}, {"content", nullptr}, {"method", "post"}}}}.dump(), "application/json;charset=UTF-8");
					return;
				}
				res.set_content(json{{"code", 0}, {"msg", ""}, {"data", {{"path", path}, {"content", content}, {"method", "post"}}}}.dump(), "application/json;charset=UTF-8");
			});

			th = std::thread([]{svr->listen("localhost", 1234);});
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}

	static void TearDownTestSuite()
	{
		if (svr)
		{
			if (svr->is_running())
				svr->stop();
			if (th.joinable())
				th.join();
			svr = nullptr;
		}
	}

};

std::unique_ptr<httplib::Server> HttpClientTest::svr = nullptr;
std::thread HttpClientTest::th;

TEST_F(HttpClientTest, RouteTest)
{
	Mirai::Details::HttpClientImpl cli("http://localhost:1234");
	json resp;
	
	resp = cli.SessionInfo("");
	EXPECT_EQ(Utils::GetValue(resp, "method", ""), "get");
	EXPECT_EQ(Utils::GetValue(resp, "path", ""), "/sessionInfo");
	
	resp = cli.About();
	EXPECT_EQ(Utils::GetValue(resp, "method", ""), "get");
	EXPECT_EQ(Utils::GetValue(resp, "path", ""), "/about");

	resp = cli.BotList();
	EXPECT_EQ(Utils::GetValue(resp, "method", ""), "get");
	EXPECT_EQ(Utils::GetValue(resp, "path", ""), "/botList");
	
	resp = cli.MessageFromId("", 0, 0_qq);
	EXPECT_EQ(Utils::GetValue(resp, "method", ""), "get");
	EXPECT_EQ(Utils::GetValue(resp, "path", ""), "/messageFromId");
	
	resp = cli.GroupList("");
	EXPECT_EQ(Utils::GetValue(resp, "method", ""), "get");
	EXPECT_EQ(Utils::GetValue(resp, "path", ""), "/groupList");
	
	resp = cli.MemberList("", 0_gid);
	EXPECT_EQ(Utils::GetValue(resp, "method", ""), "get");
	EXPECT_EQ(Utils::GetValue(resp, "path", ""), "/memberList");
	
	resp = cli.BotProfile("");
	EXPECT_EQ(Utils::GetValue(resp["data"], "method", ""), "get");
	EXPECT_EQ(Utils::GetValue(resp["data"], "path", ""), "/botProfile");
	
	resp = cli.FriendProfile("", 0_qq);
	EXPECT_EQ(Utils::GetValue(resp["data"], "method", ""), "get");
	EXPECT_EQ(Utils::GetValue(resp["data"], "path", ""), "/friendProfile");
	
	resp = cli.MemberProfile("", 0_gid, 0_qq);
	EXPECT_EQ(Utils::GetValue(resp["data"], "method", ""), "get");
	EXPECT_EQ(Utils::GetValue(resp["data"], "path", ""), "/memberProfile");
	
	resp = cli.UserProfile("", 0_qq);
	EXPECT_EQ(Utils::GetValue(resp["data"], "method", ""), "get");
	EXPECT_EQ(Utils::GetValue(resp["data"], "path", ""), "/userProfile");
	
	resp = cli.SendFriendMessage("", 0_qq, json{});
	EXPECT_EQ(Utils::GetValue(resp["data"], "method", ""), "post");
	EXPECT_EQ(Utils::GetValue(resp["data"], "path", ""), "/sendFriendMessage");
	
	resp = cli.SendGroupMessage("", 0_gid, json{});
	EXPECT_EQ(Utils::GetValue(resp["data"], "method", ""), "post");
	EXPECT_EQ(Utils::GetValue(resp["data"], "path", ""), "/sendGroupMessage");
	
	resp = cli.SendTempMessage("", 0_qq, 0_gid, json{});
	EXPECT_EQ(Utils::GetValue(resp["data"], "method", ""), "post");
	EXPECT_EQ(Utils::GetValue(resp["data"], "path", ""), "/sendTempMessage");
	
	resp = cli.Recall("", 0, 0_qq);
	EXPECT_EQ(Utils::GetValue(resp["data"], "method", ""), "post");
	EXPECT_EQ(Utils::GetValue(resp["data"], "path", ""), "/recall");
	
	// resp = cli.RoamingMessages("");
	// EXPECT_EQ(Utils::GetValue(resp, "method", ""), "post");
	// EXPECT_EQ(Utils::GetValue(resp, "path", ""), "/roamingMessages");
	
	resp = cli.FileList("", "", "", 0_gid);
	EXPECT_EQ(Utils::GetValue(resp, "method", ""), "get");
	EXPECT_EQ(Utils::GetValue(resp, "path", ""), "/file/list");
	
	resp = cli.FileInfo("", "", "", 0_gid);
	EXPECT_EQ(Utils::GetValue(resp, "method", ""), "get");
	EXPECT_EQ(Utils::GetValue(resp, "path", ""), "/file/info");
	
	resp = cli.FileMkdir("", "", "", 0_gid, "");
	EXPECT_EQ(Utils::GetValue(resp, "method", ""), "post");
	EXPECT_EQ(Utils::GetValue(resp, "path", ""), "/file/mkdir");
	
	resp = cli.FileDelete("", "", "", 0_gid);
	EXPECT_EQ(Utils::GetValue(resp["data"], "method", ""), "post");
	EXPECT_EQ(Utils::GetValue(resp["data"], "path", ""), "/file/delete");
	
	resp = cli.FileMove("", "", "", 0_gid, "", "");
	EXPECT_EQ(Utils::GetValue(resp["data"], "method", ""), "post");
	EXPECT_EQ(Utils::GetValue(resp["data"], "path", ""), "/file/move");
	
	resp = cli.FileRename("", "", "", 0_gid, "");
	EXPECT_EQ(Utils::GetValue(resp["data"], "method", ""), "post");
	EXPECT_EQ(Utils::GetValue(resp["data"], "path", ""), "/file/rename");

	resp = cli.FileUpload("", "", 0_gid, "", "", "");
	EXPECT_EQ(Utils::GetValue(resp, "method", ""), "post");
	EXPECT_EQ(Utils::GetValue(resp, "path", ""), "/file/upload");

	resp = cli.UploadImage("", "", "");
	EXPECT_EQ(Utils::GetValue(resp["data"], "method", ""), "post");
	EXPECT_EQ(Utils::GetValue(resp["data"], "path", ""), "/uploadImage");

	resp = cli.UploadAudio("", "", "");
	EXPECT_EQ(Utils::GetValue(resp["data"], "method", ""), "post");
	EXPECT_EQ(Utils::GetValue(resp["data"], "path", ""), "/uploadVoice");

	resp = cli.DeleteFriend("", 0_qq);
	EXPECT_EQ(Utils::GetValue(resp["data"], "method", ""), "post");
	EXPECT_EQ(Utils::GetValue(resp["data"], "path", ""), "/deleteFriend");

	resp = cli.Mute("", 0_gid, 0_qq, 0s);
	EXPECT_EQ(Utils::GetValue(resp["data"], "method", ""), "post");
	EXPECT_EQ(Utils::GetValue(resp["data"], "path", ""), "/mute");

	resp = cli.Unmute("", 0_gid, 0_qq);
	EXPECT_EQ(Utils::GetValue(resp["data"], "method", ""), "post");
	EXPECT_EQ(Utils::GetValue(resp["data"], "path", ""), "/unmute");

	resp = cli.Kick("", 0_gid, 0_qq, "");
	EXPECT_EQ(Utils::GetValue(resp["data"], "method", ""), "post");
	EXPECT_EQ(Utils::GetValue(resp["data"], "path", ""), "/kick");

	resp = cli.Quit("", 0_gid);
	EXPECT_EQ(Utils::GetValue(resp["data"], "method", ""), "post");
	EXPECT_EQ(Utils::GetValue(resp["data"], "path", ""), "/quit");

	resp = cli.MuteAll("", 0_gid);
	EXPECT_EQ(Utils::GetValue(resp["data"], "method", ""), "post");
	EXPECT_EQ(Utils::GetValue(resp["data"], "path", ""), "/muteAll");

	resp = cli.UnmuteAll("", 0_gid);
	EXPECT_EQ(Utils::GetValue(resp["data"], "method", ""), "post");
	EXPECT_EQ(Utils::GetValue(resp["data"], "path", ""), "/unmuteAll");

	resp = cli.SetEssence("", 0, 0_gid);
	EXPECT_EQ(Utils::GetValue(resp["data"], "method", ""), "post");
	EXPECT_EQ(Utils::GetValue(resp["data"], "path", ""), "/setEssence");

	resp = cli.GetGroupConfig("", 0_gid);
	EXPECT_EQ(Utils::GetValue(resp["data"], "method", ""), "get");
	EXPECT_EQ(Utils::GetValue(resp["data"], "path", ""), "/groupConfig");

	resp = cli.PostGroupConfig("", 0_gid, "");
	EXPECT_EQ(Utils::GetValue(resp["data"], "method", ""), "post");
	EXPECT_EQ(Utils::GetValue(resp["data"], "path", ""), "/groupConfig");

	resp = cli.GetMemberInfo("", 0_gid, 0_qq);
	EXPECT_EQ(Utils::GetValue(resp["data"], "method", ""), "get");
	EXPECT_EQ(Utils::GetValue(resp["data"], "path", ""), "/memberInfo");

	resp = cli.PostMemberInfo("", 0_gid, 0_qq, "", "");
	EXPECT_EQ(Utils::GetValue(resp["data"], "method", ""), "post");
	EXPECT_EQ(Utils::GetValue(resp["data"], "path", ""), "/memberInfo");

	resp = cli.MemberAdmin("", 0_gid, 0_qq, true);
	EXPECT_EQ(Utils::GetValue(resp["data"], "method", ""), "post");
	EXPECT_EQ(Utils::GetValue(resp["data"], "path", ""), "/memberAdmin");

	resp = cli.AnnoList("", 0_gid);
	EXPECT_EQ(Utils::GetValue(resp, "method", ""), "get");
	EXPECT_EQ(Utils::GetValue(resp, "path", ""), "/anno/list");

	resp = cli.AnnoPublish("", 0_gid, "", "", "", "");
	EXPECT_EQ(Utils::GetValue(resp, "method", ""), "post");
	EXPECT_EQ(Utils::GetValue(resp, "path", ""), "/anno/publish");

	resp = cli.AnnoDelete("", 0_gid, "");
	EXPECT_EQ(Utils::GetValue(resp["data"], "method", ""), "post");
	EXPECT_EQ(Utils::GetValue(resp["data"], "path", ""), "/anno/delete");

	resp = cli.RespNewFriendRequestEvent("", 0, 0_qq, 0_gid, 0, "");
	EXPECT_EQ(Utils::GetValue(resp["data"], "method", ""), "post");
	EXPECT_EQ(Utils::GetValue(resp["data"], "path", ""), "/resp/newFriendRequestEvent");

	resp = cli.RespMemberJoinRequestEvent("", 0, 0_qq, 0_gid, 0, "");
	EXPECT_EQ(Utils::GetValue(resp["data"], "method", ""), "post");
	EXPECT_EQ(Utils::GetValue(resp["data"], "path", ""), "/resp/memberJoinRequestEvent");

	resp = cli.RespBotInvitedJoinGroupRequestEvent("", 0, 0_qq, 0_gid, 0, "");
	EXPECT_EQ(Utils::GetValue(resp["data"], "method", ""), "post");
	EXPECT_EQ(Utils::GetValue(resp["data"], "path", ""), "/resp/botInvitedJoinGroupRequestEvent");

	resp = cli.CmdExecute("", json{});
	EXPECT_EQ(Utils::GetValue(resp["data"], "method", ""), "post");
	EXPECT_EQ(Utils::GetValue(resp["data"], "path", ""), "/cmd/execute");

	resp = cli.CmdRegister("", "", {}, "", "");
	EXPECT_EQ(Utils::GetValue(resp["data"], "method", ""), "post");
	EXPECT_EQ(Utils::GetValue(resp["data"], "path", ""), "/cmd/register");
	
}