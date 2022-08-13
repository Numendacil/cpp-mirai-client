#ifndef _MIRAI__UTILS_THREADPOOL_HPP_
#define _MIRAI__UTILS_THREADPOOL_HPP_

#include <mutex>
#include <condition_variable>
#include <type_traits>
#include <vector>
#include <queue>
#include <thread>
#include <future>
#include <functional>

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
				this->_cv.wait(lk, [this]{ return this->_stop || !this->_jobs.empty(); });
				if(this->_stop && this->_jobs.empty())
					return;
				job = std::move(this->_jobs.front());
				this->_jobs.pop();
			}

			job();

		}
	}

public:
	ThreadPool(std::size_t n) : _stop(false)
	{
		_workers.reserve(n);
		for (std::size_t i = 0; i < n; i++)
		{
			_workers.emplace_back([this](){ this->_loop(); });
		}
	}

	~ThreadPool()
	{
		{
			std::unique_lock<std::mutex> lk(this->_mtx);
			this->_stop = true;
		}
		this->_cv.notify_all();
		for (std::thread& th : this->_workers)
		{
			if (th.joinable())
				th.join();
		}
	}

	template<class F, class... Args>
	auto enqueue(F&& f, Args&&... args)
	-> std::future<std::invoke_result_t<F&&, Args&&...>>
	{
		using return_type = std::invoke_result_t<F&&, Args&&...>;

		auto task = std::make_shared<std::packaged_task<return_type()>>(
				[&]() -> return_type
				{
					return std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
				}
			);

		std::future<return_type> result = task->get_future();
		{
			std::unique_lock<std::mutex> lk(this->_mtx);
			if(this->_stop)
				return std::future<return_type>();
			this->_jobs.emplace([task]() { (*task)(); });
		}
		this->_cv.notify_one();
		return result;
	}
};

}


#endif