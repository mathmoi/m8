/// @file   Piece.hpp
/// @author Mathieu Pagé
/// @date   Feburary 2015
/// @brief  Contains the definition of the Piece and PieceType types.

#ifndef M8_PIECE_HPP_
#define M8_PIECE_HPP_

#include <cstdint>
#include <cassert>

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
   const PieceType kKnigt = 2;
   const PieceType kKing = 3;
   const PieceType kQueen = 4;
   const PieceType kBishp = 5;
   const PieceType kRook = 6;
   /// @}

   /// @name PieceType related constants
   /// Constants used in relation with PieceType.
   /// @{
   const PieceType kNoPieceType = 0;
   const PieceType kMinPieceType = 1;
   const PieceType kMaxPieceType = 6;
   /// @}

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
   inline bool CanSlideBishp(PieceType piece_type)
   {
      // A : piece_type is valid
      assert(IsPieceType(piece_type));

      return (piece_type & UINT8_C(6)) == UINT8_C(4);
   }

   ////////////////////////////////////////////////////////////////////////////
   //                               Color                                    //
   ////////////////////////////////////////////////////////////////////////////

   /// Type that represent the color of the pieces
   typedef std::uint8_t Color;

   const Color kWhite = 0;
   const Color kBlack = 1;

   /// Indicate if a value is a valid color
   ///
   /// @param color The value to evaluate
   /// @return True if the value is a valid color (0 or 1)
   inline bool IsColor(Color color) { return color <= kBlack; }

   /// Return the opposite color than the one it is passed in parameter.
   ///
   /// @param color Color for wich we want the opposite.
   /// @return kWhite if kBlack was passed in parameterm otherwise kBlack.
   inline Color OpposColor(Color color)
   {
      // A : color is valid
      assert(IsColor(color));

      return color ^ UINT8_C(1); 
   }

   ////////////////////////////////////////////////////////////////////////////
   //                               Piece                                    //
   ////////////////////////////////////////////////////////////////////////////

   /// Type that represent a piece. A piece has two attributes a PieceType and
   /// a color.
   typedef std::uint8_t Piece;

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
      // A : The piece is valid
      assert(IsPiece(piece));

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
}

#endif // M8_PIECE_HPP_