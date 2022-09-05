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

#ifndef _MIRAI__UTILS_THREADPOOL_HPP_
#define _MIRAI__UTILS_THREADPOOL_HPP_

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
	mutable std::mutex _mtx;
	std::condition_variable _cv;

	std::vector<std::thread> _workers;
	std::queue<std::function<void()>> _jobs;

	bool _stop = false;

	void _loop()
	{
		while (true)
		{
			std::function<void()> job;
			{
				std::unique_lock<std::mutex> lk(this->_mtx);
				this->_cv.wait(lk, [this] { return this->_stop || !this->_jobs.empty(); });
				if (this->_stop && this->_jobs.empty()) return;
				job = std::move(this->_jobs.front());
				this->_jobs.pop();
			}

			job();
		}
	}

public:
	ThreadPool(std::size_t n)
	{
		_workers.reserve(n);
		for (std::size_t i = 0; i < n; i++)
		{
			_workers.emplace_back([this]() { this->_loop(); });
		}
	}
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;
	ThreadPool(ThreadPool&&) = delete;
	ThreadPool& operator=(ThreadPool&&) = delete;

	~ThreadPool()
	{
		{
			std::unique_lock<std::mutex> lk(this->_mtx);
			this->_stop = true;
		}
		this->_cv.notify_all();
		for (std::thread& th : this->_workers)
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
			std::unique_lock<std::mutex> lk(this->_mtx);
			if (this->_stop) return std::future<return_type>();
			this->_jobs.emplace([task]() { (*task)(); });
		}
		this->_cv.notify_one();
		return result;
	}
};

} // namespace Mirai::Utils


#endif