/// @file   Sq.hpp
/// @author Mathieu Pagé
/// @date   Feburary 2015
/// @brief  Contains the definition of types and utilities used to manipulates 
///         chess board coordinates.

#ifndef M8_SQ_HPP_
#define M8_SQ_HPP_

#include <cstdint>

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
   /// @}

   /// Indicate if a given column is on the board.
   ///
   /// @param colmn The column to test.
   /// @return True if the column is on the board.
   inline bool IsColmnOnBoard(Colmn colmn) { return colmn < kNumColmnOnBoard; };

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
   inline bool IsRowOnBoard(Row row) { return row < kNumRowOnBoard; };

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
   const Sq kA2 = 1;
   const Sq kA3 = 2;
   const Sq kA4 = 3;
   const Sq kA5 = 4;
   const Sq kA6 = 5;
   const Sq kA7 = 6;
   const Sq kA8 = 7;

   const Sq kB1 = 8;
   const Sq kB2 = 9;
   const Sq kB3 = 10;
   const Sq kB4 = 11;
   const Sq kB5 = 12;
   const Sq kB6 = 13;
   const Sq kB7 = 14;
   const Sq kB8 = 15;

   const Sq kC1 = 16;
   const Sq kC2 = 17;
   const Sq kC3 = 18;
   const Sq kC4 = 19;
   const Sq kC5 = 20;
   const Sq kC6 = 21;
   const Sq kC7 = 22;
   const Sq kC8 = 23;

   const Sq kD1 = 24;
   const Sq kD2 = 25;
   const Sq kD3 = 26;
   const Sq kD4 = 27;
   const Sq kD5 = 28;
   const Sq kD6 = 29;
   const Sq kD7 = 30;
   const Sq kD8 = 31;

   const Sq kE1 = 32;
   const Sq kE2 = 33;
   const Sq kE3 = 34;
   const Sq kE4 = 35;
   const Sq kE5 = 36;
   const Sq kE6 = 37;
   const Sq kE7 = 38;
   const Sq kE8 = 39;

   const Sq kF1 = 40;
   const Sq kF2 = 41;
   const Sq kF3 = 42;
   const Sq kF4 = 43;
   const Sq kF5 = 44;
   const Sq kF6 = 45;
   const Sq kF7 = 46;
   const Sq kF8 = 47;

   const Sq kG1 = 48;
   const Sq kG2 = 49;
   const Sq kG3 = 50;
   const Sq kG4 = 51;
   const Sq kG5 = 52;
   const Sq kG6 = 53;
   const Sq kG7 = 54;
   const Sq kG8 = 55;

   const Sq kH1 = 56;
   const Sq kH2 = 57;
   const Sq kH3 = 58;
   const Sq kH4 = 59;
   const Sq kH5 = 60;
   const Sq kH6 = 61;
   const Sq kH7 = 62;
   const Sq kH8 = 63;
   /// @}

   /// Indicate if a given square is on the board.
   ///
   /// @param sq The square to test.
   /// @return True if the square is on the board.
   inline bool IsSqOnBoard(Sq sq) { return sq < kNumSqOnBoard; };
}

#endif // M8_SQ_HPP_