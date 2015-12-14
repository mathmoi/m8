/// @file   Piece_tests.cpp
/// @author Mathieu Pagé
/// @date   Feburary 2015
/// @brief  Contains tests of the types and functionalities in Piece.hpp.

#include "catch.hpp"
#include <cassert>

#include "../common/Piece.hpp"

using namespace m8;

TEST_CASE("Test IsPieceType")
{
   REQUIRE(IsPieceType(kPawn) == true);
   REQUIRE(IsPieceType(kKnigt) == true);
   REQUIRE(IsPieceType(kKing) == true);
   REQUIRE(IsPieceType(kQueen) == true);
   REQUIRE(IsPieceType(kBishp) == true);
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
   REQUIRE(CanSlide(kBishp) == true);

   REQUIRE(CanSlide(kPawn) == false);
   REQUIRE(CanSlide(kKnigt) == false);
   REQUIRE(CanSlide(kKing) == false);
}

TEST_CASE("Test CanSlideRook")
{
   REQUIRE(CanSlideRook(kQueen) == true);
   REQUIRE(CanSlideRook(kRook) == true);

   REQUIRE(CanSlideRook(kBishp) == false);
   REQUIRE(CanSlideRook(kPawn) == false);
   REQUIRE(CanSlideRook(kKnigt) == false);
   REQUIRE(CanSlideRook(kKing) == false);
}

TEST_CASE("Test CanSlideBishp")
{
   REQUIRE(CanSlideBishp(kQueen) == true);
   REQUIRE(CanSlideBishp(kBishp) == true);

   REQUIRE(CanSlideBishp(kRook) == false);
   REQUIRE(CanSlideBishp(kPawn) == false);
   REQUIRE(CanSlideBishp(kKnigt) == false);
   REQUIRE(CanSlideBishp(kKing) == false);
}

TEST_CASE("Test IsColor")
{
   REQUIRE(IsColor(kWhite) == true);
   REQUIRE(IsColor(kBlack) == true);

   for (uint32_t i = kBlack + 1; i <= 255; ++i)
   {
      REQUIRE(IsColor(static_cast<Color>(i)) == false);
   }
}

TEST_CASE("Test OpposColor")
{
   REQUIRE(OpposColor(kWhite) == kBlack);
   REQUIRE(OpposColor(kBlack) == kWhite);
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
   REQUIRE(NewPiece(kPawn, kWhite) == kWhitePawn);
   REQUIRE(NewPiece(kKnigt, kWhite) == kWhiteKnigt);
   REQUIRE(NewPiece(kKing, kWhite) == kWhiteKing);
   REQUIRE(NewPiece(kQueen, kWhite) == kWhiteQueen);
   REQUIRE(NewPiece(kBishp, kWhite) == kWhiteBishp);
   REQUIRE(NewPiece(kRook, kWhite) == kWhiteRook);

   REQUIRE(NewPiece(kPawn, kBlack) == kBlackPawn);
   REQUIRE(NewPiece(kKnigt, kBlack) == kBlackKnigt);
   REQUIRE(NewPiece(kKing, kBlack) == kBlackKing);
   REQUIRE(NewPiece(kQueen, kBlack) == kBlackQueen);
   REQUIRE(NewPiece(kBishp, kBlack) == kBlackBishp);
   REQUIRE(NewPiece(kRook, kBlack) == kBlackRook);
}

TEST_CASE("Test GetPieceType")
{
   REQUIRE(GetPieceType(NewPiece(kPawn, kWhite)) == kPawn);
   REQUIRE(GetPieceType(NewPiece(kKnigt, kWhite)) == kKnigt);
   REQUIRE(GetPieceType(NewPiece(kKing, kWhite)) == kKing);
   REQUIRE(GetPieceType(NewPiece(kQueen, kWhite)) == kQueen);
   REQUIRE(GetPieceType(NewPiece(kBishp, kWhite)) == kBishp);
   REQUIRE(GetPieceType(NewPiece(kRook, kWhite)) == kRook);

   REQUIRE(GetPieceType(NewPiece(kPawn, kBlack)) == kPawn);
   REQUIRE(GetPieceType(NewPiece(kKnigt, kBlack)) == kKnigt);
   REQUIRE(GetPieceType(NewPiece(kKing, kBlack)) == kKing);
   REQUIRE(GetPieceType(NewPiece(kQueen, kBlack)) == kQueen);
   REQUIRE(GetPieceType(NewPiece(kBishp, kBlack)) == kBishp);
   REQUIRE(GetPieceType(NewPiece(kRook, kBlack)) == kRook);
}

TEST_CASE("Test GetColor")
{
   REQUIRE(GetColor(NewPiece(kPawn, kWhite)) == kWhite);
   REQUIRE(GetColor(NewPiece(kKnigt, kWhite)) == kWhite);
   REQUIRE(GetColor(NewPiece(kKing, kWhite)) == kWhite);
   REQUIRE(GetColor(NewPiece(kQueen, kWhite)) == kWhite);
   REQUIRE(GetColor(NewPiece(kBishp, kWhite)) == kWhite);
   REQUIRE(GetColor(NewPiece(kRook, kWhite)) == kWhite);

   REQUIRE(GetColor(NewPiece(kPawn, kBlack)) == kBlack);
   REQUIRE(GetColor(NewPiece(kKnigt, kBlack)) == kBlack);
   REQUIRE(GetColor(NewPiece(kKing, kBlack)) == kBlack);
   REQUIRE(GetColor(NewPiece(kQueen, kBlack)) == kBlack);
   REQUIRE(GetColor(NewPiece(kBishp, kBlack)) == kBlack);
   REQUIRE(GetColor(NewPiece(kRook, kBlack)) == kBlack);
}