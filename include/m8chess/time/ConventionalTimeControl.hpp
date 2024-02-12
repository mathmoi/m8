/// @file ConventionalTimeControl.hpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023
/// @brief Contains the ConventionalTimeControl class.

#ifndef M8_TIME_CONVENTIONAL_TIME_CONTROL_HPP_
#define M8_TIME_CONVENTIONAL_TIME_CONTROL_HPP_

#include <chrono>

#include "TimeControl.hpp"

namespace m8::time
{

    /// Represents a conventional time control (x moves in y minutes).
    class ConventionalTimeControl : public TimeControl
    {
    public:
        typedef std::uint16_t NumberOfMoves;
        typedef std::chrono::steady_clock::duration Duration;

        /// Constructor
        /// 
        /// @param moves    Number of moves for each time block
        /// @param duration Time allowed at the begining of each block
        inline ConventionalTimeControl(NumberOfMoves moves, Duration duration)
        : TimeControl(TimeControlType::Conventional),
          moves_(moves),
          duration_(duration)
        {};

        /// Return the number of moves in each time block.
        inline NumberOfMoves moves() const { return moves_; };

        /// Return the time allowed for each block.
        inline Duration duration() const { return duration_; };
          
    private:
        NumberOfMoves moves_;
        Duration duration_;
    }; 

}

#endif // M8_TIME_CONVENTIONAL_TIME_CONTROL_HPP_