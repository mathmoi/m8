/// @file   Piece_tests.cpp
/// @author Mathieu Pagé
/// @date   Feburary 2015
/// @brief  Contains tests of the types and functionalities in Piece.hpp.

#include "catch.hpp"
#include <cassert>

#include "../m8chess/Piece.hpp"

using namespace m8;

TEST_CASE("Test IsPieceType")
{
   REQUIRE(IsPieceType(kPawn) == true);
   REQUIRE(IsPieceType(kKnight) == true);
   REQUIRE(IsPieceType(kKing) == true);
   REQUIRE(IsPieceType(kQueen) == true);
   REQUIRE(IsPieceType(kBishop) == true);
   REQUIRE(IsPieceType(kRook) == true);

   REQUIRE(IsPieceType(kNoPieceType) == false);
   REQUIRE(IsPieceType(7) == false);
   REQUIRE(IsPieceType(8) == false);
   REQUIRE(IsPieceType(200) == false);
}

TEST_CASE("Test CanSlide")
{
   REQUIRE(CanSlide(kQueen) == true);
   REQUIRE(CanSlide(kRook) == true);
   REQUIRE(CanSlide(kBishop) == true);

   REQUIRE(CanSlide(kPawn) == false);
   REQUIRE(CanSlide(kKnight) == false);
   REQUIRE(CanSlide(kKing) == false);
}

TEST_CASE("Test CanSlideRook")
{
   REQUIRE(CanSlideRook(kQueen) == true);
   REQUIRE(CanSlideRook(kRook) == true);

   REQUIRE(CanSlideRook(kBishop) == false);
   REQUIRE(CanSlideRook(kPawn) == false);
   REQUIRE(CanSlideRook(kKnight) == false);
   REQUIRE(CanSlideRook(kKing) == false);
}

TEST_CASE("Test CanSlideBishop")
{
   REQUIRE(CanSlideBishop(kQueen) == true);
   REQUIRE(CanSlideBishop(kBishop) == true);

   REQUIRE(CanSlideBishop(kRook) == false);
   REQUIRE(CanSlideBishop(kPawn) == false);
   REQUIRE(CanSlideBishop(kKnight) == false);
   REQUIRE(CanSlideBishop(kKing) == false);
}

TEST_CASE("Test IsColor")
{
   REQUIRE(Color::White().IsColor() == true);
   REQUIRE(Color::Black().IsColor() == true);

   for (uint32_t i = Color::Black().value() + 1; i <= 255; ++i)
   {
      REQUIRE(Color(i).IsColor() == false);
   }
}

TEST_CASE("Test OpposColor")
{
   REQUIRE(Color::White().opposite() == Color::Black());
   REQUIRE(Color::Black().opposite() == Color::White());
}

TEST_CASE("Test IsPiece")
{
   // The valid values for piece are 1 to 6 for white pieces and 9 to 14 for 
   // black pieces.
   REQUIRE(IsPiece(0) == false);
   REQUIRE(IsPiece(1) == true);
   REQUIRE(IsPiece(2) == true);
   REQUIRE(IsPiece(3) == true);
   REQUIRE(IsPiece(4) == true);
   REQUIRE(IsPiece(5) == true);
   REQUIRE(IsPiece(6) == true);
   REQUIRE(IsPiece(7) == false);
   REQUIRE(IsPiece(8) == false);
   REQUIRE(IsPiece(9) == true);
   REQUIRE(IsPiece(10) == true);
   REQUIRE(IsPiece(11) == true);
   REQUIRE(IsPiece(12) == true);
   REQUIRE(IsPiece(13) == true);
   REQUIRE(IsPiece(14) == true);

   for (uint32_t i = 15; i <= 255; ++i)
   {
      REQUIRE(IsPiece(i) == false);
   }
}

TEST_CASE("Test NewPiece")
{
   REQUIRE(NewPiece(kPawn, Color::White()) == kWhitePawn);
   REQUIRE(NewPiece(kKnight, Color::White()) == kWhiteKnight);
   REQUIRE(NewPiece(kKing, Color::White()) == kWhiteKing);
   REQUIRE(NewPiece(kQueen, Color::White()) == kWhiteQueen);
   REQUIRE(NewPiece(kBishop, Color::White()) == kWhiteBishop);
   REQUIRE(NewPiece(kRook, Color::White()) == kWhiteRook);

   REQUIRE(NewPiece(kPawn, Color::Black()) == kBlackPawn);
   REQUIRE(NewPiece(kKnight, Color::Black()) == kBlackKnight);
   REQUIRE(NewPiece(kKing, Color::Black()) == kBlackKing);
   REQUIRE(NewPiece(kQueen, Color::Black()) == kBlackQueen);
   REQUIRE(NewPiece(kBishop, Color::Black()) == kBlackBishop);
   REQUIRE(NewPiece(kRook, Color::Black()) == kBlackRook);
}

TEST_CASE("Test GetPieceType")
{
   REQUIRE(GetPieceType(NewPiece(kPawn, Color::White())) == kPawn);
   REQUIRE(GetPieceType(NewPiece(kKnight, Color::White())) == kKnight);
   REQUIRE(GetPieceType(NewPiece(kKing, Color::White())) == kKing);
   REQUIRE(GetPieceType(NewPiece(kQueen, Color::White())) == kQueen);
   REQUIRE(GetPieceType(NewPiece(kBishop, Color::White())) == kBishop);
   REQUIRE(GetPieceType(NewPiece(kRook, Color::White())) == kRook);

   REQUIRE(GetPieceType(NewPiece(kPawn, Color::Black())) == kPawn);
   REQUIRE(GetPieceType(NewPiece(kKnight, Color::Black())) == kKnight);
   REQUIRE(GetPieceType(NewPiece(kKing, Color::Black())) == kKing);
   REQUIRE(GetPieceType(NewPiece(kQueen, Color::Black())) == kQueen);
   REQUIRE(GetPieceType(NewPiece(kBishop, Color::Black())) == kBishop);
   REQUIRE(GetPieceType(NewPiece(kRook, Color::Black())) == kRook);
}

TEST_CASE("Test GetColor")
{
   REQUIRE(GetColor(NewPiece(kPawn, Color::White())) == Color::White());
   REQUIRE(GetColor(NewPiece(kKnight, Color::White())) == Color::White());
   REQUIRE(GetColor(NewPiece(kKing, Color::White())) == Color::White());
   REQUIRE(GetColor(NewPiece(kQueen, Color::White())) == Color::White());
   REQUIRE(GetColor(NewPiece(kBishop, Color::White())) == Color::White());
   REQUIRE(GetColor(NewPiece(kRook, Color::White())) == Color::White());

   REQUIRE(GetColor(NewPiece(kPawn, Color::Black())) == Color::Black());
   REQUIRE(GetColor(NewPiece(kKnight, Color::Black())) == Color::Black());
   REQUIRE(GetColor(NewPiece(kKing, Color::Black())) == Color::Black());
   REQUIRE(GetColor(NewPiece(kQueen, Color::Black())) == Color::Black());
   REQUIRE(GetColor(NewPiece(kBishop, Color::Black())) == Color::Black());
   REQUIRE(GetColor(NewPiece(kRook, Color::Black())) == Color::Black());
}