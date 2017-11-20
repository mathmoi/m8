/// @file   Move_tests.cpp
/// @author Mathieu Pagé
/// @date   December 2015
/// @brief  Contains tests of the types and functionalities in Move.hpp.

#include "catch.hpp"

#include "../m8chess/Move.hpp"
#include "../m8chess/Sq.hpp"
#include "../m8chess/Piece.hpp"

using namespace m8;

TEST_CASE("NewMove_NormalConditions_AllInformationsRetrievables")
{
    Sq from = Sq::D7();
    Sq to = Sq::E8();
    Piece piece = kWhitePawn;
    Piece piece_taken = kBlackQueen;
    Piece promote_to = kWhiteQueen;

    Move move = NewMove(from, to, piece, piece_taken, promote_to);

    REQUIRE(GetFrom(move) == from);
    REQUIRE(GetTo(move) == to);
    REQUIRE(GetPiece(move) == piece);
    REQUIRE(GetPieceTaken(move) == piece_taken);
    REQUIRE(GetPromoteTo(move) == promote_to);
}

TEST_CASE("NewMove_NoPromoteTo_AllInformationRetrievableAndNoPromotion")
{
    Sq from = Sq::D7();
    Sq to = Sq::E8();
    Piece piece = kWhitePawn;
    Piece piece_taken = kBlackQueen;

    Move move = NewMove(from, to, piece, piece_taken);

    REQUIRE(GetFrom(move) == from);
    REQUIRE(GetTo(move) == to);
    REQUIRE(GetPiece(move) == piece);
    REQUIRE(GetPieceTaken(move) == piece_taken);
    REQUIRE(GetPromoteTo(move) == kNoPiece);
}

TEST_CASE("NewMove_NoPromoteToAndNoPieceTaken_AllInformationRetrievableAndNoPromotionAndNoPieceTaken")
{
    Sq from = Sq::D7();
    Sq to = Sq::E8();
    Piece piece = kWhitePawn;

    Move move = NewMove(from, to, piece);

    REQUIRE(GetFrom(move) == from);
    REQUIRE(GetTo(move) == to);
    REQUIRE(GetPiece(move) == piece);
    REQUIRE(GetPieceTaken(move) == kNoPiece);
    REQUIRE(GetPromoteTo(move) == kNoPiece);
}