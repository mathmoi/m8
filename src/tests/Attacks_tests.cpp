/// @file   Attacks_tests.cpp
/// @author Mathieu Pagé
/// @date   Janurary 2018
/// @brief  Contains tests of the types and functionalities in Attacks.hpp.

#include "catch.hpp"

#include "../m8chess/Attacks.hpp"

using namespace m8;

TEST_CASE("GenerateRookAttacks_called_ReturnExpectedValue")
{
    Bb occ = BB_C(0xFFDFEF20EFCFAF7F);
    Bb result;

    result = GenerateRookAttacks(occ, kE5);

    REQUIRE(BB_C(0x0010102F10101010) == result);
}

TEST_CASE("m8_rook_attacks__empty_board__return_expected_value")
{
    Bb occ = BB_C(0x0000000000000080);
    Bb result;

    result = GenerateRookAttacks(occ, kH1);

    REQUIRE(BB_C(0x808080808080807F) == result);
}

TEST_CASE("m8_bishop_attacks__called__return_expected_value")
{
    Bb occ = BB_C(0xFFDFEB20EFCFAF7F);
    Bb result;

    result = GenerateBishopAttacks(occ, kD5);

    REQUIRE(BB_C(0x4022140014204080) == result);
}