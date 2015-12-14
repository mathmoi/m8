/// @file   Sq.hpp
/// @author Mathieu Pag�
/// @date   Feburary 2015
/// @brief  Contains the definition of types and utilities used to manipulates 
///         chess board coordinates.

#ifndef M8_SQ_HPP_
#define M8_SQ_HPP_

#include <cstdint>
#include <cassert>

namespace m8
{
   ////////////////////////////////////////////////////////////////////////////
   //                                Columns                                 //
   ////////////////////////////////////////////////////////////////////////////

   /// Type that represent a Column on a chess board.
   typedef std::uint8_t Colmn;

   /// Number of columns on a chess board
   const std::uint8_t kNumColmnOnBoard = 8;

   /// @name Columns constants
   /// Constants for the columns of the chessboard
   /// @{
   const Colmn kColmnA = 0;
   const Colmn kColmnB = 1;
   const Colmn kColmnC = 2;
   const Colmn kColmnD = 3;
   const Colmn kColmnE = 4;
   const Colmn kColmnF = 5;
   const Colmn kColmnG = 6;
   const Colmn kColmnH = 7;
   const Colmn kInvalColmn = 255;
   /// @}

   /// Indicate if a given column is on the board.
   ///
   /// @param colmn The column to test.
   /// @return True if the column is on the board.
   inline bool IsColmnOnBoard(Colmn colmn) { return colmn < kNumColmnOnBoard; }

   ////////////////////////////////////////////////////////////////////////////
   //                                  Rows                                  //
   ////////////////////////////////////////////////////////////////////////////
   
   /// Type that represent a row on a chess board.
   typedef std::uint8_t Row;

   /// Number of rows on a chess board
   const std::uint8_t kNumRowOnBoard = 8;

   /// @name Row constants
   /// Constants for the row of the chessboard
   /// @{
   const Row kRow1 = 0;
   const Row kRow2 = 1;
   const Row kRow3 = 2;
   const Row kRow4 = 3;
   const Row kRow5 = 4;
   const Row kRow6 = 5;
   const Row kRow7 = 6;
   const Row kRow8 = 7;
   /// @}

   /// Indicate if a given row is on the board.
   ///
   /// @param row The row to test.
   /// @return True if the row is on the board.
   inline bool IsRowOnBoard(Row row) { return row < kNumRowOnBoard; }

   ////////////////////////////////////////////////////////////////////////////
   //                               Square                                   //
   ////////////////////////////////////////////////////////////////////////////

   /// Type that represents a square of the chess board.
   typedef std::uint8_t Sq;

   // Number of squares on a chess board
   const std::uint8_t kNumSqOnBoard = 64;

   /// @name Square constants
   /// Constants for the row of the chessboard
   /// @{
   const Sq kA1 = 0;
   const Sq kB1 = 1;
   const Sq kC1 = 2;
   const Sq kD1 = 3;
   const Sq kE1 = 4;
   const Sq kF1 = 5;
   const Sq kG1 = 6;
   const Sq kH1 = 7;

   const Sq kA2 = 8;
   const Sq kB2 = 9;
   const Sq kC2 = 10;
   const Sq kD2 = 11;
   const Sq kE2 = 12;
   const Sq kF2 = 13;
   const Sq kG2 = 14;
   const Sq kH2 = 15;

   const Sq kA3 = 16;
   const Sq kB3 = 17;
   const Sq kC3 = 18;
   const Sq kD3 = 19;
   const Sq kE3 = 20;
   const Sq kF3 = 21;
   const Sq kG3 = 22;
   const Sq kH3 = 23;

   const Sq kA4 = 24;
   const Sq kB4 = 25;
   const Sq kC4 = 26;
   const Sq kD4 = 27;
   const Sq kE4 = 28;
   const Sq kF4 = 29;
   const Sq kG4 = 30;
   const Sq kH4 = 31;

   const Sq kA5 = 32;
   const Sq kB5 = 33;
   const Sq kC5 = 34;
   const Sq kD5 = 35;
   const Sq kE5 = 36;
   const Sq kF5 = 37;
   const Sq kG5 = 38;
   const Sq kH5 = 39;

   const Sq kA6 = 40;
   const Sq kB6 = 41;
   const Sq kC6 = 42;
   const Sq kD6 = 43;
   const Sq kE6 = 44;
   const Sq kF6 = 45;
   const Sq kG6 = 46;
   const Sq kH6 = 47;

   const Sq kA7 = 48;
   const Sq kB7 = 49;
   const Sq kC7 = 50;
   const Sq kD7 = 51;
   const Sq kE7 = 52;
   const Sq kF7 = 53;
   const Sq kG7 = 54;
   const Sq kH7 = 55;

   const Sq kA8 = 56;
   const Sq kB8 = 57;
   const Sq kC8 = 58;
   const Sq kD8 = 59;
   const Sq kE8 = 60;
   const Sq kF8 = 61;
   const Sq kG8 = 62;
   const Sq kH8 = 63;
   /// @}

   /// Indicate if a given square is on the board.
   ///
   /// @param sq The square to test.
   /// @return True if the square is on the board.
   inline bool IsSqOnBoard(Sq sq) { return sq < kNumSqOnBoard; }

   /// Create a square from a colmn and row.
   ///
   /// @param colmn Column of the square.
   /// @param row Row of the square.
   /// @return A square.
   inline Sq NewSq(Colmn colmn, Row row)
   { 
      // A : The column and row are valid
      assert(IsColmnOnBoard(colmn));
      assert(IsRowOnBoard(row));

      return (row << 3) + colmn;
   };

   /// Get the column of a square.
   ///
   /// @param sq The square.
   /// @return The column of the square.
   inline Colmn GetColmn(Sq sq)
   {
      // A : The square is valid
      assert(IsSqOnBoard(sq));

      return sq & 7; 
   }

   /// Get the row of a square.
   ///
   /// @param sq The square.
   /// @return The row of the square.
   inline Row GetRow(Sq sq)
   {
      // A : The square is valid
      assert(IsSqOnBoard(sq));

      return sq >> 3;
   }
}

#endif // M8_SQ_HPP_
