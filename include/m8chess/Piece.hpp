/// @file   Piece.hpp
/// @author Mathieu Pagé
/// @date   Feburary 2015
/// @brief  Contains the definition of the Piece and PieceType types.

#ifndef M8_PIECE_HPP_
#define M8_PIECE_HPP_

#include <string>
#include <cstdint>
#include <cassert>
#include <unordered_map>
#include <array>

#include "Color.hpp"

namespace m8
{
    ////////////////////////////////////////////////////////////////////////////
    //                               PieceType                                //
    ////////////////////////////////////////////////////////////////////////////

    /// Type that represents a piece type
    typedef std::uint8_t PieceType;

    /// @name PieceType constants
    /// Constants for piece types
    /// @{
    const PieceType kPawn = 1;
    const PieceType kKnight = 2;
    const PieceType kKing = 3;
    const PieceType kQueen = 4;
    const PieceType kBishop = 5;
    const PieceType kRook = 6;
    /// @}

    /// @name PieceType related constants
    /// Constants used in relation with PieceType.
    /// @{
    const PieceType kNoPieceType = 0;
    const PieceType kMinPieceType = 1;
    const PieceType kMaxPieceType = 6;
    /// @}

    /// Map that can be used to translate from character to piece type.
    const std::unordered_map<char, PieceType> char_to_piece_type_map =
    {
        { 'P', kPawn },
        { 'N', kKnight },
        { 'K', kKing },
        { 'Q', kQueen },
        { 'B', kBishop },
        { 'R', kRook }
    };

    /// Map that can be used to translate from character to piece type.
    const std::unordered_map<std::string, PieceType> name_to_piece_type_map =
    {
        { "pawn",   kPawn },
        { "knight", kKnight },
        { "king",   kKing },
        { "queen",  kQueen },
        { "bishop", kBishop },
        { "rook",   kRook },
    };

    const std::unordered_map<PieceType, char> piece_type_to_char_map =
    {
        { kPawn,   'P' },
        { kKnight, 'N' },
        { kKing,   'K' },
        { kQueen,  'Q' },
        { kBishop, 'B' },
        { kRook,   'R' }
    };

    /// Array containing all the piece types.
    const std::array<const PieceType, 6> all_piece_types =
    {
        kPawn,
        kKnight,
        kKing,
        kQueen,
        kBishop,
        kRook
    };

    /// Indicate if a value is a valid PieceType.
    ///
    /// @param piece_type The PieceType to evaluate.
    /// @returns True if the value is a valid PieceType
    inline bool IsPieceType(PieceType piece_type)
    {
        // The test below is only false if the 3 lsb are either all 0s or all 1s.
        // Theses are the only values that are not valid pieces.
        return ((piece_type + UINT8_C(1)) & UINT8_C(7)) > UINT8_C(1);
    }

    /// Indicate if a piece type can slide.
    /// 
    /// @param piece_type The piece type to check
    /// @returns True if the piece is a slider (Queen, Rook, Bishop).
    inline bool CanSlide(PieceType piece_type)
    {
        // A : piece_type is valid
        assert(IsPieceType(piece_type));

        return (piece_type & UINT8_C(4)) == UINT8_C(4);
    }

    /// Indicate if a piece type can slide like a rook.
    /// 
    /// @param piece_type The piece type to check
    /// @returns True if the piece slide like a rook (Queen, Rook).
    inline bool CanSlideRook(PieceType piece_type)
    {
        // A : piece_type is valid
        assert(IsPieceType(piece_type));

        return (piece_type & UINT8_C(5)) == UINT8_C(4);
    }

    /// Indicate if a piece type can slide like a bishop
    /// 
    /// @param piece_type The piece type to check
    /// @returns True if the piece slide like a bishop (Queen, Bishop).
    inline bool CanSlideBishop(PieceType piece_type)
    {
        // A : piece_type is valid
        assert(IsPieceType(piece_type));

        return (piece_type & UINT8_C(6)) == UINT8_C(4);
    }

    /// Get the PieceType from a character.
    ///
    /// @returns The piece type or kNoPieceType if the character does 
    ///          not represents a known piece type.
    inline PieceType GetPieceTypeFromChar(char c)
    {
        PieceType piece_type = kNoPieceType;

        auto it = char_to_piece_type_map.find(c);
        if (it != char_to_piece_type_map.end())
        {
            piece_type = it->second;
        }

        return piece_type;
    }

    /// Get the PieceType from a name.
    ///
    /// @returns The piece type or kNoPieceType if the name does not represents 
    ///          a known piece type.
    inline PieceType GetPieceTypeFromName(const std::string& name)
    {
        PieceType piece_type = kNoPieceType;

        auto it = name_to_piece_type_map.find(name);
        if (it != name_to_piece_type_map.end())
        {
            piece_type = it->second;
        }

        return piece_type;
    }

    /// Get a char representing a piece type.
    inline char GetCharFromPieceType(PieceType piece_type)
    {
        assert(IsPieceType(piece_type));

        return piece_type_to_char_map.at(piece_type);
    }

    ////////////////////////////////////////////////////////////////////////////
    //                               Piece                                    //
    ////////////////////////////////////////////////////////////////////////////

    /// Type that represent a piece. A piece has two attributes a PieceType and
    /// a color.
    typedef std::uint8_t Piece;

    const Piece kNoPiece = 0;
    const Piece kMinPiece = 1;
    const Piece kMaxPiece = 14;

    /// @name Piece constants
    /// Constants for piece of specific type and color
    /// @{
    const Piece kWhitePawn = 1;
    const Piece kWhiteKnight = 2;
    const Piece kWhiteKing = 3;
    const Piece kWhiteQueen = 4;
    const Piece kWhiteBishop = 5;
    const Piece kWhiteRook = 6;
    const Piece kBlackPawn = 9;
    const Piece kBlackKnight = 10;
    const Piece kBlackKing = 11;
    const Piece kBlackQueen = 12;
    const Piece kBlackBishop = 13;
    const Piece kBlackRook = 14;
    /// @}

    /// Map that can be used to translate from character to Piece.
    const std::unordered_map<char, Piece> char_to_piece_map =
    {
       { 'P', kWhitePawn},
       { 'N', kWhiteKnight },
       { 'K', kWhiteKing },
       { 'Q', kWhiteQueen },
       { 'B', kWhiteBishop },
       { 'R', kWhiteRook },
       { 'p', kBlackPawn },
       { 'n', kBlackKnight },
       { 'k', kBlackKing },
       { 'q', kBlackQueen },
       { 'b', kBlackBishop },
       { 'r', kBlackRook }
    };

    /// Map that can be used to translate from Piece to character.
    const std::unordered_map<Piece, char> piece_to_char_map =
    {
       { kWhitePawn, 'P' },
       { kWhiteKnight, 'N' },
       { kWhiteKing, 'K' },
       { kWhiteQueen, 'Q' },
       { kWhiteBishop, 'B' },
       { kWhiteRook, 'R' },
       { kBlackPawn, 'p' },
       { kBlackKnight, 'n' },
       { kBlackKing, 'k' },
       { kBlackQueen, 'q' },
       { kBlackBishop, 'b' },
       { kBlackRook, 'r' }
    };

    /// Array containing all the pieces.
    const std::array<const Piece, 12> all_pieces =
    {
        kWhitePawn,
        kWhiteKnight,
        kWhiteKing,
        kWhiteQueen,
        kWhiteBishop,
        kWhiteRook,
        kBlackPawn,
        kBlackKnight,
        kBlackKing,
        kBlackQueen,
        kBlackBishop,
        kBlackRook,
    };

   /// Indicate if a value is a valid piece.
   ///
   /// @param piece The value to evaluate
   /// @returns True if the value is a valid piece
   inline bool IsPiece(Piece piece)
   {
      return IsPieceType(piece & 7) && IsColor(piece >> 3);
   }

   /// Generate a new piece value.
   /// 
   /// @param piece_type The type of the new piece.
   /// @param color The color of the new piece.
   /// @return The new piece value
   inline Piece NewPiece(PieceType piece_type, Color color)
   {
      // A : The piece_type and color are valid.
      assert(IsPieceType(piece_type));
      assert(IsColor(color));

      return (color << 3) | piece_type;
   }

   /// Extract the piece type from a piece
   ///
   /// @param piece The piece containing the PieceType.
   /// @returns The PieceType of the Piece passed in parameters.
   inline PieceType GetPieceType(Piece piece)
   {
      return piece & 7;
   }

   /// Extract the color from a Piece
   ///
   /// @param piece The piece containing the color.
   /// @returns The color of the piece.
   inline Color GetColor(Piece piece)
   {
      // A : The piece is valid
       assert(IsPiece(piece));

      return piece >> 3;
   }

   /// Get the Piece from a character.
   ///
   /// @returns The piece or kNoPiece if the character does not represents a known piece 
   ///          type.
   inline PieceType GetPieceFromChar(char c)
   {
       Piece piece = kNoPiece;

       auto it = char_to_piece_map.find(c);
       if (it != char_to_piece_map.end())
       {
           piece = it->second;
       }

       return piece;
   }

   /// Get a char representing a piece.
   inline char GetCharFromPiece(Piece piece)
   {
       assert(IsPiece(piece));

       return piece_to_char_map.at(piece);
   }
}

#endif // M8_PIECE_HPP_
