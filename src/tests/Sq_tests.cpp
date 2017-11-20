/// @file   Sq_tests.cpp
/// @author Mathieu Pagé
/// @date   Feburary 2015
/// @brief  Contains tests of the types and functionalities in Sq.hpp.

#include "catch.hpp"

#include "../m8chess/Sq.hpp"

using namespace m8;

TEST_CASE("Test IsColmnOnBoard")
{
   REQUIRE(Column::NumColumnOnBoard == 8);

   for (Column colmn = 0; colmn < Column::NumColumnOnBoard; colmn.MoveNext())
   {
      REQUIRE(colmn.IsOnBoard() == true);
   }

   REQUIRE(Column::Invalid().IsOnBoard() == false);
}


TEST_CASE("Test IsRowOnBoard")
{
   REQUIRE(Row::NumRowOnBoard == 8);

   for (Row row = 0; row < Row::NumRowOnBoard; row.MoveNext())
   {
      REQUIRE(row.IsOnBoard() == true);
   }

   REQUIRE(Row::_1().MovePrevious().IsOnBoard() == false);
   REQUIRE(Row::_8().MoveNext().IsOnBoard() == false);
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
   REQUIRE(GetRow(kA1) == Row::_1());
   REQUIRE(GetRow(kH1) == Row::_1());
   REQUIRE(GetRow(kA8) == Row::_8());
   REQUIRE(GetRow(kH8) == Row::_8());

   REQUIRE(GetRow(kB1) == Row::_1());
   REQUIRE(GetRow(kD3) == Row::_3());
   REQUIRE(GetRow(kF5) == Row::_5());
   REQUIRE(GetRow(kG8) == Row::_8());
}

TEST_CASE("Test GetColmn")
{
   REQUIRE(GetColmn(kA1) == Column::A());
   REQUIRE(GetColmn(kH1) == Column::H());
   REQUIRE(GetColmn(kA8) == Column::A());
   REQUIRE(GetColmn(kH8) == Column::H());

   REQUIRE(GetColmn(kB1) == Column::B());
   REQUIRE(GetColmn(kD3) == Column::D());
   REQUIRE(GetColmn(kF5) == Column::F());
   REQUIRE(GetColmn(kG8) == Column::G());
}
