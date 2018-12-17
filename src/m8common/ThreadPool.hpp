/// @file   ThreadPool.hpp
/// @author Mathieu Pagé
/// @date   March 2018
/// @brief  Contains the definition of the ThreadPool class.

#ifndef M8_THREAD_POOL_HPP_
#define M8_THREAD_POOL_HPP_

#include <functional>
#include <vector>
#include <thread>
#include <deque>
#include <mutex>
#include <condition_variable>

namespace m8 {

    /// Implement a simple thread pool.
    class ThreadPool
    {
    public :
        using Task = std::function<void()>;

        /// Constructor
        ThreadPool(size_t threads_count);

        /// Destructor
        ~ThreadPool();

        /// Add a task to be processed.
        void Push(Task);

    private:
        void RunThread();

        std::vector<std::thread> workers_;
        std::deque<Task> tasks_;
        std::mutex mutex_;
        std::condition_variable condition_variable_;
        bool stop_;

    };

}

#endif // M8_THREAD_POOL_HPP_