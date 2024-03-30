/// @file PieceSquareTable.hpp
/// @author Mathieu Pagé
/// @date March 2024
/// Contains the PieceSquareTable structure.

#ifndef M8_OPTIONS_PIECE_SQUARE_TABLE_HPP_
#define M8_OPTIONS_PIECE_SQUARE_TABLE_HPP_

namespace m8::options
{
    struct PieceSquareTable
    {
        typedef std::array<std::int16_t, 64> SinglePieceSqTable;

        /// Piece squate table for a pawn.
        SinglePieceSqTable pawn;

        /// Piece squate table for a knight.
        SinglePieceSqTable knight;

        /// Piece squate table for a bishop.
        SinglePieceSqTable bishop;

        /// Piece squate table for a rook.
        SinglePieceSqTable rook;

        /// Piece squate table for a queen.
        SinglePieceSqTable queen;

        /// Piece squate table for a king.
        SinglePieceSqTable king;
    };
}

#endif // M8_OPTIONS_PIECE_SQUARE_TABLE_HPP_