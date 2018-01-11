/// @file   Perft.hpp
/// @author Mathieu Pagé
/// @date   Janurary 2018
/// @brief  Contains functionalities of the perft command.

#ifndef M8_PERFT_HPP_
#define M8_PERFT_HPP_

#include <cstdint>
#include <functional>

#include "Move.hpp"
#include "Board.hpp"

namespace m8
{
    /// Result of a perft test
    struct PerftResult
    {
        /// Number of lead nodes visited
        std::uint64_t nodes;

        /// Time in seconds 
        double seconds;
    };

    /// Run a perft tests.
    ///
    /// @param depth    Depth of the test to run.
    /// @param board    Position to run the test on.
    /// @param callback Method to call after each root move to give a subcount of the 
    ///                 nodes.
    PerftResult Perft(int depth, Board& board, std::function<void(Move, std::uint64_t)> callback);
}

#endif // M8_PERFT_HPP_