/// @file   ChessClock.hpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @date   July 2023
/// @brief  Contains the ChessClock class.

#ifndef M8_CHESS_CLOCK_HPP_
#define M8_CHESS_CLOCK_HPP_

#include <cassert>
#include <chrono>
#include <memory>
#include <optional>

#include "TimeControl.hpp"

namespace m8::time {

    // TODO : Remove this class it's replaced by Timer

    /// Represents a chess clock that can be started and stopped.
    class ChessClock
    {
    public:
        typedef std::chrono::steady_clock::duration Duration;

        /// Create a ChessClock object
        /// 
        /// @param time_control Time control to use.
        static std::unique_ptr<ChessClock> CreateChessClock(const TimeControl& time_control);

        /// Constructor
        /// 
        /// @param time_on_clock Initial time on the clock
        inline ChessClock(Duration time_on_clock)
        : time_on_clock_(time_on_clock),
          start_time_()
        {}

        /// Time since the last time the clock was started. Returns 0 if the clock is not
        /// running.
        inline Duration elapsed() const
        {
            if (start_time_)
            {
                return std::chrono::steady_clock::now() - start_time_.value();
            }

            return Duration(0);
        }

        /// Returns the remaining time on the clock.
        inline Duration time_on_clock() const { return time_on_clock_ - elapsed(); }

        /// Indicate if the clock is currently running.
        inline bool is_running() const { return start_time_.has_value(); }

        /// Starts the clock. The clock must previously be in a stopped state.
        inline virtual void Start()
        {
            assert(!is_running());
            start_time_ = std::chrono::steady_clock::now();
        }

        /// Stops the clock. The clock must previously be running.
        inline virtual void Stop()
        {
            assert(is_running());
            time_on_clock_ -= elapsed();
            start_time_.reset();
        }

        /// Sets the time on the clock. The clock must previously be in a stopped state.
        inline void set_time_on_clock(Duration time_on_clock)
        {
            assert(!is_running());
            time_on_clock_ = time_on_clock;
        }

    private:
        Duration time_on_clock_;
        std::optional<std::chrono::steady_clock::time_point> start_time_;
    };
}

#endif // M8_CHESS_CLOCK_HPP_
