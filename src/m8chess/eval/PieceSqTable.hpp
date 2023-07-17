/// @file   PieceSqTable.hpp
/// @author Mathieu Pag�
/// @date   Feburary 2015
/// @brief  Contains functionalities about the piece-square table evaluation features.

#ifndef M8_EVAL_PSQT_HPP_
#define M8_EVAL_PSQT_HPP_

#include <array>
#include <memory>

#include "../Piece.hpp"

namespace m8::eval
{
    /// Type of a piece square table.
    typedef std::array<int, 64> SinglePieceSqTable;
    typedef std::array<SinglePieceSqTable, kMaxPiece + 1> PieceSqTable;
    typedef std::shared_ptr<PieceSqTable> PieceSqTablePtr;

    /// Generate a piece-square table where all the values are zero.
    PieceSqTablePtr GenerateEmptyPieceSqTable();

    /// Generates the piece square table using the parameters read with the Options class.
    ///
    /// @returns A shared pointer to a newly created piece-square table.
    PieceSqTablePtr GeneratePieceSqTable();
}

#endif // M8_EVAL_PSQT_HPP_