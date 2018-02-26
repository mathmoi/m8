/// @file   EvalOptions.hpp
/// @author Mathieu Pagé
/// @date   Feburary 2017
/// @brief  Class containings the parameters used by the evaluation function.

#ifndef M8_EVAL_OPTIONS_HPP_
#define M8_EVAL_OPTIONS_HPP_

#include "../../m8chess/Piece.hpp"

namespace m8
{
    /// Options for .
    class EvalOptions
    {
    public:
        /// Set the value of a pawn.
        inline void set_pawn(int value) { pawn_ = value; }

        /// Value of a pawn.
        inline int pawn() const { return pawn_; }

        /// Set the value of a knight.
        inline void set_knight(int value) { knight_ = value; }

        /// Value of a knight.
        inline int knight() const { return knight_; }

        /// Set the value of a bishop.
        inline void set_bishop(int value) { bishop_ = value; }

        /// Value of a bishop.
        inline int bishop() const { return bishop_; }

        /// Set the value of a rook.
        inline void set_rook(int value) { rook_ = value; }

        /// Value of a rook.
        inline int rook() const { return rook_; }

        /// Set the value of a queen.
        inline void set_queen(int value) { queen_ = value; }

        /// Value of a queen.
        inline int queen() const { return queen_; }

        /// Return the value of a give piece_type.
        inline int get_value_piece_type(PieceType piece_type) const
        {
            assert(IsPieceType(piece_type));

            int value;

            switch (piece_type)
            {
            case kPawn:
                value = pawn_;
                break;
            case kKnight:
                value = knight_;
                break;
            case kBishop:
                value = bishop_;
                break;
            case kRook:
                value = rook_;
                break;
            case kQueen:
                value = queen_;
                break;
            default:
                value = 0;
                break;
            }

            return value;
        }

    private:
        int pawn_ = 100;
        int knight_ = 320;
        int bishop_ = 333;
        int rook_ = 510;
        int queen_ = 880;
    };
}

#endif // M8_EVAL_OPTIONS_HPP_