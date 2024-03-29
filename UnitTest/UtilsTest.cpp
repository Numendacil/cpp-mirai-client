#include <chrono>
#include <gtest/gtest.h>

#define MIRAI_LOGGING_LEVEL MIRAI_LOGGING_LEVELS_INFO

#include <memory>
#include <mutex>
#include <nlohmann/json.hpp>
#include <libmirai/Utils/Common.hpp>
#include <libmirai/Utils/ThreadPool.hpp>
#include <libmirai/Utils/Logger.hpp>
#include <thread>

using json = nlohmann::json;

// NOLINTBEGIN(*)

TEST(UtilsTest, ThreadPool)
{
	using namespace Mirai::Utils;
	struct Counter
	{
		int i = 0;
		std::mutex _mtx;
		int GetFinal()
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			std::lock_guard<std::mutex> lk(this->_mtx);
			i++;
			return i;
		}
	} counter;

	{
		std::future<int> result[300];
		ThreadPool pool(24);
		int max = 0;

		pool.pause();
		for (auto & i : result)
			i = pool.enqueue(&Counter::GetFinal, &counter);
		pool.resume();

		for (auto & i : result)
		{
			int res = i.get();
			if (max < res)
				max = res;
		}
		EXPECT_EQ(max, 300);

		pool.pause();
		for (size_t l = 0; l < 10; l++)
			(void)pool.enqueue(&Counter::GetFinal, &counter);

		// pool dtor
	}
	EXPECT_EQ(counter.i, 300);

	{
		ThreadPool pool(24);
		pool.pause();
		for (size_t i = 0; i < 100; i++)
			pool.enqueue(&Counter::GetFinal, &counter);
		pool.stop(true);
		// pool dtor
	}
	EXPECT_EQ(counter.i, 400);
}

TEST(UtilsTest, Common)
{
	using namespace Mirai::Utils;
	EXPECT_EQ(GetValue(json{{"null", nullptr}}, "null", "abc"), "abc");
	EXPECT_EQ(GetValue(json{{"null", nullptr}}, "waht", "abc"), "abc");
	// EXPECT_EQ(GetValue(json{1, 2, 3, 4}, "waht", "abc"), "abc");
	EXPECT_THROW(GetValue(json{{"waht", 1}}, "waht", "abc"), json::type_error);
	EXPECT_EQ(GetOptional<std::string>(json{{"a", nullptr}}, "a"), std::nullopt);
	EXPECT_EQ(GetOptional<std::string>(json{{"a", nullptr}}, std::string_view("b")), std::nullopt);
	EXPECT_EQ(GetOptional<std::string>(json{{"a", "b"}}, std::string("a")), "b");
} 


TEST(UtilsTest, Logger)
{
	struct TestLogger : public Mirai::ILogger
	{
		TestLogger() : ILogger(Mirai::LoggingLevels::ERROR) {}
		void log(const std::string& /*msg*/, Mirai::LoggingLevels /*level*/) override {}
	};
	std::shared_ptr<Mirai::ILogger> logger = std::make_shared<TestLogger>();

	#define xstr(a) std::string(str(a))
	#define str(a) #a

	int i = 0;
	LOG_ERROR(*logger, [&](){i++; return ""; }());
	EXPECT_EQ(i, 1);
	LOG_INFO(*logger, [&](){i++; return ""; }());
	EXPECT_EQ(i, 1);
	EXPECT_EQ(xstr(LOG_TRACE(*logger, [&](){i++; return ""; }())), xstr((void)0));
	EXPECT_EQ(i, 1);
}

// NOLINTEND(*)