/// @file IncrementalTimeControl.hpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023
/// @brief Contains the IncrementalTimeControl class.

#ifndef M8_TIME_INCREMENTAL_TIME_CONTROL_HPP_
#define M8_TIME_INCREMENTAL_TIME_CONTROL_HPP_

#include <chrono>

#include "TimeControl.hpp"

namespace m8::time
{

    /// Represents an incremental time control (x seconds + y seconds per move).
    class IncrementalTimeControl : public TimeControl
    {
    public:
        typedef std::chrono::steady_clock::duration Duration;

        /// Constructor
        /// 
        /// @param base      Base time for the whole game.
        /// @param increment Increment added after each moves.
        inline IncrementalTimeControl(Duration base, Duration increment)
        : TimeControl(TimeControlType::Incremental),
          base_(base),
          increment_(increment)
        {};

        /// Returns the base time for the whole game.
        inline Duration base() const { return base_; }

        /// Returns the increment added after each moves.
        inline Duration increment() const { return increment_; }
          
    private:
        Duration base_;
        Duration increment_;
    }; 

}

#endif // M8_TIME_INCREMENTAL_TIME_CONTROL_HPP_