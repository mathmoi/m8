/// @file mvvlva.hpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date August 2023

#ifndef M8_CHESS_MOVEGEN_MVV_LVA_HPP_
#define M8_CHESS_MOVEGEN_MVV_LVA_HPP_

#include "../Move.hpp"
#include "../Piece.hpp"
#include "../Types.hpp"



namespace m8::movegen
{
    typedef std::array<
                std::array<
                    std::array<EvalType,
                               kMaxPieceType + 1>,
                    kMaxPieceType + 1>,
                kMaxPieceType + 1> MvvLvaPrecalc;

    extern MvvLvaPrecalc kMvvLvaValues;

    void InitializeMvvLva();

    EvalType GetMvvLvaValue(Move move);
}

#endif // M8_CHESS_MOVEGEN_MVV_LVA_HPP_