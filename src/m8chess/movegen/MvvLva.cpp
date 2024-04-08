/// @file MvvLva.cpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date August 2023

#include "m8chess/eval/Eval.hpp"

#include "m8chess/movegen/MvvLva.hpp"

namespace m8::movegen
{
    MvvLvaPrecalc kMvvLvaValues;

    EvalType CalculateMvvLva(PieceType victim, PieceType attacker, PieceType promotion)
    {
        // We add the value of the victim (MVV) and promotion differential
        EvalType value = eval::kPieceTypeValues[static_cast<std::size_t>(eval::GamePhase::MiddleGame)][victim];
        if (IsPieceType(promotion))
        {
            value += eval::kPieceTypeValues[static_cast<std::size_t>(eval::GamePhase::MiddleGame)][promotion]
                   - eval::kPieceTypeValues[static_cast<std::size_t>(eval::GamePhase::MiddleGame)][kPawn];
        }

        // We scale the value of the victim by 16 so it is the most important factor
        value *= 16;

        // We remove the value of the attacker (LVA)
        value -= eval::kPieceTypeValues[static_cast<std::size_t>(eval::GamePhase::MiddleGame)][attacker];

        return value;        
    }

    void InitializeMvvLva()
    {
        // For simplicity we generate the MVV/LVA value for all combination of victim, 
        // attacker and promotion, even for thoses that don't makes sens. Theses will
        // just be unused.
        for (auto victim : all_piece_types)
        {
            for (auto attacker : all_piece_types)
            {
                for (auto promotion : all_piece_types)
                {
                    kMvvLvaValues[victim][attacker][promotion] = CalculateMvvLva(victim, attacker, promotion);
                }

                kMvvLvaValues[victim][attacker][kNoPiece] = CalculateMvvLva(victim, attacker, kNoPiece);
            }
        }

        // We also compute MVV/LVA for pure promotion (no victim)
        kMvvLvaValues[kNoPiece][kPawn][kQueen]  = CalculateMvvLva(kNoPiece, kPawn, kQueen);
        kMvvLvaValues[kNoPiece][kPawn][kRook]   = CalculateMvvLva(kNoPiece, kPawn, kRook);
        kMvvLvaValues[kNoPiece][kPawn][kBishop] = CalculateMvvLva(kNoPiece, kPawn, kBishop);
        kMvvLvaValues[kNoPiece][kPawn][kKnight] = CalculateMvvLva(kNoPiece, kPawn, kKnight);
        
    }

    EvalType GetMvvLvaValue(Move move)
    {
        // TODO : Is it really faster to pre-compute MVV_LVA?
        auto victim    = GetPieceType(GetPieceTaken(move));
        auto attacker  = GetPieceType(GetPiece(move));
        auto promotion = GetPieceType(GetPromoteTo(move));
        return kMvvLvaValues[victim][attacker][promotion];
    }
}