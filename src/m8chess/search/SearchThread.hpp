/// @file   SearchThread.hpp
/// @author Mathieu Pagé
/// @date   March 2018
/// @brief  Contains the SearchThread class. This class represent an Alpha-Beta search 
///         thread.

#ifndef M8_SEARCH_THREAD_HPP_
#define M8_SEARCH_THREAD_HPP_

#include <thread>
#include <mutex>
#include <condition_variable>

namespace m8 { namespace search
{
    /// The class represent an Alpha-Beta search thread.
    class SearchThread
    {
    private:
        class Impl
        {
        public:
            /// Default constructor.
            Impl();

            /// Destructor
            ~Impl();

            void Start(std::shared_ptr<int> job);

        private:
            /// Main method of the thread. This method waits for works then execute it.
            void ThreadLoop();
            std::shared_ptr<int> GetNextJob();
            void ExecuteJob(std::shared_ptr<int> job);

            std::thread thread_;

            /// Indicate that the main thread loop can continue to wait on more work.
            bool continue_loop;

            std::mutex mutex_;
            std::condition_variable condition_variable_;

            std::shared_ptr<int> job_;
        };

    public:
        /// Constructor
        SearchThread();

        /// Start the search
        inline void Start(std::shared_ptr<int> job) { impl_->Start(job); };

    private:
        std::unique_ptr<Impl> impl_;
    };

}}

#endif // M8_SEARCH_THREAD_HPP_