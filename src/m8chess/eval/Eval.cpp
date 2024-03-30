/// @file Eval.cpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date August 2023

#include <m8common/options/Options.hpp>

#include "m8chess/eval/GamePhase.hpp"
#include "m8chess/eval/Eval.hpp"

namespace m8::eval
{
    /// Values of the pieces type indexed by GamePhase, then by PieceType
    std::array<std::array<EvalType, kMaxPieceType + 1>, static_cast<int>(GamePhase::Count)> kPieceTypeValues;

    void InitializeEval()
    {
        auto& options = options::Options::get();

        for (int game_phase_index = 0; game_phase_index < static_cast<int>(GamePhase::Count); ++game_phase_index)
        {
            kPieceTypeValues[game_phase_index][kNoPieceType] = 0;

            auto& options_pieces_value_phase = game_phase_index == static_cast<int>(GamePhase::MiddleGame)
                                             ? options.pieces_values_middle_game
                                             : options.pieces_values_end_game;
            kPieceTypeValues[game_phase_index][kPawn]   = options_pieces_value_phase.pawn;
            kPieceTypeValues[game_phase_index][kKnight] = options_pieces_value_phase.knight;
            kPieceTypeValues[game_phase_index][kBishop] = options_pieces_value_phase.bishop;
            kPieceTypeValues[game_phase_index][kRook]   = options_pieces_value_phase.rook;
            kPieceTypeValues[game_phase_index][kQueen]  = options_pieces_value_phase.queen;
            kPieceTypeValues[game_phase_index][kKing]   = options_pieces_value_phase.king;
        }
    }
}