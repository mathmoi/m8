/// @file MoveLegality.hpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date August 2023

#ifndef M8_CHESS_MOVE_LEGALITY_HPP_
#define M8_CHESS_MOVE_LEGALITY_HPP_

#include "movegen/MoveGeneration.hpp"

#include "Board.hpp"

namespace m8
{

    /// Check if a move is pseudo legal
    inline bool IsPseudoLegal(const Board& board, Move move)
    {
        auto piece = GetPiece(move);
        auto color = GetColor(piece);

        // Check that the correct side is on move.
        if (color != board.side_to_move())
        {
            return false;
        }

        // Check that the piece is on the from square.
        auto from = GetFrom(move);
        if (board[from] != piece)
        {
            return false;
        }

        // Check that the taken piece if any is on the correct square.
        auto piece_taken = GetPieceTaken(move);
        auto to = GetTo(move);
        if (IsPiece(piece_taken))
        {
            auto pos_piece_taken = GetTo(move);

            // if the move is a prise en passant we need to adjust pos_piece_taken
            if (board.colmn_enpas() == GetColmn(to)
                && kPawn == GetPieceType(piece)
                && kPawn == GetPieceType(piece_taken)
                && GetColorWiseRow(color, kRow6) == GetRow(to))
            {
                pos_piece_taken = NewSq(board.colmn_enpas(),  GetColorWiseRow(color, kRow5));
            }

            if (board[pos_piece_taken] != piece_taken)
            {
                return false;
            }
        }

        // If it's a castling move, check that it is legal, that the squares 
        // between the rook and king original and final positions are empty and
        // that the squares traveled by the kink are not attacked by the 
        // opponent.
        auto castle_type = GetCastling(move);
        if (kNoCastling != castle_type)
        {
            if (!board.casle(color, castle_type))
            {
                return false;
            }

            auto row = GetRow(from);
            auto rook_original_column = board.casle_colmn(castle_type);
            auto rook_position = NewSq(rook_original_column, row);
            auto rook_final_column = castle_type = kQueenSideCastle ? kColmnD : kColmnF;
            auto rook_final_position = NewSq(rook_final_column, row);

            Bb bb_travel_king = BbBetween(from, to);
            Bb bb_travel_rook = BbBetween(rook_position, rook_final_position);

            // Check if any of the travel squared is occupied.
            Bb occ = board.bb_occupied();
            auto bb_king = GetSingleBitBb(from);
            auto bb_rook = GetSingleBitBb(rook_position);
            occ ^= bb_king | bb_rook;
            bool travel_occupied = (occ & (bb_travel_king | bb_travel_rook)) != kEmptyBb;
            if (travel_occupied)
            {
                return false;
            }

            // Check if any of the square traveled by the king or the origin or 
            // destination of the king are under attack.
            Bb bb_check_attack = bb_travel_king | bb_king | GetSingleBitBb(to);
            Bb bb_opponents = board.bb_color(OpposColor(color));

            while (bb_check_attack)
            {
                Sq pos = RemoveLsb(bb_check_attack);
                if ((movegen::AttacksTo(board, pos) & bb_opponents) != kEmptyBb)
                {
                    return false;
                }
            }
        }

        // If it is a pawn move that move two square we need to check if the square just 
        // in front of the pawn is empty.
        auto piece_type = GetPieceType(piece);
        if (kPawn == piece_type
            && GetColorWiseRow(color, kRow2) == GetRow(from)
            && GetColorWiseRow(color, kRow4) == GetRow(to))
        {
            auto jumped_sq = NewSq(GetColmn(from), GetColorWiseRow(color, kRow3));
            if (board[jumped_sq] != kNoPiece)
            {
                return false;
            }
        }

        // If it is not a pawn move or a castling move, the origin must attack the
        // destination.
        if (kPawn != piece_type
            && castle_type == kNoCastling)
        {
            if ((movegen::AttacksFrom(board, from) & GetSingleBitBb(to)) == kEmptyBb)
            {
                return false;
            }
        }

        return true;
    }
}

#endif // M8_CHESS_MOVE_LEGALITY_HPP_