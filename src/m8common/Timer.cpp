/// @file Timer.cpp
/// @author Mathieu Pagé
/// @date March 2024
/// @brief Contains the Timer class implementation

#include "m8common/Timer.hpp"

namespace m8
{
    Timer::Timer(TimerDirection direction, ClockType::duration initial_time_on_clock)
    : direction_(direction),
      initial_time_on_clock_(initial_time_on_clock)
    {}

    Timer::ClockType::duration Timer::elapsed() const
    {
        return ClockType::now() - start_time_.value();
    }

    Timer::ClockType::duration Timer::time_on_clock() const
    {
        auto time_on_clock = initial_time_on_clock_;
        if (start_time_)
        {
            time_on_clock += elapsed() * static_cast<std::int32_t>(direction_);
        }
        return time_on_clock;
    }

    void Timer::Reset(ClockType::duration time)
    {
        initial_time_on_clock_ = time;
        if (start_time_)
        {
            start_time_ = ClockType::now();
        }
    }

    void Timer::Start()
    {
        if (!start_time_)
        {
            start_time_ = ClockType::now();
        }
    }

    void Timer::Stop()
    {
        initial_time_on_clock_ = time_on_clock();
        start_time_.reset();
    }
}