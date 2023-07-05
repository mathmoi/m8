/// @file TimePerMoveTimeControl.hpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023
/// @brief Contains the TimePerMoveTimeControl class.

#ifndef M8_TIME_TIME_PER_MOVE_TIME_CONTROL_HPP_
#define M8_TIME_TIME_PER_MOVE_TIME_CONTROL_HPP_

#include <chrono>

#include "TimeControl.hpp"

namespace m8::time
{

    /// Represents an time per move time control
    class TimePerMoveTimeControl : public TimeControl
    {
    public:
        typedef std::chrono::steady_clock::duration Duration;

        /// Constructor
        /// 
        /// @param time_per_move time per move.
        inline TimePerMoveTimeControl(Duration time_per_move)
        : TimeControl(TimeControlType::TimePerMove),
          time_per_move_(time_per_move)
        {};

        /// Returns the time per move
        inline Duration time_per_move() const { return time_per_move_; }

    private:
        Duration time_per_move_;
    }; 

}

#endif // M8_TIME_TIME_PER_MOVE_TIME_CONTROL_HPP_