/// @file   Bb_tests.cpp
/// @author Mathieu Pagé
/// @date   Feburary 2015
/// @brief  Contains tests of the types and functionalities in square.hpp.

#include "catch.hpp"

#include <cstdint>

#include "../m8common/Bb.hpp"
#include "../m8chess/Sq.hpp"

using namespace m8;
using namespace std;

TEST_CASE("Test GetBit")
{
   Bb bb = 0;
   Bb bb_ones = UINT64_C(0) - 1;

   for (uint8_t i = 0; i < 64; ++i)
   {
      REQUIRE(GetBit(bb, i) == false);
      REQUIRE(GetBit(bb_ones, i) == true);
   }

   Bb bb_two = UINT64_C(2);
   REQUIRE(GetBit(bb_two, 0) == false);
   REQUIRE(GetBit(bb_two, 1) == true);
   REQUIRE(GetBit(bb_two, 2) == false);
}

TEST_CASE("Test SetBit")
{
   Bb bb = 0;

   SetBit(bb, 2);
   REQUIRE(bb == UINT64_C(4));

   SetBit(bb, 2);
   REQUIRE(bb == UINT64_C(4));
}

TEST_CASE("Test UnsetBit")
{
   Bb bb = UINT64_C(4);

   UnsetBit(bb, 2);
   REQUIRE(bb == UINT64_C(0));

   UnsetBit(bb, 2);
   REQUIRE(bb == UINT64_C(0));
}

TEST_CASE("Test SwitchBit")
{
   Bb bb = UINT64_C(0);

   SwitchBit(bb, 2);
   REQUIRE(bb == UINT64_C(4));

   SwitchBit(bb, 2);
   REQUIRE(bb == UINT64_C(0));
}

TEST_CASE("Test GetLsb")
{
   for (uint8_t i = 0; i < 64; ++i)
   {
      Bb bb = GetSingleBitBb(i);
      REQUIRE(GetLsb(bb) == i);
   }

   Bb bb = GetSingleBitBb(63) | GetSingleBitBb(32) | GetSingleBitBb(5);
   REQUIRE(GetLsb(bb) == 5);
}

TEST_CASE("Test GetMsb")
{
   for (uint8_t i = 0; i < 64; ++i)
   {
      Bb bb = GetSingleBitBb(i);
      REQUIRE(GetMsb(bb) == i);
   }

   Bb bb = GetSingleBitBb(63) | GetSingleBitBb(32) | GetSingleBitBb(5);
   REQUIRE(GetMsb(bb) == 63);
}

TEST_CASE("Test GetPopct")
{
   for (uint8_t i = 0; i < 64; ++i)
   {
      Bb bb = GetSingleBitBb(i) - 1;
      REQUIRE(GetPopct(bb) == i);
   }

   Bb bb = GetSingleBitBb(63) | GetSingleBitBb(32) | GetSingleBitBb(5);
   REQUIRE(GetPopct(bb) == 3);

   bb = UINT64_C(0) - 1;
   REQUIRE(GetPopct(bb) == 64);
}

TEST_CASE("Test RemoveLsb")
{
   Bb bb = GetSingleBitBb(63) | GetSingleBitBb(32) | GetSingleBitBb(5);

   REQUIRE(RemoveLsb(bb) == 5);
   REQUIRE(RemoveLsb(bb) == 32);
   REQUIRE(RemoveLsb(bb) == 63);
   REQUIRE(bb == 0);
}

TEST_CASE("Shift_ZeroLength_BitboardIsUnchanged")
{
    Bb original = BB_C(0);
    SetBit(original, 10);
    SetBit(original, 19);
    SetBit(original, 54);

    Bb bb = original;
    Shift(bb, 0);

    REQUIRE(original == bb);
}

TEST_CASE("Shift_PositiveLength_BitboardIsShiftedLeft")
{
    Bb bb = BB_C(0);
    SetBit(bb, 10);
    SetBit(bb, 19);
    SetBit(bb, 54);

    Bb expected = BB_C(0);
    SetBit(expected, 13);
    SetBit(expected, 22);
    SetBit(expected, 57);

    Shift(bb, 3);

    REQUIRE(expected == bb);
}

TEST_CASE("Shift_NegateiveLength_BitboardIsShiftedRight")
{
    Bb bb = BB_C(0);
    SetBit(bb, 10);
    SetBit(bb, 19);
    SetBit(bb, 54);

    Bb expected = BB_C(0);
    SetBit(expected, 5);
    SetBit(expected, 14);
    SetBit(expected, 49);

    Shift(bb, -5);

    REQUIRE(expected == bb);
}

TEST_CASE("BbBetween__c2_to_g6__d2_e4_f5_returned")
{
    Bb between = BbBetween(kC2, kG6);
    REQUIRE(between == BB_C(0x0000002010080000));
}

TEST_CASE("BbBetween__g6_to_c2__d2_e4_f5_returned")
{
    using namespace std;
    Bb between = BbBetween(kG6, kC2);

    REQUIRE(between == BB_C(0x0000002010080000));
}

TEST_CASE("BbBetween__g6_to_c1__kEmptyBb_returned")
{
    using namespace std;
    Bb between = BbBetween(kG6, kC1);

    REQUIRE(between == kEmptyBb);
}