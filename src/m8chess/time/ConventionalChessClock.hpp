/// @file ConventionalChessClock.hpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023
/// @brief Contains the ConventionalChessClock class.

#ifndef M8_TIME_CONVENTIONAL_CHESS_CLOCK_HPP_
#define M8_TIME_CONVENTIONAL_CHESS_CLOCK_HPP_

#include "ChessClock.hpp"
#include "ConventionalTimeControl.hpp"

namespace m8::time
{
    /// Chess clock that handles incremental time controls.
    class ConventionalChessClock : public ChessClock
    {
    public:
        ConventionalChessClock(ConventionalTimeControl time_control)
        : ChessClock(time_control.duration()),
          time_control_(time_control),
          moves_before_control_(time_control.moves())
        {}

        /// Stops the clock and adds time to the clock if we finised a control block. The
        /// clock must previously be running.
        inline virtual void Stop()
        {
            ChessClock::Stop();

            --moves_before_control_;

            if (moves_before_control_ <= 0)
            {
                set_time_on_clock(time_on_clock() + time_control_.duration());
                moves_before_control_ = time_control_.moves();
            }
        }

    private:
        ConventionalTimeControl time_control_;
        ConventionalTimeControl::NumberOfMoves moves_before_control_;
    };
}

#endif // M8_TIME_CONVENTIONAL_CHESS_CLOCK_HPP_
