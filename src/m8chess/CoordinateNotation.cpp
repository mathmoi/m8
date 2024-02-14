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
    Move ParseCastlingMove(CastleType castling, const Board& board)
    {
        Color side_to_move = board.side_to_move();
        Piece piece = NewPiece(kKing, side_to_move);

        Sq from = GetLsb(board.bb_piece(piece));

        Colmn colmn = castling == kQueenSideCastle ? kColmnC : kColmnG;
        Row first_row = GetColorWiseRow(side_to_move, kRow1);
        Sq to = NewSq(colmn, first_row);

        return NewCastlingMove(from, to, piece, castling);
    }

    Move ParseRegularMove(std::string input, const Board& board)
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

        return NewMove(from, to, piece, captured, promote_to);
    }

    Move ParseCoordinateNotation(std::string input, const Board& board)
    {
        if (input == "O-O")
        {
            return ParseCastlingMove(kKingSideCastle, board);
        }

        if (input == "O-O-O")
        {
            return ParseCastlingMove(kQueenSideCastle, board);
        }

        return ParseRegularMove(input, board);
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

    std::string RenderCoordinateNotation(Move move)
    {
        CastleType castling = GetCastling(move);
        if (castling == kKingSideCastle)
        {
            return "O-O";
        }

        if (castling == kQueenSideCastle)
        {
            return "O-O-O";
        }

        return RenderCoordinateNotationNormalMove(move);
    }
}
