/// @file   SAN_tests.cpp
/// @author Mathieu Pag�
/// @date   Feburary 2015
/// @brief  Contains tests of methods contained in SAN.hpp.

#include <string>

#include "catch.hpp"

#include "../m8chess/SAN.hpp"

using namespace m8;

void ParserTestSAN(std::string fen, std::string san, Move expected)
{
    Board board(fen);
    Move actual = ParseSAN(san, board);
    REQUIRE(expected == actual);
}

void RenderTestSAN(std::string fen, Move move, std::string expected)
{
    Board board(fen);
    std::string actual = RenderSAN(move, board);
    REQUIRE(expected == actual);
}

/////////////////////////////////////////////////////////////////////////////////////////
//                                 ParseSAN tests                                      //
/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("ParseSAN_OO_CorrectMovedReturned")
{   
    ParserTestSAN("r3k3/1pp1q1P1/Q7/2n3pP/3PR3/2N5/1PP5/rB2K2R w Kq g6 0 1",
               "O-O",
               NewCastlingMove(kE1, kG1, kWhiteKing, kKingSideCastle));
}

TEST_CASE("ParseSAN_OOO_CorrectMovedReturned")
{
    ParserTestSAN("r3k3/1pp1q1P1/Q7/2n3pP/3PR3/2N5/1PP5/rB2K2R b Kq g6 0 1",
               "O-O-O",
               NewCastlingMove(kE8, kC8, kBlackKing, kQueenSideCastle));
}

TEST_CASE("ParseSAN_PawnTwoSquarePush_CorrectMoveReturned")
{
    ParserTestSAN("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 b kq - 0 1",
               "c5",
               NewMove(kC7, kC5, kBlackPawn));
}

TEST_CASE("ParseSAN_NormalRookMove_CorrectMoveReturned")
{
    ParserTestSAN("r3k3/1pp1q1P1/Q7/2n3pP/3PR3/2N5/1PP5/rB2K2R w Kq g6 0 1",
               "Re6",
               NewMove(kE4, kE6, kWhiteRook));
}

TEST_CASE("ParseSAN_NormalKnightMove_CorrectMoveReturned")
{
    ParserTestSAN("r3k3/1pp1q1P1/Q7/2n3pP/3PR3/2N5/1PP5/rB2K2R w Kq g6 0 1",
               "Na4",
               NewMove(kC3, kA4, kWhiteKnight));
}

TEST_CASE("ParseSAN_KnightCapture_CorrectMoveReturned")
{
    ParserTestSAN("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 b kq - 0 1",
               "Nxc4",
               NewMove(kA5, kC4, kBlackKnight, kWhitePawn));
}

TEST_CASE("ParseSAN_PawnCapture_CorrectMoveReturned")
{
    ParserTestSAN("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 b kq - 0 1",
               "gxh6",
               NewMove(kG7, kH6, kBlackPawn, kWhiteKnight));
}

TEST_CASE("ParseSAN_PawnPromotion_CorrectMoveReturned")
{
    ParserTestSAN("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 b kq - 0 1",
               "b1=Q",
               NewMove(kB2, kB1, kBlackPawn, kNoPiece, kBlackQueen));
}

TEST_CASE("ParseSAN_PawnUnderPromotionAndCapture_CorrectMoveReturned")
{
    ParserTestSAN("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 b kq - 0 1",
               "bxa1=N",
               NewMove(kB2, kA1, kBlackPawn, kWhiteRook, kBlackKnight));
}

TEST_CASE("ParseSAN_PriseEnPassant_CorrectMoveReturned")
{
    ParserTestSAN("r3k2r/Ppp2ppp/1b3nbN/nP1pP3/BBP5/q4N2/Pp1P2PP/R2Q1RK1 w kq d5 0 1",
               "exd6",
               NewMove(kE5, kD6, kWhitePawn, kBlackPawn));
}

TEST_CASE("ParseSAN_CheckingMove_CorrectMoveReturned")
{
    ParserTestSAN("r3k2r/Ppp2ppp/1b2pnbN/nP2P3/BBP5/q4N2/Pp2P1PP/R2Q1RK1 w kq - 0 1",
               "Qd7+",
               NewMove(kD1, kD7, kWhiteQueen));
}

TEST_CASE("ParseSAN_MatingMove_CorrectMoveReturned")
{
    ParserTestSAN("r3k2r/Ppp2ppp/1b2pNN1/nP2P1n1/BBP5/q7/Pp2P1PP/R2Q1RK1 w kq - 0 1",
               "Qd7#",
               NewMove(kD1, kD7, kWhiteQueen));
}

TEST_CASE("ParseSAN_AmbiguousMoveDifferentiatedByColumn_CorrectMoveReturned")
{
    ParserTestSAN("4k3/8/2r3r1/8/8/8/8/4K3 b - - 0 1",
               "Rce6+",
               NewMove(kC6, kE6, kBlackRook));
}

TEST_CASE("ParseSAN_AmbiguousMoveDifferentiatedByRow_CorrectMoveReturned")
{
    ParserTestSAN("2r1k3/8/8/8/8/2r5/8/4K3 b - - 0 1",
               "R8c6",
               NewMove(kC8, kC6, kBlackRook));
}

TEST_CASE("ParseSAN_AmbiguousMoveDifferentiatedByRowAndColumn_CorrectMoveReturned")
{
    ParserTestSAN("2N1k3/8/8/8/2N1N3/8/8/4K3 w - - 0 1",
               "Nc4d6",
               NewMove(kC4, kD6, kWhiteKnight));
}

TEST_CASE("ParseSAN_NonAmbiguousMoveBecauseOfPinnedPiece_CorrectMoveReturned")
{
    ParserTestSAN("4k3/4r3/8/8/8/2N1N3/8/4K3 w - - 0 1",
               "Nd5",
               NewMove(kC3, kD5, kWhiteKnight));
}

/////////////////////////////////////////////////////////////////////////////////////////
//                                 RenderSAN tests                                     //
/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("RenderSAN_KingSideMove_CorrectStringReturned")
{
    RenderTestSAN("r3k3/1pp1q1P1/Q7/2n3pP/3PR3/2N5/1PP5/rB2K2R w Kq g6 0 1",
               NewCastlingMove(kE1, kG1, kWhiteKing, kKingSideCastle),
               "O-O");
}

TEST_CASE("RenderSAN_QueenSideMove_CorrectStringReturned")
{
    RenderTestSAN("r3k3/1pp1q1P1/Q7/2n3pP/3PR3/2N5/1PP5/rB2K2R b Kq g6 0 1",
               NewCastlingMove(kE8, kC8, kBlackKing, kQueenSideCastle),
               "O-O-O");
}

TEST_CASE("RenderSAN_SimpleKnightMove_CorrectStringReturned")
{
    RenderTestSAN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
               NewMove(kB1, kC3, kWhiteKnight),
               "Nc3");
}

TEST_CASE("RenderSAN_PawnTwoSquarePush_CorrectStringReturned")
{
    RenderTestSAN("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 b kq - 0 1",
               NewMove(kC7, kC5, kBlackPawn),
               "c5");
}

TEST_CASE("RenderSAN_KnightCapture_CorrectStringReturned")
{
    RenderTestSAN("r3k2r/Pppp1ppp/5nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 b kq - 0 1",
               NewMove(kA5, kC4, kBlackKnight, kWhitePawn),
               "Nxc4");
}

TEST_CASE("RenderSAN_PawnCapture_CorrectStringReturned")
{
    RenderTestSAN("r3k2r/Pppp1ppp/5nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 b kq - 0 1",
               NewMove(kG7, kH6, kBlackPawn, kWhiteKnight),
               "gxh6");
}

TEST_CASE("RenderSAN_PawnPromotion_CorrectStringReturned")
{
    RenderTestSAN("r3k2r/Pppp1ppp/5nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 b kq - 0 1",
               NewMove(kB2, kB1, kBlackPawn, kNoPiece, kBlackQueen),
               "b1=Q");
}

TEST_CASE("RenderSAN_PawnUnderPromotionAndCapture_CorrectStringReturned")
{
    RenderTestSAN("r3k2r/Pppp1ppp/5nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 b kq - 0 1",
               NewMove(kB2, kA1, kBlackPawn, kWhiteRook, kBlackKnight),
               "bxa1=N");
}

TEST_CASE("RenderSAN_PriseEnPassant_CorrectStringReturned")
{
    RenderTestSAN("r3k2r/Ppp2ppp/1b3nbN/nP1pP3/BBP5/q4N2/Pp1P2PP/R2Q1RK1 w kq d5 0 1",
               NewMove(kE5, kD6, kWhitePawn, kBlackPawn),
               "exd6");
}

TEST_CASE("RenderSAN_CheckingMove_CorrectStringReturned")
{
    RenderTestSAN("r3k2r/Ppp2ppp/1b2pnbN/nP2P3/BBP5/q4N2/Pp2P1PP/R2Q1RK1 w kq - 0 1",
               NewMove(kD1, kD7, kWhiteQueen),
               "Qd7+");
}

TEST_CASE("RenderSAN_MatingMove_CorrectStringReturned")
{
    RenderTestSAN("r3k2r/Ppp2ppp/1b2pNN1/nP2P1n1/BBP5/q7/Pp2P1PP/R2Q1RK1 w kq - 0 1",
               NewMove(kD1, kD7, kWhiteQueen),
               "Qd7#");
}


TEST_CASE("RenderSAN_AmbiguousMoveDifferentiatedByColumn_CorrectStringReturned")
{
    RenderTestSAN("4k3/8/2r3r1/8/8/8/8/4K3 b - - 0 1",
               NewMove(kC6, kE6, kBlackRook),
               "Rce6+");
}

TEST_CASE("RenderSAN_AmbiguousMoveDifferentiatedByColumn2_CorrectStringReturned")
{
    RenderTestSAN("8/1p6/p2k2p1/P2n2rp/8/R7/1P5r/2KR4 w - h6 0 38",
               NewMove(kA3, kD3, kWhiteRook),
               "Rad3");
}

TEST_CASE("RenderSAN_AmbiguousMoveDifferentiatedByColumn3_CorrectStringReturned")
{
    RenderTestSAN("8/1p6/p2k2p1/P2n2rp/8/R7/1P5r/2KR4 w - h6 0 38",
               NewMove(kD1, kD3, kWhiteRook),
               "Rdd3");
}

TEST_CASE("RenderSAN_AmbiguousMoveDifferentiatedByRow_CorrectStringReturned")
{
    RenderTestSAN("2r1k3/8/8/8/8/2r5/8/4K3 b - - 0 1",
               NewMove(kC8, kC6, kBlackRook),
               "R8c6");
}

TEST_CASE("RenderSAN_AmbiguousMoveDifferentiatedByRowAndColumn_CorrectStringReturned")
{
    RenderTestSAN("2N1k3/8/8/8/2N1N3/8/8/4K3 w - - 0 1",
               NewMove(kC4, kD6, kWhiteKnight),
               "Nc4d6+");
}

TEST_CASE("RenderSAN_NonAmbiguousMoveBecauseOfPinnedPiece_CorrectStringReturned")
{
    RenderTestSAN("4k3/4r3/8/8/8/2N1N3/8/4K3 w - - 0 1",
               NewMove(kC3, kD5, kWhiteKnight),
               "Nd5");
}

TEST_CASE("RenderSAN_SimplePawnMove_CorrectStringReturned")
{
    RenderTestSAN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
               NewMove(kB2, kB3, kWhitePawn),
               "b3");
}