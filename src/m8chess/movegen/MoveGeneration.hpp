/// @file   MoveGeneration.hpp
/// @author Mathieu Pagé
/// @date   December 2015
/// @brief  Contains the definition of types and utilities to generate moves

#ifndef M8_CHESS_MOVEGEN_MOVE_GENERATION_HPP_
#define M8_CHESS_MOVEGEN_MOVE_GENERATION_HPP_

#include <cstdint>
#include <stdexcept>

#include "../../m8common/Bb.hpp"

#include "../Board.hpp"
#include "../Const.hpp"
#include "../Move.hpp"
#include "../Piece.hpp"
#include "../Sq.hpp"
#include "../XRay.hpp"

#include "Attacks.hpp"

namespace m8::movegen
{
    /// Generate a bitboard of all the squares that attacks a given square.
    ///
    /// @param sq Square for which we want to get the attackers.
    inline Bb AttacksTo(const Board& board, Sq sq)
    {
        Bb queens = board.bb_piece(kWhiteQueen) | board.bb_piece(kBlackQueen);
        Bb rooks = board.bb_piece(kWhiteRook) | board.bb_piece(kBlackRook);
        Bb bishops = board.bb_piece(kWhiteBishop) | board.bb_piece(kBlackBishop);
        Bb knights = board.bb_piece(kWhiteKnight) | board.bb_piece(kBlackKnight);
        Bb kings = board.bb_piece(kWhiteKing) | board.bb_piece(kBlackKing);

        Bb attackers = GenerateRookAttacks(board.bb_occupied(), sq) & (queens | rooks);
        attackers |= GenerateBishopAttacks(board.bb_occupied(), sq) & (queens | bishops);
        attackers |= knight_attack_bb[sq] & knights;
        attackers |= king_attack_bb[sq] & kings;

        Bb bb_sq = GetSingleBitBb(sq);
        attackers |= (((bb_sq << 7) & ~GetColmnBb(kColmnH)) | ((bb_sq << 9) & ~GetColmnBb(kColmnA))) & board.bb_piece(kBlackPawn);
        attackers |= (((bb_sq >> 9) & ~GetColmnBb(kColmnH)) | ((bb_sq >> 7) & ~GetColmnBb(kColmnA))) & board.bb_piece(kWhitePawn);

        return attackers;
    }

    /// Generate a bitboard of the pawns attacking a given square.
    inline Bb GeneratePawnAttacksTo(Color color, Sq sq)
    {
        Bb bb = GetSingleBitBb(sq) & ~kBbColmn[kColmnA];
        Bb result = (color == kWhite ? bb >> 9 : bb << 7);

        bb = GetSingleBitBb(sq) & ~kBbColmn[kColmnH];
        result |= (color == kWhite ? bb >> 7 : bb << 9);

        return result;
    }

    /// Generate a bitboard of all the squares containing a given piece type that 
    /// attacks a given square.
    inline Bb GenerateAttacksTo(const Board& board, Piece piece, Sq sq)
    {
        assert(IsPiece(piece));
        assert(IsSqOnBoard(sq));

        Bb result;

        switch (GetPieceType(piece))
        {
        case kRook:
            result = GenerateRookAttacks(board.bb_occupied(), sq);
            break;

        case kBishop:
            result = GenerateBishopAttacks(board.bb_occupied(), sq);
            break;

        case kQueen:
            result = GenerateRookAttacks(board.bb_occupied(), sq) |
                     GenerateBishopAttacks(board.bb_occupied(), sq);
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

        result &= board.bb_piece(piece);
        return result;
    }

    /// Returns the target squares give the color of a moving piece and wheter we are
    /// generating captures or not.
    ///
    /// @param color      Color of the piece moving.
    /// @param is_captures Indicate if we want the targets of captures or not.
    /// @return A bitboard containing the target squares.
    inline Bb GetTargets(const Board& board, Color color, bool is_captures)
    {
        Bb targets;
        if (is_captures)
            targets = board.bb_color(OpposColor(color));
        else
            targets = ~board.bb_occupied();

        return targets;
    }

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
    inline Move* GenerateSimpleMoves(const Board& board,
                                     Color color,
                                     bool is_captures,
                                     Piece piece,
                                     Bb bb_pieces,
                                     AttackArray attack_array,
                                     Move* next_move)
    {
        Bb targets = GetTargets(board, color, is_captures);

        while (bb_pieces)
        {
            Sq from = RemoveLsb(bb_pieces);
            Bb destinations = attack_array[from] & targets;

            while (destinations)
            {
                Sq to = RemoveLsb(destinations);
                *(next_move++) = NewMove(from, to, piece, board[to]);
            }
        }

        return next_move;
    }

    /// Generate the moves of the knight. The moves are added to an array specified by
    /// the parameter next_move the end of the sequence of moves is indicated by a 
    /// null move (kNullMove).
    ///
    /// @param color       Color of the knight to generate the moves for.
    /// @param is_captures Indicate if we should generate captures or non captures 
    ///                    moves.
    /// @param next_move   Pointer into an array where we can add moves.
    /// @return A pointer to the position after the last move inserted into the array.
    inline Move* GenerateKnightMoves(const Board& board, Color color, bool is_captures, Move* next_move)
    {
        Piece piece = NewPiece(kKnight, color);
        return GenerateSimpleMoves(board,
                                   color,
                                   is_captures,
                                   piece,
                                   board.bb_piece(piece),
                                   knight_attack_bb,
                                   next_move);
    }

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
    inline Move* GenerateCastlingMoves(const Board& board,
        Color color,
        std::uint8_t castling_side,
        Colmn king_final_column,
        Colmn rook_original_column,
        Colmn rook_final_column,
        Move* next_move)
    {
        if (board.casle(color, castling_side))
        {
            Piece king = NewPiece(kKing, color);
            Bb bb_king = board.bb_piece(king);
            Sq king_position = GetLsb(bb_king);
            Row row = GetRow(king_position);
            Sq king_final_position = NewSq(king_final_column, row);
            Sq rook_position = NewSq(rook_original_column, row);
            Sq rook_final_position = NewSq(rook_final_column, row);

            Bb bb_travel_king = BbBetween(king_position, king_final_position);
            Bb bb_travel_rook = BbBetween(rook_position, rook_final_position);

            // Check if any of the travel squared is occupied.
            Bb occ = board.bb_occupied();
            occ ^= GetSingleBitBb(rook_position) | bb_king;
            bool travel_occupied = (occ & (bb_travel_king | bb_travel_rook)) != kEmptyBb;

            if (!travel_occupied)
            {
                // Check if any of the square traveled by the king or the origin or 
                // destination of the king are under attack.
                Bb bb_check_attack = bb_travel_king | bb_king | GetSingleBitBb(king_final_position);
                Bb bb_opponents = board.bb_color(OpposColor(color));

                bool attacked = false;
                while (bb_check_attack && !attacked)
                {
                    Sq pos = RemoveLsb(bb_check_attack);
                    attacked = (AttacksTo(board, pos) & bb_opponents) != kEmptyBb;
                }

                if (!attacked)
                {
                    *(next_move++) = NewCastlingMove(king_position, king_final_position, king, castling_side);
                }
            }
        }

        return next_move;
    }

    /// Generate castling moves.
    ///
    /// @param color         Color of the king to generate the castling moves for.
    /// @param next_move     Pointer into an array where we can add moves.
    /// @return A pointer to the position after the last move inserted into the array.
    inline Move* GenerateCastlingMoves(const Board& board, Color color, Move* next_move)
    {
        next_move = GenerateCastlingMoves(board, color, kKingSideCastle, kColmnG, board.casle_colmn(kKingSideCastle), kColmnF, next_move);
        next_move = GenerateCastlingMoves(board, color, kQueenSideCastle, kColmnC, board.casle_colmn(kQueenSideCastle), kColmnD, next_move);

        return next_move;
    }

    /// Generate the moves of the king. The moves are added to an array specified by
    /// the parameter next_move.
    ///
    /// @param color       Color of the king to generate the moves for.
    /// @param is_captures Indicate if we should generate captures or non captures 
    ///                    moves.
    /// @param next_move   Pointer into an array where we can add moves.
    /// @return A pointer to the position after the last move inserted into the array.
    inline Move* GenerateKingMoves(const Board& board, Color color, bool is_captures, Move* next_move)
    {
        Piece piece = NewPiece(kKing, color);
        next_move = GenerateSimpleMoves(board,
                                        color,
                                        is_captures,
                                        piece,
                                        board.bb_piece(piece),
                                        king_attack_bb,
                                        next_move);

        if (!is_captures)
        {
            next_move = GenerateCastlingMoves(board, color, next_move);
        }

        return next_move;
    }

    /// Generate pawn moves from a bitboard representing destinations squares.
    ///
    /// @param color       Color of the pawn to generate the moves for.
    /// @param target      Bitboard representing the destinations.
    /// @param from_delta  Delta to apply to the destination to get the origin.
    /// @param next_move   Pointer into an array where we can add moves.
    /// @return A pointer to the position after the last move inserted into the array.
    inline Move* UnpackPawnMoves(const Board& board, Color color, Bb target, int from_delta, Move* next_move)
    {
        Piece piece = NewPiece(kPawn, color);
        Row eighth_row = 7 - 7 * color;

        while (target)
        {
            Sq to = RemoveLsb(target);
            Sq from = to + from_delta;
            if (GetRow(to) != eighth_row)
            {
                *(next_move++) = NewMove(from, to, piece, board[to]);
            }
            else
            {
                *(next_move++) = NewMove(from, to, piece, board[to], NewPiece(kQueen, color));
                *(next_move++) = NewMove(from, to, piece, board[to], NewPiece(kRook, color));
                *(next_move++) = NewMove(from, to, piece, board[to], NewPiece(kKnight, color));
                *(next_move++) = NewMove(from, to, piece, board[to], NewPiece(kBishop, color));
            }
        }

        return next_move;
    }

    /// Generate pawn side captures.
    ///
    /// @param color         Color of the pawn to generate the moves for.
    /// @param ignored_colmn Column that should be ignored.
    /// @param delta         Delta applied between the from and to squares.
    /// @param next_move     Pointer into an array where we can add moves.
    /// @return A pointer to the position after the last move inserted into the array.
    inline Move* GeneratePawnSideCaptures(const Board& board, Color color, Colmn ignored_colmn, int delta, Move* next_move)
    {
        Piece piece = NewPiece(kPawn, color);

        Bb target = board.bb_piece(piece) & ~kBbColmn[ignored_colmn];
        Shift(target, delta);
        target &= board.bb_color(OpposColor(color));
        next_move = UnpackPawnMoves(board, color, target, -delta, next_move);

        return next_move;
    }

    /// Generate the en-passant capture
    ///
    /// @param color         Color of the pawn to generate the moves for.
    /// @param next_move     Pointer into an array where we can add moves.
    /// @return A pointer to the position after the last move inserted into the array.
    inline Move* GeneratePriseEnPassant(const Board& board, Color color, Move* next_move)
    {
        Colmn enpas = board.colmn_enpas();
        if (IsColmnOnBoard(enpas))
        {
            Piece pawn = NewPiece(kPawn, color);
            Row row = GetColorWiseRow(color, kRow5);

            if (enpas > kColmnA)
            {
                Sq from = NewSq(enpas - 1, row);
                if (board[from] == pawn)
                {
                    Sq to = NewSq(enpas, GetColorWiseRow(color, kRow6));
                    Piece captured = NewPiece(kPawn, OpposColor(color));
                    *(next_move++) = NewMove(from, to, pawn, captured);
                }
            }

            if (enpas < kColmnH)
            {
                Sq from = NewSq(enpas + 1, row);
                if (board[from] == pawn)
                {
                    Sq to = NewSq(enpas, GetColorWiseRow(color, kRow6));
                    Piece captured = NewPiece(kPawn, OpposColor(color));
                    *(next_move++) = NewMove(from, to, pawn, captured);
                }
            }
        }

        return next_move;
    }

    /// Generate pawn promotions
    ///
    /// @param color         Color of the pawn to generate the moves for.
    /// @param next_move     Pointer into an array where we can add moves.
    /// @return A pointer to the position after the last move inserted into the array.
    inline Move* GeneratePawnPromotions(const Board& board, Color color, Move* next_move)
    {
        Piece piece = NewPiece(kPawn, color);
        Row seventh_row = kRow7 - 5 * color;
        int forward_move = 8 - 16 * color;

        Bb target = board.bb_piece(piece) & kBbRow[seventh_row];
        Shift(target, forward_move);
        target &= ~board.bb_occupied();
        next_move = UnpackPawnMoves(board, color, target, -forward_move, next_move);

        return next_move;
    }

    /// Generate the moves of the pawns. The moves are added to an array specified by
    /// the parameter next_move.
    ///
    /// @param color       Color of the pawn to generate the moves for.
    /// @param next_move   Pointer into an array where we can add moves.
    /// @return A pointer to the position after the last move inserted into the array.
    inline Move* GeneratePawnMoves(const Board& board, Color color, Move* next_move)
    {
        Piece piece = NewPiece(kPawn, color);
        Row third_row = kRow3 + 3 * color;
        Row seventh_row = kRow7 - 5 * color;
        int forward_move = 8 - 16 * color;

        // Generate the standard one square forward moves. We need to exclude pawns on 
        // the 7th rank that will generate promotions.
        Bb target = board.bb_piece(piece) & ~kBbRow[seventh_row];
        Shift(target, forward_move);
        target &= ~board.bb_occupied();

        // Generate the two squares moves
        Bb target_dbl = target & kBbRow[third_row];
        Shift(target_dbl, forward_move);
        target_dbl &= ~board.bb_occupied();

        next_move = UnpackPawnMoves(board, color, target, -forward_move, next_move);
        next_move = UnpackPawnMoves(board, color, target_dbl, -forward_move * 2, next_move);

        return next_move;
    }

    /// Generate the pawn's captures. The moves are added to an array specified by
    /// the parameter next_move. Promotions are considered captures and are generated
    /// by this method.
    ///
    /// @param color       Color of the pawn to generate the captures for.
    /// @param next_move   Pointer into an array where we can add moves.
    /// @return A pointer to the position after the last move inserted into the array.
    inline Move* GeneratePawnCaptures(const Board& board, Color color, Move* next_move)
    {
        int forward_left = 7 - 16 * color;
        int forward_right = 9 - 16 * color;

        next_move = GeneratePawnSideCaptures(board, color, kColmnA, forward_left, next_move);
        next_move = GeneratePawnSideCaptures(board, color, kColmnH, forward_right, next_move);
        next_move = GeneratePriseEnPassant(board, color, next_move);
        next_move = GeneratePawnPromotions(board, color, next_move);

        return next_move;
    }

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
    inline Move* GenerateSliderMove(const Board& board, Piece piece, Color color, Bb targets, bool slide_like_rook, bool slide_like_bishop, Move* next_move)
    {
        Bb bb_from = board.bb_piece(piece);

        while (bb_from)
        {
            Sq from = RemoveLsb(bb_from);
            
            Bb bb_to = kEmptyBb;
            if (slide_like_rook)
                bb_to |= GenerateRookAttacks(board.bb_occupied(), from);
            if (slide_like_bishop)
                bb_to |= GenerateBishopAttacks(board.bb_occupied(), from);
            bb_to &= targets;

            while (bb_to)
            {
                Sq to = RemoveLsb(bb_to);

                *(next_move++) = NewMove(from, to, piece, board[to]);
            }
        }

        return next_move;
    }

    /// Generate the moves of the rooks. The moves are added to an array specified by
    /// the parameter next_move.
    ///
    /// @param color       Color of the piece to generate the moves for.
    /// @param is_captures Indicate if we should generate captures or non captures 
    ///                    moves.
    /// @param next_move   Pointer into an array where we can add moves.
    /// @return A pointer to the position after the last move inserted into the array.
    inline Move* GenerateRookMoves(const Board& board, Color color, bool is_captures, Move* next_move)
    {
        Piece piece = NewPiece(kRook, color);
        Bb targets = GetTargets(board, color, is_captures);

        return GenerateSliderMove(board, piece, color, targets, true, false, next_move);
    }

    /// Generate the moves of the bishops. The moves are added to an array specified by
    /// the parameter next_move.
    ///
    /// @param color       Color of the piece to generate the moves for.
    /// @param is_captures Indicate if we should generate captures or non captures 
    ///                    moves.
    /// @param next_move   Pointer into an array where we can add moves.
    /// @return A pointer to the position after the last move inserted into the array.
    inline Move* GenerateBishopMoves(const Board& board, Color color, bool is_captures, Move* next_move)
    {
        Piece piece = NewPiece(kBishop, color);
        Bb targets = GetTargets(board, color, is_captures);

        return GenerateSliderMove(board, piece, color, targets, false, true, next_move);
    }

    /// Generate the moves of the queens. The moves are added to an array specified by
    /// the parameter next_move.
    ///
    /// @param color       Color of the piece to generate the moves for.
    /// @param is_captures Indicate if we should generate captures or non captures 
    ///                    moves.
    /// @param next_move   Pointer into an array where we can add moves.
    /// @return A pointer to the position after the last move inserted into the array.
    inline Move* GenerateQueenMoves(const Board& board, Color color, bool is_captures, Move* next_move)
    {
        Piece piece = NewPiece(kQueen, color);
        Bb targets = GetTargets(board, color, is_captures);

        return GenerateSliderMove(board, piece, color, targets, true, true, next_move);
    }

    /// Generate all captures and promotion moves.
    ///
    /// @param color     Color of the pieces to generate the moves for.
    /// @param next_move Pointer into an array where we can add moves.
    /// @return A pointer to the position afther the last move inserted into the array.
    inline Move* GenerateCaptures(const Board& board, Color color, Move* next_move)
    {
        next_move = GeneratePawnCaptures(board, color, next_move);
        next_move = GenerateKnightMoves(board, color, true, next_move);
        next_move = GenerateBishopMoves(board, color, true, next_move);
        next_move = GenerateRookMoves(board, color, true, next_move);
        next_move = GenerateQueenMoves(board, color, true, next_move);
        next_move = GenerateKingMoves(board, color, true, next_move);

        return next_move;
    }

    /// Generate all quiet moves.
    ///
    /// @param color     Color of the pieces to generate the moves for.
    /// @param next_move Pointer into an array where we can add moves.
    /// @return A pointer to the position afther the last move inserted into the array.
    inline Move* GenerateQuietMoves(const Board& board, Color color, Move* next_move)
    {
        next_move = GeneratePawnMoves(board, color, next_move);
        next_move = GenerateKnightMoves(board, color, false, next_move);
        next_move = GenerateBishopMoves(board, color, false, next_move);
        next_move = GenerateRookMoves(board, color, false, next_move);
        next_move = GenerateQueenMoves(board, color, false, next_move);
        next_move = GenerateKingMoves(board, color, false, next_move);

        return next_move;
    }

    /// Generate all moves for the current side on move.
    ///
    /// @param next_move Pointer into an array where we can add moves.
    /// @return A pointer to the position afther the last move inserted into the array.
    inline Move* GenerateAllMoves(const Board& board, Move* next_move)
    {        
        Color color = board.side_to_move();

        next_move = GenerateCaptures(board, color, next_move);
        next_move = GenerateQuietMoves(board, color, next_move);
        
        return next_move;
    }

    /// Generate all quiet moves for the current side on move.
    ///
    /// @param next_move Pointer into an array where we can add moves.
    /// @return A pointer to the position afther the last move inserted into the array.
    inline Move* GenerateAllQuietMoves(const Board& board, Move* next_move)
    {
        Color color = board.side_to_move();

        next_move = GenerateQuietMoves(board, color, next_move);

        return next_move;
    }

    /// Generate all captures for the current side on move.
    ///
    /// @param next_move Pointer into an array where we can add moves.
    /// @return A pointer to the position afther the last move inserted into the array.
    inline Move* GenerateAllCaptures(const Board& board, Move* next_move)
    {
        Color color = board.side_to_move();

        next_move = GenerateCaptures(board, color, next_move);

        return next_move;
    }

    /// Returns a bitboard of the pinned pieces of a give color.
    inline Bb GetPinnedPieces(const Board& board, Color color)
    {
        Sq king_sq = GetLsb(board.bb_piece(NewPiece(kKing, color)));
        Color attacker_color = OpposColor(color);
        Bb attacker_like_rook = board.bb_piece(NewPiece(kRook, attacker_color))
                              | board.bb_piece(NewPiece(kQueen, attacker_color));
        Bb attacker_like_bishop = board.bb_piece(NewPiece(kBishop, attacker_color))
                                | board.bb_piece(NewPiece(kQueen, attacker_color));
        Bb occ = board.bb_occupied();
        Bb blockers = board.bb_color(color);

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

#endif // M8_CHESS_MOVEGEN_MOVE_GENERATION_HPP_
