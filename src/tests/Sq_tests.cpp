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

   for (Column colmn = 0; colmn < Column::NumColumnOnBoard; colmn = colmn.MoveRight())
   {
      REQUIRE(colmn.IsOnBoard() == true);
   }

   REQUIRE(Column::Invalid().IsOnBoard() == false);
}


TEST_CASE("Test IsRowOnBoard")
{
   REQUIRE(Row::NumRowOnBoard == 8);

   for (Row row = 0; row < Row::NumRowOnBoard; row = row.MoveUp())
   {
      REQUIRE(row.IsOnBoard() == true);
   }

   REQUIRE(Row::_1().MoveDown().IsOnBoard() == false);
   REQUIRE(Row::_8().MoveUp().IsOnBoard() == false);
}

TEST_CASE("Test IsSqOnBoard")
{
   REQUIRE(Sq::NumSqOnBoard == 64);

   for (Sq sq = 0; sq.IsOnBoard(); sq = sq.MoveNext())
   {
      REQUIRE(sq.IsOnBoard() == true);
   }

   REQUIRE(Sq::A1().MoveDown().IsOnBoard() == false);
   REQUIRE(Sq::H8().MoveUp().IsOnBoard() == false);
}

TEST_CASE("Test GetRow")
{
   REQUIRE(Sq::A1().row() == Row::_1());
   REQUIRE(Sq::H1().row() == Row::_1());
   REQUIRE(Sq::A8().row() == Row::_8());
   REQUIRE(Sq::H8().row() == Row::_8());

   REQUIRE(Sq::B1().row() == Row::_1());
   REQUIRE(Sq::D3().row() == Row::_3());
   REQUIRE(Sq::F5().row() == Row::_5());
   REQUIRE(Sq::G8().row() == Row::_8());
}

TEST_CASE("Test GetColmn")
{
   REQUIRE(Sq::A1().column() == Column::A());
   REQUIRE(Sq::H1().column() == Column::H());
   REQUIRE(Sq::A8().column() == Column::A());
   REQUIRE(Sq::H8().column() == Column::H());

   REQUIRE(Sq::B1().column() == Column::B());
   REQUIRE(Sq::D3().column() == Column::D());
   REQUIRE(Sq::F5().column() == Column::F());
   REQUIRE(Sq::G8().column() == Column::G());
}
