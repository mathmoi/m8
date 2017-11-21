/// @file   Sq_tests.cpp
/// @author Mathieu Pagé
/// @date   Feburary 2015
/// @brief  Contains tests of the types and functionalities in Sq.hpp.

#include "catch.hpp"

#include "../m8chess/Sq.hpp"

using namespace m8;

TEST_CASE("Test IsColmnOnBoard")
{
   REQUIRE(kNumColmnOnBoard == 8);

   for (Colmn colmn = 0; colmn < kNumColmnOnBoard; ++colmn)
   {
      REQUIRE(IsColmnOnBoard(colmn) == true);
   }

   REQUIRE(IsColmnOnBoard(kColmnA - 1) == false);
   REQUIRE(IsColmnOnBoard(kColmnH + 1) == false);

   REQUIRE(IsColmnOnBoard(kInvalColmn) == false);
}


TEST_CASE("Test IsRowOnBoard")
{
   REQUIRE(kNumRowOnBoard == 8);

   for (Row row = 0; row < kNumRowOnBoard; ++row)
   {
      REQUIRE(IsRowOnBoard(row) == true);
   }

   REQUIRE(IsRowOnBoard(kRow1 - 1) == false);
   REQUIRE(IsRowOnBoard(kRow8 + 1) == false);
}

TEST_CASE("Test IsSqOnBoard")
{
   REQUIRE(kNumSqOnBoard == 64);

   for (Sq sq = 0; sq < kNumSqOnBoard; ++sq)
   {
      REQUIRE(IsSqOnBoard(sq) == true);
   }

   REQUIRE(IsSqOnBoard(kA1 - 1) == false);
   REQUIRE(IsSqOnBoard(kH8 + 1) == false);
}

TEST_CASE("Test GetRow")
{
   REQUIRE(GetRow(kA1) == kRow1);
   REQUIRE(GetRow(kH1) == kRow1);
   REQUIRE(GetRow(kA8) == kRow8);
   REQUIRE(GetRow(kH8) == kRow8);

   REQUIRE(GetRow(kB1) == kRow1);
   REQUIRE(GetRow(kD3) == kRow3);
   REQUIRE(GetRow(kF5) == kRow5);
   REQUIRE(GetRow(kG8) == kRow8);
}

TEST_CASE("Test GetColmn")
{
   REQUIRE(GetColmn(kA1) == kColmnA);
   REQUIRE(GetColmn(kH1) == kColmnH);
   REQUIRE(GetColmn(kA8) == kColmnA);
   REQUIRE(GetColmn(kH8) == kColmnH);

   REQUIRE(GetColmn(kB1) == kColmnB);
   REQUIRE(GetColmn(kD3) == kColmnD);
   REQUIRE(GetColmn(kF5) == kColmnF);
   REQUIRE(GetColmn(kG8) == kColmnG);
}
