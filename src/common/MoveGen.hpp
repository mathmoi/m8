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

        /// Generate the pawn's captures. The moves are added to an array specified by
        /// the parameter next_move. Promotions are considered captures and are generated
        /// by this method.
        ///
        /// @param color       Color of the pawn to generate the captures for.
        /// @param next_move   Pointer into an array where we can add moves.
        /// @return A pointer to the position after the last move inserted into the array.
        inline Move* GeneratePawnCaptures(Color color, Move* next_move) const;

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

        /// Generate pawn moves from a bitboard representing destinations squares.
        ///
        /// @param color       Color of the pawn to generate the moves for.
        /// @param target      Bitboard representing the destinations.
        /// @param from_delta  Delta to apply to the destination to get the origin.
        /// @param next_move   Pointer into an array where we can add moves.
        /// @return A pointer to the position after the last move inserted into the array.
        inline Move* UnpackPawnMoves(Color color, Bb target, int from_delta, Move* next_move) const;

        /// Generate pawn side captures.
        ///
        /// @param color         Color of the pawn to generate the moves for.
        /// @param ignored_colmn Column that should be ignored.
        /// @param delta         Delta applied between the from and to squares.
        /// @param next_move     Pointer into an array where we can add moves.
        /// @return A pointer to the position after the last move inserted into the array.
        inline Move* GeneratePawnSideCaptures(Color color, Colmn ignored_colmn, int delta, Move* next_move) const;

        /// Generate pawn promotions
        ///
        /// @param color         Color of the pawn to generate the moves for.
        /// @param next_move     Pointer into an array where we can add moves.
        /// @return A pointer to the position after the last move inserted into the array.
        inline Move* GeneratePawnPromotions(Color color, Move* next_move) const;

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
        if (is_captures) // TODO : Test : Could this branch be replaced by some multiplication (color)?
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

    inline Move* MoveGen::UnpackPawnMoves(Color color, Bb target, int from_delta, Move* next_move) const
    {
        Piece piece = NewPiece(kPawn, color);
        Row eighth_row = 7 - 7 * color;

        while (target)
        {
            Sq to = RemoveLsb(target);
            Sq from = to + from_delta;
            if (GetRow(to) != eighth_row)
            {
                *(next_move++) = NewMove(from, to, piece, board_[to]);
            }
            else
            {
                *(next_move++) = NewMove(from, to, piece, board_[to], NewPiece(kQueen, color));
                *(next_move++) = NewMove(from, to, piece, board_[to], NewPiece(kRook, color));
                *(next_move++) = NewMove(from, to, piece, board_[to], NewPiece(kKnigt, color));
                *(next_move++) = NewMove(from, to, piece, board_[to], NewPiece(kBishp, color));
            }
        }

        return next_move;
    }

    inline Move* MoveGen::GeneratePawnSideCaptures(Color color, Colmn ignored_colmn, int delta, Move* next_move) const
    {
        Piece piece = NewPiece(kPawn, color);

        Bb target = board_.bb_piece(piece) & ~kBbColmn[ignored_colmn];
        Shift(target, delta);
        target &= board_.bb_color(OpposColor(color));
        next_move = UnpackPawnMoves(color, target, -delta, next_move);

        return next_move;
    }

    inline Move* MoveGen::GeneratePawnPromotions(Color color, Move* next_move) const 
    {
        Piece piece = NewPiece(kPawn, color);
        Row seventh_row = kRow7 - 5 * color;
        int forward_move = 8 - 16 * color;

        Bb target = board_.bb_piece(piece) & kBbRow[seventh_row];
        Shift(target, forward_move);
        target &= ~board_.bb_occupied();
        next_move = UnpackPawnMoves(color, target, -forward_move, next_move);

        return next_move;
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

        next_move = UnpackPawnMoves(color, target, -forward_move, next_move);
        next_move = UnpackPawnMoves(color, target_dbl, -forward_move * 2, next_move);

        return next_move;
    }

    inline Move* MoveGen::GeneratePawnCaptures(Color color, Move* next_move) const
    {
        int forward_left = 7 - 16 * color;
        int forward_right = 9 - 16 * color;

        next_move = GeneratePawnSideCaptures(color, kColmnA, forward_left, next_move);
        next_move = GeneratePawnSideCaptures(color, kColmnH, forward_right, next_move);
        next_move = GeneratePawnPromotions(color, next_move);

        return next_move;
    }
}

#endif
