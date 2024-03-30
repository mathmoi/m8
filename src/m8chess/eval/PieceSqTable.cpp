/// @file   PieceSqTable.cpp
/// @author Mathieu Pagé
/// @date   Feburary 2018
/// @brief  Contains functionalities about the piece-square table evaluation features.

#include "m8chess/eval/PieceSqTable.hpp"
#include "m8chess/Board.hpp"
#include "m8common/options/Options.hpp"

namespace m8::eval {

    PhasePieceSqTable gPieceSqTable;

    void GeneratePieceSqTable(SinglePieceSqTable& single_psqt,
                              options::PieceSquareTable::SinglePieceSqTable& options_psqt,
                              EvalType piece_value,
                              Color color)
    {
        auto multiplier = color == kWhite ? 1 : -1;
        for (size_t sq = 0; sq < kNumSqOnBoard; ++sq)
        {
            single_psqt[GetColorWiseSq(color, sq)] = (piece_value + options_psqt[sq]) * multiplier;
        }
    }

    void InitializePieceSqTable()
    {
        auto& options = options::Options::get();
        for (int index_phase = 0; index_phase < static_cast<int>(GamePhase::Count); ++index_phase)
        {
            auto& phase_psqt = index_phase == static_cast<int>(GamePhase::MiddleGame)
                             ? options.piece_square_table.middle_game
                             : options.piece_square_table.end_game;
            auto& phase_piece_value = index_phase == static_cast<int>(GamePhase::MiddleGame)
                                    ? options.pieces_values_middle_game
                                    : options.pieces_values_end_game;

            for (Color color = kWhite; IsColor(color); ++color)
            {
                GeneratePieceSqTable(gPieceSqTable[index_phase][NewPiece(kPawn,   color)], phase_psqt.pawn,   phase_piece_value.pawn,   color);
                GeneratePieceSqTable(gPieceSqTable[index_phase][NewPiece(kKnight, color)], phase_psqt.knight, phase_piece_value.knight, color);
                GeneratePieceSqTable(gPieceSqTable[index_phase][NewPiece(kBishop, color)], phase_psqt.bishop, phase_piece_value.bishop, color);
                GeneratePieceSqTable(gPieceSqTable[index_phase][NewPiece(kRook,   color)], phase_psqt.rook,   phase_piece_value.rook,   color);
                GeneratePieceSqTable(gPieceSqTable[index_phase][NewPiece(kQueen,  color)], phase_psqt.queen,  phase_piece_value.queen,  color);
                GeneratePieceSqTable(gPieceSqTable[index_phase][NewPiece(kKing,   color)], phase_psqt.king,   phase_piece_value.king,   color);
            }
        }
    }
}