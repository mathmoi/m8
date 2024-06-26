/// @file   Checkmate_tests.cpp
/// @author Mathieu Pag�
/// @date   Janurary 2018
/// @brief  Contains tests for the methods located in checkmate.hpp.

#include "catch2/catch_all.hpp"

#include <cstdint>

#include "m8chess/Checkmate.hpp"

using namespace m8;

TEST_CASE("IsInCheck__white_king_attack_by_rook__return_true")
{
    Board board("8/8/4k3/8/r3K2R/8/8/8 w - - 0 1");

    bool in_check = IsInCheck(kWhite, board);

    REQUIRE(in_check == true);
}

TEST_CASE("IsInCheck__black_king_is_not_in_check__return_false")
{
    Board board("8/8/4k3/8/r3K2R/8/8/8 w - - 0 1");

    bool in_check = IsInCheck(kBlack, board);

    REQUIRE(in_check == false);
}

TEST_CASE("IsInCheck__white_king_is_protected_by_own_piece__return_false")
{
    Board board("8/8/4k3/8/r2QK2R/8/8/8 w - - 0 1");

    bool in_check = IsInCheck(kWhite, board);

    REQUIRE(in_check == false);
}