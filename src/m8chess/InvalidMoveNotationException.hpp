/// @file InvalidMoveNotationException.hpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// 
/// @date December 2022
///
/// Contains the InvalidMoveNotationException class

#ifndef M8_CHESS_INVALID_MOVE_NOTATION_EXCEPTION_HPP_
#define M8_CHESS_INVALID_MOVE_NOTATION_EXCEPTION_HPP_

#include <stdexcept>

namespace m8 {

    /// Exception used when a string can't be parsed as a SAN move.
    class InvalidMoveNotationException : public std::runtime_error
    {
    public:
        InvalidMoveNotationException()
            : runtime_error("Invalid move")
        {};

        InvalidMoveNotationException(std::string message)
            : runtime_error(message)
        {};
    };
}

#endif // M8_CHESS_INVALID_MOVE_NOTATION_EXCEPTION_HPP_