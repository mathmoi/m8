/// @file Zobrist.hpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023

#ifndef M8_TRANSPOSITION_ZOBRIST_HPP_
#define M8_TRANSPOSITION_ZOBRIST_HPP_

#include <array>

#include "../Piece.hpp"
#include "../Sq.hpp"

namespace m8::transposition
{
    /// Type of a Zobrist hash key
    typedef std::uint64_t ZobristKey;

    /// Array of 64 zobrist hash keys for a single piece
    typedef std::array<ZobristKey, 64> PieceZobristTable;

    /// Array of zobrist keys for all the pieces
    typedef std::array<PieceZobristTable, kMaxPiece + 1> ZobristTable;

    /// Global array containing the zobrist keys
    extern ZobristTable gZobristTable;

    /// Array containing zobrist keys for columns that can be taken en passant
    extern std::array<ZobristKey, kNumColmnOnBoard> gZobristEnPassant;

    /// A single zobrist key to be used when it's black to move
    extern ZobristKey gZobristBlackToMove;

    /// Array of zobris keys for every possible values of the castling flags.
    extern std::array<ZobristKey, 16> gZobristCastling;

    /// Initialisation function for the global array of zobrist keys
    void InitializeZobrist();    
}

#endif // M8_TRANSPOSITION_ZOBRIST_HPP_

