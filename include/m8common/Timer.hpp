/// @file Timer.hpp
/// @author Mathieu Pagé
/// @date March 2024
/// @brief Contains the Timer class definition

#ifndef M8_TIMER_HPP_
#define M8_TIMER_HPP_

#include <chrono>
#include <optional>

namespace m8
{
    /// Direction of a timer.
    enum class TimerDirection : std::int32_t
    {
        Up   = 1,
        Down = -1
    };

    /// Implement a simple timer that can count time up or down
    class Timer
    {
    public:
        typedef std::chrono::steady_clock ClockType;

        /// Constructor
        /// 
        /// @param direction             Direction the timer is counting time.
        /// @param initial_time_on_clock Initial time on the timer.
        Timer(TimerDirection direction, ClockType::duration initial_time_on_clock = std::chrono::seconds(0));

        /// Time currently on the clock
        ClockType::duration time_on_clock() const;

        /// Time elapsed since the last time the Timer was started.
        ClockType::duration elapsed() const;

        /// Reset the time on the clock
        void Reset(ClockType::duration time = std::chrono::seconds(0));

        /// Starts the timer.
        void Start();

        /// Stops the time.
        void Stop();
        
    private:
        TimerDirection direction_;
        ClockType::duration initial_time_on_clock_;
        std::optional<ClockType::time_point> start_time_;
    };
}

#endif // M8_TIMER_HPP_