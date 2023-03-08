// Copyright (C) 2022 Numendacil and contributors
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef MIRAI_UTILS_THREADPOOL_HPP_
#define MIRAI_UTILS_THREADPOOL_HPP_

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>
#include <vector>

namespace Mirai::Utils
{


class ThreadPool
{
protected:
	mutable std::mutex mtx_;
	std::condition_variable cv_;

	std::vector<std::thread> workers_;
	std::queue<std::function<void()>> jobs_;

	bool stop_ = false;

	void loop_()
	{
		while (true)
		{
			std::function<void()> job;
			{
				std::unique_lock<std::mutex> lk(this->mtx_);
				this->cv_.wait(lk, [this] { return this->stop_ || !this->jobs_.empty(); });
				if (this->stop_ && this->jobs_.empty()) return;
				job = std::move(this->jobs_.front());
				this->jobs_.pop();
			}

			job();
		}
	}

public:
	ThreadPool(std::size_t n)
	{
		workers_.reserve(n);
		for (std::size_t i = 0; i < n; i++)
		{
			workers_.emplace_back([this]() { this->loop_(); });
		}
	}
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;
	ThreadPool(ThreadPool&&) = delete;
	ThreadPool& operator=(ThreadPool&&) = delete;

	~ThreadPool()
	{
		{
			std::unique_lock<std::mutex> lk(this->mtx_);
			this->stop_ = true;
		}
		this->cv_.notify_all();
		for (std::thread& th : this->workers_)
		{
			if (th.joinable()) th.join();
		}
	}

	template<class F, class... Args>
	auto enqueue(F&& f, Args&&... args) -> std::future<std::invoke_result_t<F&&, Args&&...>>
	{
		using return_type = std::invoke_result_t<F&&, Args&&...>;

		auto task = std::make_shared<std::packaged_task<return_type()>>(
			std::bind(std::forward<F>(f), std::forward<Args>(args)...));

		std::future<return_type> result = task->get_future();
		{
			std::unique_lock<std::mutex> lk(this->mtx_);
			if (this->stop_) return std::future<return_type>();
			this->jobs_.emplace([task]() { (*task)(); });
		}
		this->cv_.notify_one();
		return result;
	}
};

} // namespace Mirai::Utils


#endif