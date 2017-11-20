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
      REQUIRE(bb[i] == false);
      REQUIRE(bb_ones[i] == true);
   }

   Bb bb_two = UINT64_C(2);
   REQUIRE(bb_two[0] == false);
   REQUIRE(bb_two[1] == true);
   REQUIRE(bb_two[2] == false);
}

TEST_CASE("Test SetBit")
{
   Bb bb = 0;

   bb.Set(2);
   REQUIRE(bb == UINT64_C(4));

   bb.Set(2);
   REQUIRE(bb == UINT64_C(4));
}

TEST_CASE("Test UnsetBit")
{
   Bb bb = UINT64_C(4);

   bb.Unset(2);
   REQUIRE(bb == UINT64_C(0));

   bb.Unset(2);
   REQUIRE(bb == UINT64_C(0));
}

TEST_CASE("Test Switch")
{
   Bb bb = UINT64_C(0);

   bb.Switch(2);
   REQUIRE(bb == UINT64_C(4));

   bb.Switch(2);
   REQUIRE(bb == UINT64_C(0));
}

TEST_CASE("Test GetLsb")
{
   for (uint8_t i = 0; i < 64; ++i)
   {
      Bb bb = Bb::GetSingleBitBb(i);
      REQUIRE(bb.GetLSB() == i);
   }

   Bb bb = Bb::GetSingleBitBb(63) | Bb::GetSingleBitBb(32) | Bb::GetSingleBitBb(5);
   REQUIRE(bb.GetLSB() == 5);
}

TEST_CASE("Test GetMsb")
{
   for (uint8_t i = 0; i < 64; ++i)
   {
      Bb bb = Bb::GetSingleBitBb(i);
      REQUIRE(bb.GetMSB() == i);
   }

   Bb bb = Bb::GetSingleBitBb(63) | Bb::GetSingleBitBb(32) | Bb::GetSingleBitBb(5);
   REQUIRE(bb.GetMSB() == 63);
}

TEST_CASE("Test GetPopct")
{
   for (uint8_t i = 0; i < 64; ++i)
   {
      Bb bb = Bb::GetSingleBitBb(i) - 1;
      REQUIRE(bb.GetPopct() == i);
   }

   Bb bb = Bb::GetSingleBitBb(63) | Bb::GetSingleBitBb(32) | Bb::GetSingleBitBb(5);
   REQUIRE(bb.GetPopct() == 3);

   bb = UINT64_C(0) - 1;
   REQUIRE(bb.GetPopct() == 64);
}

TEST_CASE("Test RemoveLsb")
{
   Bb bb = Bb::GetSingleBitBb(63) | Bb::GetSingleBitBb(32) | Bb::GetSingleBitBb(5);

   REQUIRE(bb.RemoveLSB() == 5);
   REQUIRE(bb.RemoveLSB() == 32);
   REQUIRE(bb.RemoveLSB() == 63);
   REQUIRE(bb == 0);
}

TEST_CASE("Shift_ZeroLength_BitboardIsUnchanged")
{
    Bb original = UINT64_C(0);
    original.Set(10);
    original.Set(19);
    original.Set(54);

    Bb bb = original;
    bb.Shift( 0);

    REQUIRE(original == bb);
}

TEST_CASE("Shift_PositiveLength_BitboardIsShiftedLeft")
{
    Bb bb = UINT64_C(0);
    bb.Set(10);
    bb.Set(19);
    bb.Set(54);

    Bb expected = UINT64_C(0);
    expected.Set(13);
    expected.Set(22);
    expected.Set(57);

    bb.Shift( 3);

    REQUIRE(expected == bb);
}

TEST_CASE("Shift_NegateiveLength_BitboardIsShiftedRight")
{
    Bb bb = UINT64_C(0);
    bb.Set(10);
    bb.Set(19);
    bb.Set(54);

    Bb expected = UINT64_C(0);
    expected.Set(5);
    expected.Set(14);
    expected.Set(49);

    bb.Shift( -5);

    REQUIRE(expected == bb);
}

TEST_CASE("BbBetween__c2_to_g6__d2_e4_f5_returned")
{
    Bb between = BbBetween(Sq::C2().value(), Sq::G6().value());
    REQUIRE(between == UINT64_C(0x0000002010080000));
}

TEST_CASE("BbBetween__g6_to_c2__d2_e4_f5_returned")
{
    using namespace std;
    Bb between = BbBetween(Sq::G6().value(), Sq::C2().value());

    REQUIRE(between == UINT64_C(0x0000002010080000));
}

TEST_CASE("BbBetween__g6_to_c1__Bb::Empty()_returned")
{
    using namespace std;
    Bb between = BbBetween(Sq::G6().value(), Sq::C1().value());

    REQUIRE(between == Bb::Empty());
}