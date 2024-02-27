/// @file CoordinateNotation.cpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// 
/// @date December 2022

#include <cctype>
#include <cmath>
#include <sstream>

#include "m8chess/CoordinateNotation.hpp"

namespace m8
{
    Move ParseCoordinateNotation(std::string input, const Board& board)
    {
        std::string::const_iterator first = input.begin();
        std::string::const_iterator end  = input.end();

        Colmn from_colmn = static_cast<Colmn>(*first++ - 'a');
        if (!IsColmnOnBoard(from_colmn)) throw InvalidMoveNotationException("Invalid from column");
        Row   from_row   = static_cast<Row>  (*first++ - '1');
        if (!IsRowOnBoard(from_row)) throw InvalidMoveNotationException("Invalid from row");
        Sq    from       = NewSq(from_colmn, from_row);

        Piece piece      = board[from];
        if (!IsPiece(piece)) throw InvalidMoveNotationException("No piece on from square");

        Colmn to_colmn   = static_cast<Colmn>(*first++ - 'a');
        if (!IsColmnOnBoard(to_colmn)) throw InvalidMoveNotationException("Invalid to column");
        Row   to_row     = static_cast<Row>  (*first++ - '1');
        if (!IsRowOnBoard(to_row)) throw InvalidMoveNotationException("Invalid to row");
        Sq    to         = NewSq(to_colmn, to_row);

        Piece captured   = board[to];

        Piece promote_to = kNoPiece;
        if(first < end)
        {
            PieceType promotion_type = GetPieceTypeFromChar(std::toupper(*first++));
            if (!IsPieceType(promotion_type)) throw InvalidMoveNotationException();
            promote_to = NewPiece(promotion_type, board.side_to_move());
        }

        if (first < end) throw InvalidMoveNotationException("Too many characters");

        // If the move is a pawn moving diagonally to en empty space it must be a prise 
        // en passant.
        if (GetPieceType(piece) == kPawn && std::abs(from_colmn - to_colmn) == 1 && captured == kNoPiece)
        {
            captured = NewPiece(kPawn, OpposColor(board.side_to_move()));
        }

        CastleType castle_type = kNoCastling;
        if (GetPieceType(piece) == kKing)
        {
            // If the move is a king taking it's own rook it's a castling move (chess960)
            if (GetPieceType(captured) == kRook
                && GetColor(piece) == GetColor(captured))
            {
                castle_type = to_colmn == board.casle_colmn(kKingSideCastle) ? kKingSideCastle : kQueenSideCastle;
                Colmn castle_to_colomn = castle_type == kKingSideCastle ? kColmnG : kColmnC;
                Row castle_to_row = GetRow(from);
                to = NewSq(castle_to_colomn, castle_to_row);
                captured = kNoPiece;
            }

            if (from_colmn == kColmnE)
            {
                // If the move is a king move from column e to g it's a king side castle
                if (to_colmn == kColmnG)
                {
                    castle_type = kKingSideCastle;
                }
                // If the move is a king move from column e to g it's a king side castle
                else if (to_colmn == kColmnC)
                {
                    castle_type = kQueenSideCastle;
                }
            }
        }
        
        return NewMove(from, to, piece, captured, promote_to, castle_type);
    }

    std::string RenderCoordinateNotationNormalMove(Move move)
    {
        std::ostringstream oss;

        oss << SqToString(GetFrom(move));
        oss << SqToString(GetTo(move));

        Piece promote_to = GetPromoteTo(move);
        if (IsPiece(promote_to))
        {
            PieceType piece_type = GetPieceType(promote_to);
            oss << static_cast<char>(std::tolower(GetCharFromPieceType(piece_type)));
        }

        return oss.str();
    }

    std::string RenderCoordinateNotationCastlingMove(CastleType type, Move move, const Board& board, bool chess960)
    {
        Sq from = GetFrom(move);
        Sq to;
        if (chess960)
        {
            Colmn to_colmn = board.casle_colmn(type);
            Row   to_row   = GetRow(from);
            to = NewSq(to_colmn, to_row);
        }
        else
        {
            to = GetTo(move);
        }
        
        std::ostringstream oss;

        oss << SqToString(from);
        oss << SqToString(to);

        return oss.str();
    }

    std::string RenderCoordinateNotation(Move move, const Board& board, bool chess960)
    {
        CastleType castling = GetCastling(move);
        if (castling != kNoCastling)
        {
            return RenderCoordinateNotationCastlingMove(castling, move, board, chess960);
        }

        return RenderCoordinateNotationNormalMove(move);
    }
}
