/// @file   Move.hpp
/// @author Mathieu Pagé
/// @date   December 2015
/// @brief  Contains the definition of types and utilities used to manipulates moves.

#include <cstdint>

#include "Sq.hpp"
#include "Piece.hpp"

#ifndef M8_MOVE_HPP_
#define M8_MOVE_HPP_

namespace m8
{
    /// Type tha represent a move.
    typedef std::uint32_t Move;

    /// Move list.
    typedef std::array<Move, 256> MoveList;

    /// Null move to identify the end of lists
    const Move kNullMove = 0;

    /// @name Move constants
    /// Constants used to pack and unpack moves.
    /// @{
    const int kFromPos = 0;
    const int kToPos = 8;
    const int kCastlingPos = 14;
    const int kPiecePos = 16;
    const int kPromoteToPos = 20;
    const int kPieceTakenPos = 25;
    
    const int kFromSize = 6;
    const int kToSize = 6;
    const int kCastlingSize = 2;
    const int kPieceSize = 4;
    const int kPieceTakenSize = 4;
    const int kPromoteToSize = 4;
    /// @}

    /// Create a new move. This method pack all the informations of a move into a single
    /// uint32_t. The informations are mapped this way :
    ///  
    ///  bits     content
    ///  -------  -----------------------------------------
    ///   0 -  5  From square
    ///   8 - 13  To square
    ///  14 - 15  Castling type (1: queenside, 2: kingside)
    ///  16 - 19  Piece moved
    ///  20 - 23  Pieced promoted to if any
    ///  25 - 28  Piece taken if any
    ///
    /// @param from        From square.
    /// @param to          To square.
    /// @param piece       Moved piece.
    /// @param piece_taken Piece taken if any.
    /// @param promote_to  Piece promoted to if any.
    /// @return A move.
    inline Move NewMove(Sq from, Sq to, Piece piece, Piece piece_taken, Piece promote_to)
    {
        // A : All the parameters are valids
        assert(from.IsOnBoard());
        assert(to.IsOnBoard());
        assert(IsPiece(piece));
        assert(!piece_taken || IsPiece(piece_taken));
        assert(!promote_to || IsPiece(promote_to));

        return from.value() << kFromPos       |
               to.value()   << kToPos         |
               piece        << kPiecePos      |
               piece_taken  << kPieceTakenPos |
               promote_to   << kPromoteToPos;
    }

    /// Create a new move. This is an overload without promotion. The other NewMove
    /// documentation still applies.
    ///
    /// @param from        From square.
    /// @param to          To square.
    /// @param piece       Moved piece.
    /// @param piece_taken Piece taken if any.
    /// @return A move.
    inline Move NewMove(Sq from, Sq to, Piece piece, Piece piece_taken)
    {
        // A : All the parameters are valids
        assert(from.IsOnBoard());
        assert(to.IsOnBoard());
        assert(IsPiece(piece));
        assert(!piece_taken || IsPiece(piece_taken));

        return from.value() << kFromPos |
               to.value()   << kToPos |
               piece        << kPiecePos |
               piece_taken  << kPieceTakenPos;
    }

    /// Create a new move. This is an overload for castling moves. The other NewMove
    /// documentation still applies.
    ///
    /// @param from        From square.
    /// @param to          To square.
    /// @param piece       Moved piece.
    /// @param castling    Type of castling move.
    /// @return A move.
    inline Move NewCastlingMove(Sq from, Sq to, Piece piece, std::uint8_t castling)
    {
        // A : All the parameters are valids
        assert(from.IsOnBoard());
        assert(to.IsOnBoard());
        assert(IsPiece(piece));
        assert(castling == 1 || castling == 2);

        return from.value() << kFromPos    |
               to.value()   << kToPos      |
               piece        << kPiecePos   |
               castling     << kCastlingPos;
    }

    /// Create a new move. This is an overload without piece taken or a promotion. The 
    /// other NewMove documentation still applies.
    ///
    /// @param from        From square.
    /// @param to          To square.
    /// @param piece       Moved piece.
    /// @return A move.
    inline Move NewMove(Sq from, Sq to, Piece piece)
    {
        // A : All the parameters are valids
        assert(from.IsOnBoard());
        assert(to.IsOnBoard());
        assert(IsPiece(piece));

        return from.value() << kFromPos |
               to.value()   << kToPos   |
               piece        << kPiecePos;
    }

    /// Extract the from square from a move.
    ///
    /// @param move The move to extract from.
    /// @return A square.
    inline Sq GetFrom(Move move)
    {
        return (move >> kFromPos) & ((1 << kFromSize) - 1);
    }

    /// Extract the to square from a move.
    ///
    /// @param move The move to extract from.
    /// @return A square.
    inline Sq GetTo(Move move)
    {
        return (move >> kToPos) & ((1 << kToSize) - 1);
    }

    /// Extract the piece from a move.
    ///
    /// @param move The move to extract from.
    /// @return A piece
    inline Piece GetPiece(Move move)
    {
        return (move >> kPiecePos) & ((1 << kPieceSize) - 1);
    }

    /// Extract the piece taken from a move.
    ///
    /// @param move The move to extract from.
    /// @return A piece or kNoPiece.
    inline Piece GetPieceTaken(Move move)
    {
        return (move >> kPieceTakenPos) & ((1 << kPieceTakenSize) - 1);
    }

    /// Extract the piece promoted to from a move.
    ///
    /// @param move The move to extract from.
    /// @return A piece or kNoPiece.
    inline Piece GetPromoteTo(Move move)
    {
        return (move >> kPromoteToPos) & ((1 << kPromoteToSize) - 1);
    }

    /// Extract the piece castling type from a move.
    ///
    /// @param move The move to extract from.
    /// @return the castling type.
    inline std::uint8_t GetCastling(Move move)
    {
        return (move >> kCastlingPos) & ((1 << kCastlingSize) - 1);
    }
}

#endif