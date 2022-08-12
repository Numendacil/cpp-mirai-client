#include <chrono>
#include <gtest/gtest.h>

#include <mutex>
#include <nlohmann/json.hpp>
#include <libmirai/Utils/Common.hpp>
#include <libmirai/Utils/Factory.hpp>
#include <libmirai/Utils/ThreadPool.hpp>
#include <thread>

using json = nlohmann::json;
TEST(UtilsTest, ThreadPool)
{
	using namespace Mirai::Utils;
	ThreadPool pool(24);
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
	std::future<int> result[300];
	for (int i = 0; i < 300; i++)
		result[i] = pool.enqueue(&Counter::GetFinal, &counter);
	int max = 0;
	for (int i = 0; i < 300; i++)
	{
		int res = result[i].get();
		if (max < res)
			max = res;
	}
	EXPECT_EQ(max, 300);
}

TEST(UtilsTest, Common)
{
	using namespace Mirai::Utils;
	EXPECT_EQ(GetValue(json{{"null", nullptr}}, "null", "abc"), "abc");
	EXPECT_EQ(GetValue(json{{"null", nullptr}}, "waht", "abc"), "abc");
	EXPECT_EQ(GetValue(json{1, 2, 3, 4}, "waht", "abc"), "abc");
	EXPECT_THROW(GetValue(json{{"waht", 1}}, "waht", "abc"), json::type_error);
	EXPECT_EQ(GetOptional<std::string>(json{{"a", nullptr}}, "a"), std::nullopt);
	EXPECT_EQ(GetOptional<std::string>(json{{"a", nullptr}}, std::string_view("b")), std::nullopt);
	EXPECT_EQ(GetOptional<std::string>(json{{"a", "b"}}, std::string("a")), "b");
} 