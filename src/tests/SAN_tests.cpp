/// @file   SAN_tests.cpp
/// @author Mathieu Pagé
/// @date   Feburary 2015
/// @brief  Contains tests of methods contained in SAN.hpp.

#include <string>

#include "catch.hpp"

#include "../m8chess/SAN.hpp"

using namespace m8;

void ParserTest(std::string fen, std::string san, Move expected)
{
    Board board(fen);
    Move actual = ParseSAN(san, board);
    REQUIRE(expected == actual);
}

TEST_CASE("ParseSAN_OO_CorrectMovedReturned")
{   
    ParserTest("r3k3/1pp1q1P1/Q7/2n3pP/3PR3/2N5/1PP5/rB2K2R w Kq g6 0 1",
               "O-O",
               NewCastlingMove(kE1, kG1, kWhiteKing, kKingSideCastle));
}

TEST_CASE("ParseSAN_OOO_CorrectMovedReturned")
{
    ParserTest("r3k3/1pp1q1P1/Q7/2n3pP/3PR3/2N5/1PP5/rB2K2R b Kq g6 0 1",
               "O-O-O",
               NewCastlingMove(kE8, kC8, kBlackKing, kQueenSideCastle));
}

TEST_CASE("ParseSAN_PawnTwoSquarePush_CorrectMoveReturned")
{
    ParserTest("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 b kq - 0 1",
               "c5",
               NewMove(kC7, kC5, kBlackPawn));
}

TEST_CASE("ParseSAN_NormalRookMove_CorrectMoveReturned")
{
    ParserTest("r3k3/1pp1q1P1/Q7/2n3pP/3PR3/2N5/1PP5/rB2K2R w Kq g6 0 1",
               "Re6",
               NewMove(kE4, kE6, kWhiteRook));
}

TEST_CASE("ParseSAN_NormalKnightMove_CorrectMoveReturned")
{
    ParserTest("r3k3/1pp1q1P1/Q7/2n3pP/3PR3/2N5/1PP5/rB2K2R w Kq g6 0 1",
               "Na4",
               NewMove(kC3, kA4, kWhiteKnight));
}

TEST_CASE("ParseSAN_KnightCapture_CorrectMoveReturned")
{
    ParserTest("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 b kq - 0 1",
               "Nxc4",
               NewMove(kA5, kC4, kBlackKnight, kWhitePawn));
}

TEST_CASE("ParseSAN_PawnCapture_CorrectMoveReturned")
{
    ParserTest("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 b kq - 0 1",
               "gxh6",
               NewMove(kG7, kH6, kBlackPawn, kWhiteKnight));
}

TEST_CASE("ParseSAN_PawnPromotion_CorrectMoveReturned")
{
    ParserTest("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 b kq - 0 1",
               "b1=Q",
               NewMove(kB2, kB1, kBlackPawn, kNoPiece, kBlackQueen));
}

TEST_CASE("ParseSAN_PawnUnderPromotionAndCapture_CorrectMoveReturned")
{
    ParserTest("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 b kq - 0 1",
               "bxa1=N",
               NewMove(kB2, kA1, kBlackPawn, kWhiteRook, kBlackKnight));
}

TEST_CASE("ParseSAN_PriseEnPassant_CorrectMoveReturned")
{
    ParserTest("r3k2r/Ppp2ppp/1b3nbN/nP1pP3/BBP5/q4N2/Pp1P2PP/R2Q1RK1 w kq d5 0 1",
               "exd6",
               NewMove(kE5, kD6, kWhitePawn, kBlackPawn));
}

TEST_CASE("ParseSAN_CheckingMove_CorrectMoveReturned")
{
    ParserTest("r3k2r/Ppp2ppp/1b2pnbN/nP2P3/BBP5/q4N2/Pp2P1PP/R2Q1RK1 w kq - 0 1",
               "Qd7+",
               NewMove(kD1, kD7, kWhiteQueen));
}

TEST_CASE("ParseSAN_MatingMove_CorrectMoveReturned")
{
    ParserTest("r3k2r/Ppp2ppp/1b2pNN1/nP2P1n1/BBP5/q7/Pp2P1PP/R2Q1RK1 w kq - 0 1",
               "Qd7#",
               NewMove(kD1, kD7, kWhiteQueen));
}

TEST_CASE("ParseSAN_AmbiguousMoveDifferentiatedByColumn_CorrectMoveReturned")
{
    ParserTest("4k3/8/2r3r1/8/8/8/8/4K3 b - - 0 1",
               "Rce6+",
               NewMove(kC6, kE6, kBlackRook));
}

TEST_CASE("ParseSAN_AmbiguousMoveDifferentiatedByRow_CorrectMoveReturned")
{
    ParserTest("2r1k3/8/8/8/8/2r5/8/4K3 b - - 0 1",
               "R8c6",
               NewMove(kC8, kC6, kBlackRook));
}

TEST_CASE("ParseSAN_AmbiguousMoveDifferentiatedByRowAndColumn_CorrectMoveReturned")
{
    ParserTest("2N1k3/8/8/8/2N1N3/8/8/4K3 w - - 0 1",
               "Nc4d6",
               NewMove(kC4, kD6, kWhiteKnight));
}

TEST_CASE("ParseSAN_NonAmbiguousMoveBecauseOfPinnedPiece_CorrectMoveReturned")
{
    ParserTest("4k3/4r3/8/8/8/2N1N3/8/4K3 w - - 0 1",
               "Nd5",
               NewMove(kC3, kD5, kWhiteKnight));
}