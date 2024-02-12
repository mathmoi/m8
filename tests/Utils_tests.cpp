/// @file Utils_tests.cpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023

#include "catch.hpp"

#include "../m8common/Utils.hpp"

TEST_CASE("CalculateMask_Pos2Size3_CorrecMaskedReturned")
{
    std::uint64_t position = 2;
    std::uint64_t size     = 3;
    std::uint64_t expected = 0x1c;

    std::uint64_t actual = m8::CalculateMask(position, size);

    REQUIRE(expected == actual);
}