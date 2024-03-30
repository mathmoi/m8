/// @file   PieceSqTable.hpp
/// @author Mathieu Pagé
/// @date   Feburary 2015
/// @brief  Contains functionalities about the piece-square table evaluation features.

#ifndef M8_EVAL_PSQT_HPP_
#define M8_EVAL_PSQT_HPP_

#include <array>

#include "m8chess/Piece.hpp"
#include "m8chess/Types.hpp"

#include "GamePhase.hpp"

namespace m8::eval
{
    /// Type of a piece square table.
    typedef std::array<EvalType, 64> SinglePieceSqTable;
    typedef std::array<SinglePieceSqTable, kMaxPiece + 1> PieceSqTable;
    typedef std::array<PieceSqTable, static_cast<int>(GamePhase::Count)> PhasePieceSqTable;
    
    extern PhasePieceSqTable gPieceSqTable;

    /// Initialize the piece square table using the parameters read with the Options class.
    void InitializePieceSqTable();
}

#endif // M8_EVAL_PSQT_HPP_