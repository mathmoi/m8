/// @file   InvalidMoveException.hpp
/// @author Mathieu Pag�
/// @date   January 2020
/// @brief  Contains an exception classe used to indicate that an invalid move was tried.

#ifndef M8_INVALID_MOVE_EXCEPTION_HPP_
#define M8_INVALID_MOVE_EXCEPTION_HPP_

#include <stdexcept>

namespace m8::engine
{
    /// Exception used when a string can't be parsed as a SAN move.
    class InvalidMoveException : public std::runtime_error
    {
    public:
        InvalidMoveException()
            : runtime_error("Invalid move")
        {};

        InvalidMoveException(std::string message)
            : runtime_error(message)
        {};
    };
}

#endif // M8_INVALID_MOVE_EXCEPTION_HPP_



