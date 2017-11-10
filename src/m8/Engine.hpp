/// @file	Engine.hpp
/// @author Mathieu Pagé
/// @date	November 2017
/// @brief	Contains the Engine class that encapsulate all m8 functionalities.

#ifndef M8_ENGINE_HPP_
#define M8_ENGINE_HPP_

#include "../m8chess/Board.hpp"

namespace m8
{
    /// Encapsulate all m8 functionalities
    class Engine
    {
    public:

        /// Constructor
        Engine();

        /// Accessor for the board
        ///
        /// @returns The board
        inline const Board& board() const { return board_; };

        /// Set the board position using a fen string.
        ///
        /// @param fen XFen string representing the new position.
        inline void set_fen(std::string fen) { board_ = Board(fen); };
        
    private:
        Board board_;
    };
}

#endif