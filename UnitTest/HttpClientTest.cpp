
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
#include <libmirai/Types/BasicTypes.hpp>
#include <libmirai/Messages/Messages.hpp>
#include <libmirai/Impl/HttpClientImpl.hpp>

using json = nlohmann::json;

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
				res.set_content(json{{"code", 0}, {"msg", ""}, {"data", {{"path", path}, {"query", query}}}}.dump(), "application/json;charset=UTF-8");
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
					res.set_content(json{{"code", 500}, {"msg", "Unexpected Format"}}.dump(), "application/json;charset=UTF-8");
					return;
				}
				res.set_content(json{{"code", 0}, {"msg", ""}, {"data", {{"path", path}, {"content", content}}}}.dump(), "application/json;charset=UTF-8");
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

TEST_F(HttpClientTest, MockServerTest)
{
	Mirai::Details::HttpClientImpl cli("http://localhost:1234");
	EXPECT_THROW(cli.FileUpload("aaa", "/path", Mirai::GID_t(123456), "group", "name", "content"), Mirai::MiraiApiHttpException);
}