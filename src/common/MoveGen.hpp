/// @file   MoveGen.hpp
/// @author Mathieu Pagé
/// @date   December 2015
/// @brief  Contains the definition of types and utilities to generate moves

#include <cstdint>

#include "Bb.hpp"
#include "Sq.hpp"
#include "Piece.hpp"
#include "Move.hpp"
#include "Board.hpp"

#ifndef M8_MOVE_GEN_HPP_
#define M8_MOVE_GEN_HPP_

namespace m8
{
    /// Class containing all the functionalitues necessary to generate moves.
    class MoveGen
    {
    public:

        /// Constructor.
        ///
        /// @param board Board for which this object will generate the moves.
        inline MoveGen(const Board& board)
            : board_(board)
        {};

        /// Initialize the precalculated data associated with the MoveGen class. This 
        /// method should be called once before the class can be used.
        static void InitializePreCalc();

        /// Generate the moves of the knight. The moves are added to an array specified by
        /// the parameter next_move the end of the sequence of moves is indicated by a 
        /// null move (kNullMove).
        ///
        /// @param color       Color of the knight to generate the moves for.
        /// @param is_captures Indicate if we should generate captures or non captures 
        ///                    moves.
        /// @param next_move   Pointer into an array where we can add moves.
        /// @return A pointer to the position after the last move inserted into the array.
        inline Move* GenerateKnightMoves(Color color, bool is_captures, Move* next_move) const;

        /// Generate the moves of the king. The moves are added to an array specified by
        /// the parameter next_move.
        ///
        /// @param color       Color of the king to generate the moves for.
        /// @param is_captures Indicate if we should generate captures or non captures 
        ///                    moves.
        /// @param next_move   Pointer into an array where we can add moves.
        /// @return A pointer to the position after the last move inserted into the array.
        inline Move* GenerateKingMoves(Color color, bool is_captures, Move* next_move) const;

        /// Generate the moves of the pawns. The moves are added to an array specified by
        /// the parameter next_move.
        ///
        /// @param color       Color of the pawn to generate the moves for.
        /// @param next_move   Pointer into an array where we can add moves.
        /// @return A pointer to the position after the last move inserted into the array.
        inline Move* GeneratePawnMoves(Color color, Move* next_move) const;
    private:
        /// Type for an attack array for simples moves (knight and kings).
        typedef std::array<Bb, 64> AttackArray;

        /// Initialize the precalculation of knight_attack_bb_.
        static void InitializeKnightAttackBb();

        /// Initialize the precalculation of king_attack_bb_.
        static void InitializeKingAttackBb();

        /// Generates simples moves from an attack array (knights and king)
        ///
        /// @param color        Color of the pieces for which to generates moves.
        /// @param is_captures  Indicate if we should generate captures or non captures 
        ///                     moves.
        /// @param piece        Piece for which we generate the moves.
        /// @param bb_piece     Bitboard indicating the positions of the pieces for which
        ///                     to generate moves.
        /// @param attack_array Attack array for the type of piece for which to generate 
        ///                     moves.
        /// @param next_move    Pointer into an array of move where we can add moves.
        /// @return A pointer to the position after the last move inserted into the array.
        inline Move* GenerateSimpleMoves(Color color,
                                         bool is_captures,
                                         Piece piece,
                                         Bb bb_pieces,
                                         AttackArray attack_array,
                                         Move* next_move) const;

        static AttackArray knight_attack_bb_;
        static AttackArray king_attack_bb_;

        const Board& board_;
    };

    inline Move* MoveGen::GenerateSimpleMoves(Color color,
                                              bool is_captures,
                                              Piece piece,
                                              Bb bb_pieces,
                                              AttackArray attack_array,
                                              Move* next_move) const
    {
        Bb targets;
        if (is_captures)
            targets = board_.bb_color(OpposColor(color));
        else
            targets = ~board_.bb_occupied();

        while (bb_pieces)
        {
            Sq from = RemoveLsb(bb_pieces);
            Bb destinations = attack_array[from] & targets;

            while (destinations)
            {
                Sq to = RemoveLsb(destinations);
                *(next_move++) = NewMove(from, to, piece, board_[to]);
            }
        }

        return next_move;
    }

    inline Move* MoveGen::GenerateKnightMoves(Color color, bool is_captures, Move* next_move) const
    {
        Piece piece = NewPiece(kKnigt, color);
        return GenerateSimpleMoves(color,
                                   is_captures,
                                   piece,
                                   board_.bb_piece(piece),
                                   knight_attack_bb_,
                                   next_move);
    }

    inline Move* MoveGen::GenerateKingMoves(Color color, bool is_captures, Move* next_move) const
    {
        Piece piece = NewPiece(kKing, color);
        return GenerateSimpleMoves(color,
                                   is_captures,
                                   piece,
                                   board_.bb_piece(piece),
                                   king_attack_bb_,
                                   next_move);

        // TODO : Generate castling moves. We'll need a way to encode Chess960 castlings.
    }

    inline Move* MoveGen::GeneratePawnMoves(Color color, Move* next_move) const
    {
        Piece piece = NewPiece(kPawn, color);
        Row third_row = kRow3 + 3 * color;
        Row seventh_row = kRow7 - 5 * color;
        int forward_move = 8 - 16 * color;

        // Generate the standard one square forward moves. We need to exclude pawns on 
        // the 7th rank that will generate promotions.
        Bb target = board_.bb_piece(piece) & ~kBbRow[seventh_row];
        Shift(target, forward_move);
        target &= ~board_.bb_occupied();

        // Generate the two squares moves
        Bb target_dbl = target & kBbRow[third_row];
        Shift(target_dbl, forward_move);
        target_dbl &= ~board_.bb_occupied();

        while (target)
        {
            Sq to = RemoveLsb(target);
            Sq from = to - forward_move;
            *(next_move++) = NewMove(from, to, piece);
        }

        while (target_dbl)
        {
            Sq to = RemoveLsb(target_dbl);
            Sq from = to - forward_move * 2;
            *(next_move++) = NewMove(from, to, piece);
        }

        return next_move;
    }

}

#endif
