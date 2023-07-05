/// @file TimeControl.hpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023
/// @brief Contains the TimeControl class.

#ifndef M8_CHESS_TIME_TIME_CONTROL_HPP_
#define M8_CHESS_TIME_TIME_CONTROL_HPP_

namespace m8::time
{
    /// Enumeration of the time controls types
    enum class TimeControlType
    {
        /// Conventional time control (x moves in y minutes)
        Conventional,

        /// Incremental time control (x minutes + y seconds per move)
        Incremental,

        /// Fixed time per move time control
        TimePerMove
    };

    /// Base class for all time controls types.
    class TimeControl
    {
    protected:
        /// Constructor
        /// 
        /// @param type Type of the TimeControl
        inline TimeControl(TimeControlType type)
        : type_(type)
        {}

    public:
        /// Returns the type of time control
        inline TimeControlType type() const { return type_; }
        
    private:
        TimeControlType type_;
    };
}

#endif // M8_CHESS_TIME_TIME_CONTROL_HPP_