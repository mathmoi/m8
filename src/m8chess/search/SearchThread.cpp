/// @file   SearchThread.cpp
/// @author Mathieu Pagé
/// @date   March 2018
/// @brief  Contains the SearchThread class. This class represent an Alpha-Beta search 
///         thread.

#include <iostream>

#include "SearchThread.hpp"

namespace m8 { namespace search
{
    void SearchThread::Impl::ExecuteJob(std::shared_ptr<int> job)
    {
        while (continue_loop && *job > 0)
        {
            std::cout << "working " << *job << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            --(*job);
        }
    }

    std::shared_ptr<int> SearchThread::Impl::GetNextJob()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_variable_.wait(lock, [this]() { return !continue_loop || job_; });

        auto job = job_;
        job_.reset();

        return job;
    }

    void SearchThread::Impl::ThreadLoop()
    {
        std::cout << "Thread loop begin" << std::endl;
        while (continue_loop)
        {
            auto job = GetNextJob();
            
            if (job)
            {
                ExecuteJob(job);
            }
        }
        std::cout << "Thread loop end" << std::endl;
    }

    SearchThread::Impl::Impl()
        : thread_(&SearchThread::Impl::ThreadLoop, this),
          continue_loop(true)
    {
        std::cout << "SearchThread Constructed" << std::endl;
    }

    SearchThread::Impl::~Impl()
    {
        std::cout << "SearchThread destructed" << std::endl;
        if (thread_.joinable())
        {
            std::cout << "SearchThread joining" << std::endl;
            {
                std::lock_guard<std::mutex> guard(mutex_);
                continue_loop = false;
                condition_variable_.notify_one();
            }
            thread_.join();
            std::cout << "SearchThread joined" << std::endl;
        }
    }

    void SearchThread::Impl::Start(std::shared_ptr<int> job)
    {
        std::lock_guard<std::mutex> guard(mutex_);
        job_ = job;
        condition_variable_.notify_one();
    }

    SearchThread::SearchThread()
        : impl_(std::make_unique<Impl>())
    {}
}}