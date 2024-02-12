/// @file IncrementalChessClock.hpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023
/// @brief Contains the IncrementalChessClock class.

#ifndef M8_TIME_INCREMENTAL_CHESS_CLOCK_HPP_
#define M8_TIME_INCREMENTAL_CHESS_CLOCK_HPP_

#include "ChessClock.hpp"
#include "IncrementalTimeControl.hpp"

namespace m8::time
{
    /// Chess clock that handles incremental time controls.
    class IncrementalChessClock : public ChessClock
    {
    public:
        IncrementalChessClock(IncrementalTimeControl time_control)
        : ChessClock(time_control.base() + time_control.increment()),
          time_control_(time_control)
        {}

        /// Stops the clock and adds the increment to the time on the clock. The clock
        /// must previously be running.
        inline virtual void Stop()
        {
            ChessClock::Stop();
            set_time_on_clock(time_on_clock() + time_control_.increment());
        }

    private:
        IncrementalTimeControl time_control_;
    };
}

#endif // M8_TIME_INCREMENTAL_CHESS_CLOCK_HPP_