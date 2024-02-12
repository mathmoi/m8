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
#include "../MoveList.hpp"
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

    /// Generate a bitboard of all square that a given square attacks.
    inline Bb AttacksFrom(const Board& board, Sq sq)
    {
        switch (board[sq])
        {
        case kNoPieceType:
            return kEmptyBb;

        case kWhitePawn:
            return ((GetSingleBitBb(sq) & ~GetColmnBb(kColmnA)) << 7)
                 | ((GetSingleBitBb(sq) & ~GetColmnBb(kColmnH)) << 9);

        case kBlackPawn:
            return ((GetSingleBitBb(sq) & ~GetColmnBb(kColmnA)) >> 9)
                 | ((GetSingleBitBb(sq) & ~GetColmnBb(kColmnH)) >> 7);

        case kWhiteKnight:
        case kBlackKnight:
            return knight_attack_bb[sq];

        case kWhiteKing:
        case kBlackKing:
            return king_attack_bb[sq];

        case kWhiteQueen:
        case kBlackQueen:
            return GenerateBishopAttacks(board.bb_occupied(), sq)
                 | GenerateRookAttacks(board.bb_occupied(), sq);

        case kWhiteBishop:
        case kBlackBishop:
            return GenerateBishopAttacks(board.bb_occupied(), sq);

        case kWhiteRook:
        case kBlackRook:
            return GenerateRookAttacks(board.bb_occupied(), sq);
        }

        assert(false && "Execution should never reach here");
        return kEmptyBb;
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
    /// @param move_list    List in which to add moves.
    /// @return A pointer to the position after the last move inserted into the array.
    inline void GenerateSimpleMoves(const Board& board,
                                    Color color,
                                    bool is_captures,
                                    Piece piece,
                                    Bb bb_pieces,
                                    AttackArray attack_array,
                                    MoveList& move_list)
    {
        Bb targets = GetTargets(board, color, is_captures);

        while (bb_pieces)
        {
            Sq from = RemoveLsb(bb_pieces);
            Bb destinations = attack_array[from] & targets;

            while (destinations)
            {
                Sq to = RemoveLsb(destinations);
                move_list.Push(NewMove(from, to, piece, board[to]));
            }
        }
    }

    /// Generate the moves of the knight. The moves are added to an array specified by
    /// the parameter next_move the end of the sequence of moves is indicated by a 
    /// null move (kNullMove).
    ///
    /// @param color       Color of the knight to generate the moves for.
    /// @param is_captures Indicate if we should generate captures or non captures 
    ///                    moves.
    /// @param move_list   List in which to add moves.
    /// @return A pointer to the position after the last move inserted into the array.
    inline void GenerateKnightMoves(const Board& board, 
                                     Color color,
                                     bool is_captures,
                                     MoveList& move_list)
    {
        Piece piece = NewPiece(kKnight, color);
        GenerateSimpleMoves(board,
                            color,
                            is_captures,
                            piece,
                            board.bb_piece(piece),
                            knight_attack_bb,
                            move_list);
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
    /// @param move_list            List in which to add moves.
    /// @return A pointer to the position after the last move inserted into the array.
    inline void GenerateCastlingMoves(const Board& board,
                                      Color color,
                                      std::uint8_t castling_side,
                                      Colmn king_final_column,
                                      Colmn rook_original_column,
                                      Colmn rook_final_column,
                                      MoveList& move_list)
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
                    move_list.Push(NewCastlingMove(king_position,
                                                       king_final_position,
                                                       king,
                                                       castling_side));
                }
            }
        }
    }

    /// Generate castling moves.
    ///
    /// @param color         Color of the king to generate the castling moves for.
    /// @param move_list     List in which to add moves.
    /// @return A pointer to the position after the last move inserted into the array.
    inline void GenerateCastlingMoves(const Board& board, Color color, MoveList& move_list)
    {
        GenerateCastlingMoves(board, color, kKingSideCastle, kColmnG, board.casle_colmn(kKingSideCastle), kColmnF, move_list);
        GenerateCastlingMoves(board, color, kQueenSideCastle, kColmnC, board.casle_colmn(kQueenSideCastle), kColmnD, move_list);
    }

    /// Generate the moves of the king. The moves are added to an array specified by
    /// the parameter next_move.
    ///
    /// @param color       Color of the king to generate the moves for.
    /// @param is_captures Indicate if we should generate captures or non captures 
    ///                    moves.
    /// @param move_list   List in which to add moves.
    /// @return A pointer to the position after the last move inserted into the array.
    inline void GenerateKingMoves(const Board& board, Color color, 
                                  bool is_captures, MoveList& move_list)
    {
        Piece piece = NewPiece(kKing, color);
       GenerateSimpleMoves(board,
                           color,
                           is_captures,
                           piece,
                           board.bb_piece(piece),
                           king_attack_bb,
                           move_list);

        if (!is_captures)
        {
            GenerateCastlingMoves(board, color, move_list);
        }
    }

    /// Generate pawn moves from a bitboard representing destinations squares.
    ///
    /// @param color       Color of the pawn to generate the moves for.
    /// @param target      Bitboard representing the destinations.
    /// @param from_delta  Delta to apply to the destination to get the origin.
    /// @param move_list   List in which to add moves.
    /// @return A pointer to the position after the last move inserted into the array.
    inline void UnpackPawnMoves(const Board& board, Color color, Bb target, int from_delta, MoveList& move_list)
    {
        Piece piece = NewPiece(kPawn, color);
        Row eighth_row = 7 - 7 * color;

        while (target)
        {
            Sq to = RemoveLsb(target);
            Sq from = to + from_delta;
            if (GetRow(to) != eighth_row)
            {
                move_list.Push(NewMove(from, to, piece, board[to]));
            }
            else
            {
                move_list.Push(NewMove(from, to, piece, board[to], NewPiece(kQueen, color)));
                move_list.Push(NewMove(from, to, piece, board[to], NewPiece(kRook, color)));
                move_list.Push(NewMove(from, to, piece, board[to], NewPiece(kKnight, color)));
                move_list.Push(NewMove(from, to, piece, board[to], NewPiece(kBishop, color)));
            }
        }
    }

    /// Generate pawn side captures.
    ///
    /// @param color         Color of the pawn to generate the moves for.
    /// @param ignored_colmn Column that should be ignored.
    /// @param delta         Delta applied between the from and to squares.
    /// @param move_list     List in which to add moves.
    /// @return A pointer to the position after the last move inserted into the array.
    inline void GeneratePawnSideCaptures(const Board& board,
                                         Color color,
                                         Colmn ignored_colmn,
                                         int delta,
                                         MoveList& move_list)
    {
        Piece piece = NewPiece(kPawn, color);

        Bb target = board.bb_piece(piece) & ~kBbColmn[ignored_colmn];
        Shift(target, delta);
        target &= board.bb_color(OpposColor(color));
        UnpackPawnMoves(board, color, target, -delta, move_list);
    }

    /// Generate the en-passant capture
    ///
    /// @param color     Color of the pawn to generate the moves for.
    /// @param move_list List in which to add moves.
    /// @return A pointer to the position after the last move inserted into the array.
    inline void GeneratePriseEnPassant(const Board& board, Color color, MoveList& move_list)
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
                    move_list.Push(NewMove(from, to, pawn, captured));
                }
            }

            if (enpas < kColmnH)
            {
                Sq from = NewSq(enpas + 1, row);
                if (board[from] == pawn)
                {
                    Sq to = NewSq(enpas, GetColorWiseRow(color, kRow6));
                    Piece captured = NewPiece(kPawn, OpposColor(color));
                    move_list.Push(NewMove(from, to, pawn, captured));
                }
            }
        }
    }

    /// Generate pawn promotions
    ///
    /// @param color         Color of the pawn to generate the moves for.
    /// @param move_list     List in which to add moves.
    /// @return A pointer to the position after the last move inserted into the array.
    inline void GeneratePawnPromotions(const Board& board, Color color, MoveList& move_list)
    {
        Piece piece = NewPiece(kPawn, color);
        Row seventh_row = kRow7 - 5 * color;
        int forward_move = 8 - 16 * color;

        Bb target = board.bb_piece(piece) & kBbRow[seventh_row];
        Shift(target, forward_move);
        target &= ~board.bb_occupied();
        UnpackPawnMoves(board, color, target, -forward_move, move_list);
    }

    /// Generate the moves of the pawns. The moves are added to an array specified by
    /// the parameter next_move.
    ///
    /// @param color       Color of the pawn to generate the moves for.
    /// @param move_list   List in which to add moves.
    /// @return A pointer to the position after the last move inserted into the array.
    inline void GeneratePawnMoves(const Board& board, Color color, MoveList& move_list)
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

        UnpackPawnMoves(board, color, target, -forward_move, move_list);
        UnpackPawnMoves(board, color, target_dbl, -forward_move * 2, move_list);
    }

    /// Generate the pawn's captures. The moves are added to an array specified by
    /// the parameter next_move. Promotions are considered captures and are generated
    /// by this method.
    ///
    /// @param color       Color of the pawn to generate the captures for.
    /// @param move_list   List in which to add moves.
    /// @return A pointer to the position after the last move inserted into the array.
    inline void GeneratePawnCaptures(const Board& board, Color color, MoveList& move_list)
    {
        int forward_left = 7 - 16 * color;
        int forward_right = 9 - 16 * color;

        GeneratePawnSideCaptures(board, color, kColmnA, forward_left, move_list);
        GeneratePawnSideCaptures(board, color, kColmnH, forward_right, move_list);
        GeneratePriseEnPassant(board, color, move_list);
        GeneratePawnPromotions(board, color, move_list);
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
    /// @param move_list         List in which to add moves.
    /// @return A pointer to the position after the last move inserted into the array.
    inline void GenerateSliderMove(const Board& board,
                                   Piece piece,
                                   Color color,
                                   Bb targets,
                                   bool slide_like_rook,
                                   bool slide_like_bishop,
                                   MoveList& move_list)
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

                move_list.Push(NewMove(from, to, piece, board[to]));
            }
        }
    }

    /// Generate the moves of the rooks. The moves are added to an array specified by
    /// the parameter next_move.
    ///
    /// @param color       Color of the piece to generate the moves for.
    /// @param is_captures Indicate if we should generate captures or non captures 
    ///                    moves.
    /// @param move_list   List in which to add moves.
    /// @return A pointer to the position after the last move inserted into the array.
    inline void GenerateRookMoves(const Board& board, Color color, bool is_captures, MoveList& move_list)
    {
        Piece piece = NewPiece(kRook, color);
        Bb targets = GetTargets(board, color, is_captures);

        GenerateSliderMove(board, piece, color, targets, true, false, move_list);
    }

    /// Generate the moves of the bishops. The moves are added to an array specified by
    /// the parameter next_move.
    ///
    /// @param color       Color of the piece to generate the moves for.
    /// @param is_captures Indicate if we should generate captures or non captures 
    ///                    moves.
    /// @param move_list   List in which to add moves.
    /// @return A pointer to the position after the last move inserted into the array.
    inline void GenerateBishopMoves(const Board& board, Color color, bool is_captures, MoveList& move_list)
    {
        Piece piece = NewPiece(kBishop, color);
        Bb targets = GetTargets(board, color, is_captures);

        GenerateSliderMove(board, piece, color, targets, false, true, move_list);
    }

    /// Generate the moves of the queens. The moves are added to an array specified by
    /// the parameter next_move.
    ///
    /// @param color       Color of the piece to generate the moves for.
    /// @param is_captures Indicate if we should generate captures or non captures 
    ///                    moves.
    /// @param move_list   List in which to add moves.
    /// @return A pointer to the position after the last move inserted into the array.
    inline void GenerateQueenMoves(const Board& board, Color color, bool is_captures, MoveList& move_list)
    {
        Piece piece = NewPiece(kQueen, color);
        Bb targets = GetTargets(board, color, is_captures);

        GenerateSliderMove(board, piece, color, targets, true, true, move_list);
    }

    /// Generate all captures and promotion moves.
    ///
    /// @param color     Color of the pieces to generate the moves for.
    /// @param move_list List in which to add moves.
    /// @return A pointer to the position afther the last move inserted into the array.
    inline void GenerateCaptures(const Board& board, Color color, MoveList& move_list)
    {
        GeneratePawnCaptures(board, color, move_list);
        GenerateKnightMoves (board, color, true, move_list);
        GenerateBishopMoves (board, color, true, move_list);
        GenerateRookMoves   (board, color, true, move_list);
        GenerateQueenMoves  (board, color, true, move_list);
        GenerateKingMoves   (board, color, true, move_list);
    }

    /// Generate all quiet moves.
    ///
    /// @param color     Color of the pieces to generate the moves for.
    /// @param move_list List in which to add moves.
    /// @return A pointer to the position afther the last move inserted into the array.
    inline void GenerateQuietMoves(const Board& board, Color color, MoveList& move_list)
    {
        GeneratePawnMoves(board, color, move_list);
        GenerateKnightMoves(board, color, false, move_list);
        GenerateBishopMoves(board, color, false, move_list);
        GenerateRookMoves(board, color, false, move_list);
        GenerateQueenMoves(board, color, false, move_list);
        GenerateKingMoves(board, color, false, move_list);
    }

    /// Generate all moves for the current side on move.
    ///
    /// @param move_list   List in which to add moves.
    /// @return A pointer to the position afther the last move inserted into the array.
    inline void GenerateAllMoves(const Board& board, MoveList& move_list)
    {        
        Color color = board.side_to_move();

        GenerateCaptures(board, color, move_list);
        GenerateQuietMoves(board, color, move_list);
    }

    /// Generate all quiet moves for the current side on move.
    ///
    /// @param move_list List in which to add moves.
    /// @return A pointer to the position afther the last move inserted into the array.
    inline void GenerateAllQuietMoves(const Board& board, MoveList& move_list)
    {
        Color color = board.side_to_move();

        GenerateQuietMoves(board, color, move_list);
    }

    /// Generate all captures for the current side on move.
    ///
    /// @param move_list List in which to add moves.
    /// @return A pointer to the position afther the last move inserted into the array.
    inline void GenerateAllCaptures(const Board& board, MoveList& move_list)
    {
        Color color = board.side_to_move();

        GenerateCaptures(board, color, move_list);
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
