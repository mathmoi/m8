/// @file   Attacks_tests.cpp
/// @author Mathieu Pag�
/// @date   Janurary 2018
/// @brief  Contains tests of the types and functionalities in Attacks.hpp.

#include "catch2/catch_all.hpp"

#include "m8chess/movegen/Attacks.hpp"
#include "m8chess/movegen/MoveGeneration.hpp"

using namespace m8;
using namespace m8::movegen;

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

TEST_CASE("kiwipete_position__attacks_from__return_expected_value")
{
    // Arange
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    Sq sq;
    Bb expected;
    
    SECTION ("Rook       on a1") { sq = kA1; expected = BB_C(0x000000000000011e); }
    SECTION ("Rook       on h7") { sq = kH8; expected = BB_C(0x7080808080800000); }
    SECTION ("Queen      on e7") { sq = kE7; expected = BB_C(0x3828380402000000); }
    SECTION ("Bishop     on d2") { sq = kD2; expected = BB_C(0x0000804020140014); }
    SECTION ("Knight     on c3") { sq = kC3; expected = BB_C(0x0000000a1100110a); }
    SECTION ("King       on e1") { sq = kE1; expected = BB_C(0x0000000000003828); }
    SECTION ("King       on e8") { sq = kE8; expected = BB_C(0x2838000000000000); }
    SECTION ("White pawn on h2") { sq = kH2; expected = BB_C(0x0000000000400000); }
    SECTION ("White pawn on a2") { sq = kA2; expected = BB_C(0x0000000000020000); }
    SECTION ("White pawn on d5") { sq = kD5; expected = BB_C(0x0000140000000000); }
    SECTION ("Black pawn on h3") { sq = kH3; expected = BB_C(0x0000000000004000); }
    SECTION ("Black pawn on a7") { sq = kA7; expected = BB_C(0x0000020000000000); }
    SECTION ("Black pawn on e6") { sq = kE6; expected = BB_C(0x0000002800000000); }

    // Act
    Bb actual = AttacksFrom(board, sq);

    // Assert
    REQUIRE(expected == actual);    
}