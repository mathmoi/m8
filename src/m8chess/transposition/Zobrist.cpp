/// @file Zobrist.cpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023

#include <limits>
#include <random>

#include "../Piece.hpp"
#include "../Sq.hpp"

#include "Zobrist.hpp"

namespace m8::transposition
{
    ZobristTable gZobristTable;
    std::array<ZobristKey, kNumColmnOnBoard> gZobristEnPassant;
    ZobristKey gZobristBlackToMove;
    std::array<ZobristKey, 16> gZobristCastling;


    void InitializeZobrist()
    {
        std::mt19937 generator(42); // The generator is seeded with a constant so it will 
                                    // always generate the same keys.
        std::uniform_int_distribution<ZobristKey> dist(std::numeric_limits<ZobristKey>::min(),
                                                       std::numeric_limits<ZobristKey>::max());

        gZobristBlackToMove = dist(generator);

        for (auto piece : all_pieces)
        {
            for (Sq sq = 0; sq < kNumSqOnBoard; ++sq)
            {
                gZobristTable[piece][sq] = dist(generator);
            }
        }

        for (auto& key : gZobristEnPassant)
        {
            key = dist(generator);
        }

        for (auto& key : gZobristCastling)
        {
            key = dist(generator);
        }
    }
}