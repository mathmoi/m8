/// @file   XRay_tests.cpp
/// @author Mathieu Pag�
/// @date   Janurary 2018
/// @brief  Contains tests of the types and functionalities in XRay.hpp.

#include "catch2/catch_all.hpp"

#include "m8chess/XRay.hpp"

using namespace m8;

TEST_CASE("GenerateRookXRay_ComplexePositions_XRayAttackesReturned")
{
    Bb occ = BB_C(0x0000006500080008);
    Bb blockers = BB_C(0x0000002400080000);

    Bb expected = BB_C(0x0000004300000808);

    Bb xray = GenerateRookXRay(occ, blockers, kD5);

    REQUIRE(expected == xray);
}

TEST_CASE("GenerateBishopXRay_ComplexePositions_XRayAttackesReturned")
{
    Bb occ = BB_C(0x4000100004204100);
    Bb blockers = BB_C(0x4000100000204100);

    Bb expected = BB_C(0x4020000000004000);

    Bb xray = GenerateBishopXRay(occ, blockers, kD5);

    REQUIRE(expected == xray);
}