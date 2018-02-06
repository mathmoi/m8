/// @file   MoveGen.hpp
/// @author Mathieu Pag�
/// @date   December 2015
/// @brief  Contains the definition of types and utilities to generate moves

#include <cstdint>
#include <stdexcept>

#include "../m8common/m8common.hpp"
#include "Const.hpp"
#include "Sq.hpp"
#include "Piece.hpp"
#include "Move.hpp"
#include "Board.hpp"
#include "Attacks.hpp"
#include "XRay.hpp"

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

        /// Generate a bitboard of all the squares containing a given piece type that 
        /// attacks a given square.
        inline Bb GenerateAttacksTo(Piece piece, Sq sq) const;

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

        /// Generate the moves of the rooks. The moves are added to an array specified by
        /// the parameter next_move.
        ///
        /// @param color       Color of the piece to generate the moves for.
        /// @param is_captures Indicate if we should generate captures or non captures 
        ///                    moves.
        /// @param next_move   Pointer into an array where we can add moves.
        /// @return A pointer to the position after the last move inserted into the array.
        inline Move* GenerateRookMoves(Color color, bool is_captures, Move* next_move) const;

        /// Generate the moves of the bishops. The moves are added to an array specified by
        /// the parameter next_move.
        ///
        /// @param color       Color of the piece to generate the moves for.
        /// @param is_captures Indicate if we should generate captures or non captures 
        ///                    moves.
        /// @param next_move   Pointer into an array where we can add moves.
        /// @return A pointer to the position after the last move inserted into the array.
        inline Move* GenerateBishopMoves(Color color, bool is_captures, Move* next_move) const;

        /// Generate the moves of the queens. The moves are added to an array specified by
        /// the parameter next_move.
        ///
        /// @param color       Color of the piece to generate the moves for.
        /// @param is_captures Indicate if we should generate captures or non captures 
        ///                    moves.
        /// @param next_move   Pointer into an array where we can add moves.
        /// @return A pointer to the position after the last move inserted into the array.
        inline Move* GenerateQueenMoves(Color color, bool is_captures, Move* next_move) const;

        /// Generate all captures and promotion moves.
        ///
        /// @param color     Color of the pieces to generate the moves for.
        /// @param next_move Pointer into an array where we can add moves.
        /// @return A pointer to the position afther the last move inserted into the array.
        inline Move* GenerateCaptures(Color color, Move* next_move) const;

        /// Generate all quiet moves.
        ///
        /// @param color     Color of the pieces to generate the moves for.
        /// @param next_move Pointer into an array where we can add moves.
        /// @return A pointer to the position afther the last move inserted into the array.
        inline Move* GenerateQuietMoves(Color color, Move* next_move) const;

        /// Generate all moves for the current side on move.
        ///
        /// @param next_move Pointer into an array where we can add moves.
        /// @return A pointer to the position afther the last move inserted into the array.
        inline Move* GenerateAllMoves(Move* next_move) const;

        /// Generate a bitboard of all the squares that attacks a given square.
        ///
        /// @param sq Square for which we want to get the attackers.
        inline Bb AttacksTo(Sq sq) const;
        
        /// Returns a bitboard of the pinned pieces of a give color.
        inline Bb GetPinnedPieces(Color color) const;

    private:

        /// Generate a bitboard of the square attacked by a rook on a given square given a
        /// specific occupation of the board.
        ///
        /// @param from Position of the rook.
        /// @param occupation Occupation of the board.
        /// @return A bitboard indicating the squares attacked by the rook.
        static Bb GenerateRookAttackForOccupancy(Sq from, Bb occupation);
        
        /// Generate a bitboard of the square attacked by a bishop on a given square given
        /// a specific occupation of the board.
        ///
        /// @param from Position of the bishop.
        /// @param occupation Occupation of the board.
        /// @return A bitboard indicating the squares attacked by the bishop.
        static Bb GenerateBishopAttackForOccupancy(Sq from, Bb occupation);

        /// Generate a bitboard of the pawns attacking a given square.
        inline static Bb GeneratePawnAttacksTo(Color color, Sq sq);

        /// Returns the target squares give the color of a moving piece and wheter we are
        /// generating captures or not.
        ///
        /// @param color      Color of the piece moving.
        /// @param is_captures Indicate if we want the targets of captures or not.
        /// @return A bitboard containing the target squares.
        inline Bb GetTargets(Color color, bool is_capture) const;

        /// Generate moves of slider pieces.
        ///
        /// @param piece             Piece for which we generate moves
        /// @param color             Color for of the moving piece
        /// @param targets           Targets squares that should be considered. Allows to 
        ///                          generate all moves, just the captures or just the
        ///                          non-captures.
        /// @param slide_like_rook   Indicate if the piece can move horizontaly and 
        ///                          verticaly.
        /// @param slide_like_bishop Indicate if the piece can move diagonaly.
        /// @param next_move         Pointer into an array where we can add moves.
        /// @return A pointer to the position after the last move inserted into the array.
        inline Move* GenerateSliderMove(Piece piece, Color color, Bb targets, bool slide_like_rook, bool slide_like_bishop, Move* next_move) const;

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

        /// Generate the en-passant capture
        ///
        /// @param color         Color of the pawn to generate the moves for.
        /// @param next_move     Pointer into an array where we can add moves.
        /// @return A pointer to the position after the last move inserted into the array.
        inline Move* GeneratePriseEnPassant(Color color, Move* next_move) const;

        /// Generate pawn promotions
        ///
        /// @param color         Color of the pawn to generate the moves for.
        /// @param next_move     Pointer into an array where we can add moves.
        /// @return A pointer to the position after the last move inserted into the array.
        inline Move* GeneratePawnPromotions(Color color, Move* next_move) const;

        /// Generate castling moves.
        ///
        /// @param color         Color of the king to generate the castling moves for.
        /// @param next_move     Pointer into an array where we can add moves.
        /// @return A pointer to the position after the last move inserted into the array.
        inline Move* GenerateCastlingMoves(Color color, Move* next_move) const;

        /// Generate a castling move, king side or queen side depending on the parameters
        /// passed.
        ///
        /// @param color                Color of the side to generate castling for. 
        /// @param castling_side        Castling side. kKingSideCastle ou kQueenSideCastle.
        /// @param king_final_column    Column of the king afther the castling
        /// @param rook_original_column Column of the rook before the castling. Because of
        ///                             the Chess960 rules this might be any column.
        /// @param rook_final_column    Column of the rook afther the castling move.
        /// @param next_move            Pointer into an array where we can add moves.
        /// @return A pointer to the position after the last move inserted into the array.
        inline Move* GenerateCastlingMoves(Color color,
                                           std::uint8_t castling_side,
                                           Colmn king_final_column,
                                           Colmn rook_original_column,
                                           Colmn rook_final_column,
                                           Move* next_move) const;
        
        const Board& board_;
    };

    inline Bb MoveGen::AttacksTo(Sq sq) const
    {
        Bb queens = board_.bb_piece(kWhiteQueen) | board_.bb_piece(kBlackQueen);
        Bb rooks = board_.bb_piece(kWhiteRook) | board_.bb_piece(kBlackRook);
        Bb bishops = board_.bb_piece(kWhiteBishop) | board_.bb_piece(kBlackBishop);
        Bb knights = board_.bb_piece(kWhiteKnight) | board_.bb_piece(kBlackKnight);
        Bb kings = board_.bb_piece(kWhiteKing) | board_.bb_piece(kBlackKing);

        Bb attackers = GenerateRookAttacks(board_.bb_occupied(), sq) & (queens | rooks);
        attackers |= GenerateBishopAttacks(board_.bb_occupied(), sq) & (queens | bishops);
        attackers |= knight_attack_bb[sq] & knights;
        attackers |= king_attack_bb[sq] & kings;

        Bb bb_sq = GetSingleBitBb(sq);
        attackers |= (((bb_sq << 7) & ~GetColmnBb(kColmnH)) | ((bb_sq << 9) & ~GetColmnBb(kColmnA))) & board_.bb_piece(kBlackPawn);
        attackers |= (((bb_sq >> 9) & ~GetColmnBb(kColmnH)) | ((bb_sq >> 7) & ~GetColmnBb(kColmnA))) & board_.bb_piece(kWhitePawn);

        return attackers;
    }

    inline Bb MoveGen::GeneratePawnAttacksTo(Color color, Sq sq)
    {
        Bb bb = GetSingleBitBb(sq) & ~kBbColmn[kColmnA];
        Bb result = (color == kWhite ? bb >> 9 : bb << 7);

        bb = GetSingleBitBb(sq) & ~kBbColmn[kColmnH];
        result |= (color == kWhite ? bb >> 7 : bb << 9);

        return result;
    }

    inline Bb MoveGen::GenerateAttacksTo(Piece piece, Sq sq) const
    {
        assert(IsPiece(piece));
        assert(IsSqOnBoard(sq));

        Bb result;

        switch (GetPieceType(piece))
        {
        case kRook:
            result = GenerateRookAttacks(board_.bb_occupied(), sq);
            break;

        case kBishop:
            result = GenerateBishopAttacks(board_.bb_occupied(), sq);
            break;

        case kQueen:
            result = GenerateRookAttacks(board_.bb_occupied(), sq) |
                     GenerateBishopAttacks(board_.bb_occupied(), sq);
            break;

        case kKnight:
            result = knight_attack_bb[sq];
            break;

        case kKing:
            result = king_attack_bb[sq];
            break;

        case kPawn:
            result = GeneratePawnAttacksTo(GetColor(piece), sq);
            break;

        default:
            throw std::out_of_range("Invalid PieceType");
        }

        result &= board_.bb_piece(piece);
        return result;
    }

    inline Bb MoveGen::GetTargets(Color color, bool is_captures) const
    {
        Bb targets;
        if (is_captures)
            targets = board_.bb_color(OpposColor(color));
        else
            targets = ~board_.bb_occupied();

        return targets;
    }

    inline Move* MoveGen::GenerateSimpleMoves(Color color,
                                              bool is_captures,
                                              Piece piece,
                                              Bb bb_pieces,
                                              AttackArray attack_array,
                                              Move* next_move) const
    {
        Bb targets = GetTargets(color, is_captures);

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
        Piece piece = NewPiece(kKnight, color);
        return GenerateSimpleMoves(color,
                                   is_captures,
                                   piece,
                                   board_.bb_piece(piece),
                                   knight_attack_bb,
                                   next_move);
    }

    inline Move* MoveGen::GenerateKingMoves(Color color, bool is_captures, Move* next_move) const
    {
        Piece piece = NewPiece(kKing, color);
        next_move = GenerateSimpleMoves(color,
                                        is_captures,
                                        piece,
                                        board_.bb_piece(piece),
                                        king_attack_bb,
                                        next_move);

        if (!is_captures)
        {
            next_move = GenerateCastlingMoves(color, next_move);
        }

        return next_move;
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
                *(next_move++) = NewMove(from, to, piece, board_[to], NewPiece(kKnight, color));
                *(next_move++) = NewMove(from, to, piece, board_[to], NewPiece(kBishop, color));
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

    inline Move* MoveGen::GeneratePriseEnPassant(Color color, Move* next_move) const
    {
        Colmn enpas = board_.colmn_enpas();
        if (IsColmnOnBoard(enpas))
        {
            Piece pawn = NewPiece(kPawn, color);
            Row row = GetColorWiseRow(color, kRow5);

            if (enpas > kColmnA)
            {
                Sq from = NewSq(enpas - 1, row);
                if (board_[from] == pawn)
                {
                    Sq to = NewSq(enpas, GetColorWiseRow(color, kRow6));
                    Piece captured = NewPiece(kPawn, OpposColor(color));
                    *(next_move++) = NewMove(from, to, pawn, captured);
                }
            }

            if (enpas < kColmnH)
            {
                Sq from = NewSq(enpas + 1, row);
                if (board_[from] == pawn)
                {
                    Sq to = NewSq(enpas, GetColorWiseRow(color, kRow6));
                    Piece captured = NewPiece(kPawn, OpposColor(color));
                    *(next_move++) = NewMove(from, to, pawn, captured);
                }
            }
        }

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

    inline Move* MoveGen::GenerateCastlingMoves(Color color,
                                                std::uint8_t castling_side,
                                                Colmn king_final_column,
                                                Colmn rook_original_column,
                                                Colmn rook_final_column,
                                                Move* next_move) const
    {
        if (board_.casle(color, castling_side))
        {
            Piece king = NewPiece(kKing, color);
            Bb bb_king = board_.bb_piece(king);
            Sq king_position = GetLsb(bb_king);
            Row row = GetRow(king_position);
            Sq king_final_position = NewSq(king_final_column, row);
            Sq rook_position = NewSq(rook_original_column, row);
            Sq rook_final_position = NewSq(rook_final_column, row);

            Bb bb_travel_king = BbBetween(king_position, king_final_position);
            Bb bb_travel_rook = BbBetween(rook_position, rook_final_position);

            // Check if any of the travel squared is occupied.
            Bb occ = board_.bb_occupied();
            occ ^= GetSingleBitBb(rook_position) | bb_king;
            bool travel_occupied = (occ & (bb_travel_king | bb_travel_rook)) != kEmptyBb;

            if (!travel_occupied)
            {
                // Check if any of the square traveled by the king or the origin or 
                // destination of the king are under attack.
                Bb bb_check_attack = bb_travel_king | bb_king | GetSingleBitBb(king_final_position);
                Bb bb_opponents = board_.bb_color(OpposColor(color));

                bool attacked = false;
                while (bb_check_attack && !attacked)
                {
                    Sq pos = RemoveLsb(bb_check_attack);
                    attacked = (AttacksTo(pos) & bb_opponents) != kEmptyBb;
                }

                if (!attacked)
                {
                    *(next_move++) = NewCastlingMove(king_position, king_final_position, king, castling_side);
                }
            }
        }

        return next_move;
    }

    inline Move* MoveGen::GenerateCastlingMoves(Color color, Move* next_move) const
    {
        next_move = GenerateCastlingMoves(color, kKingSideCastle, kColmnG, board_.casle_colmn(kKingSideCastle), kColmnF, next_move);
        next_move = GenerateCastlingMoves(color, kQueenSideCastle, kColmnC, board_.casle_colmn(kQueenSideCastle), kColmnD, next_move);

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
        next_move = GeneratePriseEnPassant(color, next_move);
        next_move = GeneratePawnPromotions(color, next_move);

        return next_move;
    }

    inline Move* MoveGen::GenerateSliderMove(Piece piece, Color color, Bb targets, bool slide_like_rook, bool slide_like_bishop, Move* next_move) const
    {
        Bb bb_from = board_.bb_piece(piece);

        while (bb_from)
        {
            Sq from = RemoveLsb(bb_from);
            
            Bb bb_to = kEmptyBb;
            if (slide_like_rook)
                bb_to |= GenerateRookAttacks(board_.bb_occupied(), from);
            if (slide_like_bishop)
                bb_to |= GenerateBishopAttacks(board_.bb_occupied(), from);
            bb_to &= targets;

            while (bb_to)
            {
                Sq to = RemoveLsb(bb_to);

                *(next_move++) = NewMove(from, to, piece, board_[to]);
            }
        }

        return next_move;
    }

    inline Move* MoveGen::GenerateRookMoves(Color color, bool is_captures, Move* next_move) const
    {
        Piece piece = NewPiece(kRook, color);
        Bb targets = GetTargets(color, is_captures);

        return GenerateSliderMove(piece, color, targets, true, false, next_move);
    }

    inline Move* MoveGen::GenerateBishopMoves(Color color, bool is_captures, Move* next_move) const
    {
        Piece piece = NewPiece(kBishop, color);
        Bb targets = GetTargets(color, is_captures);

        return GenerateSliderMove(piece, color, targets, false, true, next_move);
    }

    inline Move* MoveGen::GenerateQueenMoves(Color color, bool is_captures, Move* next_move) const
    {
        Piece piece = NewPiece(kQueen, color);
        Bb targets = GetTargets(color, is_captures);

        return GenerateSliderMove(piece, color, targets, true, true, next_move);
    }

    inline Move* MoveGen::GenerateCaptures(Color color, Move* next_move) const
    {
        next_move = GeneratePawnCaptures(color, next_move);
        next_move = GenerateKnightMoves(color, true, next_move);
        next_move = GenerateBishopMoves(color, true, next_move);
        next_move = GenerateRookMoves(color, true, next_move);
        next_move = GenerateQueenMoves(color, true, next_move);
        next_move = GenerateKingMoves(color, true, next_move);

        return next_move;
    }

    inline Move* MoveGen::GenerateQuietMoves(Color color, Move* next_move) const
    {
        next_move = GeneratePawnMoves(color, next_move);
        next_move = GenerateKnightMoves(color, false, next_move);
        next_move = GenerateBishopMoves(color, false, next_move);
        next_move = GenerateRookMoves(color, false, next_move);
        next_move = GenerateQueenMoves(color, false, next_move);
        next_move = GenerateKingMoves(color, false, next_move);

        return next_move;
    }

    inline Move* MoveGen::GenerateAllMoves(Move* next_move) const
    {        
        Color color = board_.side_to_move();

        next_move = GenerateCaptures(color, next_move);
        next_move = GenerateQuietMoves(color, next_move);
        
        return next_move;
    }

    inline Bb MoveGen::GetPinnedPieces(Color color) const
    {
        Sq king_sq = GetLsb(board_.bb_piece(NewPiece(kKing, color)));
        Color attacker_color = OpposColor(color);
        Bb attacker_like_rook = board_.bb_piece(NewPiece(kRook, attacker_color))
                              | board_.bb_piece(NewPiece(kQueen, attacker_color));
        Bb attacker_like_bishop = board_.bb_piece(NewPiece(kBishop, attacker_color))
                                | board_.bb_piece(NewPiece(kQueen, attacker_color));
        Bb occ = board_.bb_occupied();
        Bb blockers = board_.bb_color(color);

        Bb pinners = (GenerateRookXRay(occ, blockers, king_sq) & attacker_like_rook)
                   | (GenerateBishopXRay(occ, blockers, king_sq) & attacker_like_bishop);

        Bb pinned = kEmptyBb;
        while (pinners)
        {
            Sq pinner_sq = RemoveLsb(pinners);

            pinned |= BbBetween(king_sq, pinner_sq) & blockers;
        }

        return pinned;
    }
}

#endif