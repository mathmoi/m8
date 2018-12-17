/// @file   ThreadPool.cpp
/// @author Mathieu Pagé
/// @date   Feburary 2018
/// @brief  Contains the ThreadPools definitions.

#include "ThreadPool.hpp"

namespace m8
{
    ThreadPool::ThreadPool(size_t threads_count)
        : stop_(false),
          workers_(threads_count)
    {
        for (size_t x = 0; x < threads_count; ++x)
        {
            workers_.push_back(std::thread(&ThreadPool::RunThread, this));
        }
    }

    ThreadPool::~ThreadPool()
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            stop_ = true;
            condition_variable_.notify_all();
        }

        for (auto& worker : workers_)
        {
            if (worker.joinable())
            {
                worker.join();
            }
        }
    }

    void ThreadPool::Push(ThreadPool::Task task)
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            tasks_.push_back(task);
        }

        condition_variable_.notify_one();
    }

    void ThreadPool::RunThread()
    {
        Task task;
        while (true)
        {
            {
                std::unique_lock<std::mutex> lock(mutex_);

                while (!stop_ && tasks_.empty())
                {
                    condition_variable_.wait(lock);
                }

                if (stop_)
                {
                    return;
                }

                task = tasks_.front();
                tasks_.pop_front();
            }

            task();
        }
    }
}