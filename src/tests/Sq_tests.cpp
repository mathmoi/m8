/// @file   square_tests.cpp
/// @author Mathieu Pagé
/// @date   Feburary 2015
/// @brief  Contains tests of the types and functionalities in square.hpp.

#include "catch.hpp"

#include "..\common\Sq.hpp"

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
