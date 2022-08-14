
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <stdexcept>
#include <memory>
#include <string>
#include <thread>
#include <type_traits>

#include <gtest/gtest.h>

#include <ixwebsocket/IXWebSocketServer.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXWebSocketMessage.h>
#include <nlohmann/json.hpp>

#include <libmirai/Exceptions/Exceptions.hpp>
#include <libmirai/Types/Types.hpp>
#include <libmirai/Utils/Common.hpp>
#include <libmirai/Impl/MessageClientImpl.hpp>

using json = nlohmann::json;
using namespace Mirai;
using namespace std::chrono_literals;

class MessageClientTest : public testing::Test
{
protected:
	static std::unique_ptr<ix::WebSocketServer> svr;
	static void SetUpTestSuite()
	{
		if (!svr)
		{
			svr = std::make_unique<ix::WebSocketServer>(4321);
			svr->disablePerMessageDeflate();

			svr->setOnClientMessageCallback([](
				std::shared_ptr<ix::ConnectionState>,
				ix::WebSocket& websocket, 
				const ix::WebSocketMessagePtr& msg
			)
			{
				if (msg->type == ix::WebSocketMessageType::Open)
				{
					json resp = {
						{"uri", msg->openInfo.uri},
						{"protocol", msg->openInfo.protocol}
					}; 
					websocket.send(resp.dump());
				}
				else if (msg->type == ix::WebSocketMessageType::Message)
				{
					json resp = {
						{"msg", msg->str}
					}; 
					websocket.send(resp.dump());
				}
			}
			);
			
			if (!svr->listenAndStart())
				throw std::runtime_error("Can't start server");
		}
	}

	static void TearDownTestSuite()
	{
		if (svr)
		{
			svr->stop();
			svr = nullptr;
		}
	}

};

std::unique_ptr<ix::WebSocketServer> MessageClientTest::svr;

TEST_F(MessageClientTest, HandlerTest)
{
	Details::MessageClientImpl client(60s, 60s, true, false, false);
	ix::WebSocketOpenInfo info;

	std::mutex mtx;
	std::condition_variable cv;
	bool start = false;
	bool error = false;

	client.OnOpen([&](const ix::WebSocketOpenInfo &i)
	{
		std::lock_guard<std::mutex> lk(mtx);
		info = i;
		start = true;
		error = false;
		cv.notify_one();
		EXPECT_EQ(i.uri, "/test?msg=hello");
	});
	client.OnText([&](const std::string& msg)
	{
		json data = json::parse(msg);
		EXPECT_EQ(info.uri, data["uri"].get<std::string>());
	});
	client.OnClose([&](const ix::WebSocketCloseInfo& info)
	{
		std::lock_guard<std::mutex> lk(mtx);
		start = false;
		error = false;
	});
	client.OnError([&](const ix::WebSocketErrorInfo& info)
	{
		std::lock_guard<std::mutex> lk(mtx);
		error = true;
		cv.notify_one();
	});

	client.Connect("ws://127.0.0.1:4321/test?msg=hello");

	{
		std::unique_lock<std::mutex> lk(mtx);
		ASSERT_TRUE(cv.wait_for(lk, 5s, [&]{ return start || error; }));
		EXPECT_TRUE(start);
		EXPECT_FALSE(error);
	}

	EXPECT_TRUE(client.isConnected());

	client.Disconnect();
	EXPECT_FALSE(start);
	EXPECT_FALSE(error);

	client.Connect("ws://127.0.0.1:1000");

	{
		std::unique_lock<std::mutex> lk(mtx);
		ASSERT_TRUE(cv.wait_for(lk, 5s, [&]{ return start || error; }));
		EXPECT_FALSE(start);
		EXPECT_TRUE(error);
	}

	EXPECT_FALSE(client.isConnected());
	client.Disconnect();
}