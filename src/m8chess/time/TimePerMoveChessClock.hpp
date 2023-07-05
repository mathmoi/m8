/// @file TimePerMoveChessClock.hpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023
/// @brief Contains the TimePerMoveChessClock class.

#ifndef M8_TIME_TIME_PER_MOVE_CHESS_CLOCK_HPP_
#define M8_TIME_TIME_PER_MOVE_CHESS_CLOCK_HPP_

#include "ChessClock.hpp"
#include "TimePerMoveTimeControl.hpp"

namespace m8::time
{
    /// Chess clock that handles the time per move time control
    class TimePerMoveChessClock : public ChessClock
    {
    public:
        TimePerMoveChessClock(TimePerMoveTimeControl time_control)
        : ChessClock(time_control.time_per_move()),
          time_control_(time_control)
        {}

        /// Stops the clock and reset the time on the clock. The clock must previously be
        /// running.
        inline virtual void Stop()
        {
            ChessClock::Stop();
            set_time_on_clock(time_control_.time_per_move());
        }

    private:
        TimePerMoveTimeControl time_control_;
    };
}

#endif // M8_TIME_TIME_PER_MOVE_CHESS_CLOCK_HPP_