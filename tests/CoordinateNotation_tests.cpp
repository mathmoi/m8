/// @file   CoordinateNotation_tests.cpp
/// @author Mathieu Pagé
/// @date   December 2022
/// @brief  Contains tests of methods contained in CoordinateNotation.hpp.

#include <string>

#include "catch2/catch_all.hpp"

#include "m8chess/CoordinateNotation.hpp"

using namespace m8;

void ParserTest(std::string fen, std::string move, Move expected)
{
    Board board(fen);
    Move actual = ParseCoordinateNotation(move, board);
    REQUIRE(expected == actual);
}

void ParserTestRaise(std::string fen, std::string move)
{
    Board board(fen);
    REQUIRE_THROWS_AS(ParseCoordinateNotation(move, board), InvalidMoveNotationException);
}

/////////////////////////////////////////////////////////////////////////////////////////
//                        ParseCoordinateNotation tests                                //
/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("ParseCoordinateNotation_OO_CorrectMovedReturned")
{   
    ParserTest("r3k3/1pp1q1P1/Q7/2n3pP/3PR3/2N5/1PP5/rB2K2R w Kq g6 0 1",
               "O-O",
               NewCastlingMove(kE1, kG1, kWhiteKing, kKingSideCastle));
}

TEST_CASE("ParseCoordinateNotation_OOO_CorrectMovedReturned")
{
    ParserTest("r3k3/1pp1q1P1/Q7/2n3pP/3PR3/2N5/1PP5/rB2K2R b Kq g6 0 1",
               "O-O-O",
               NewCastlingMove(kE8, kC8, kBlackKing, kQueenSideCastle));
}

TEST_CASE("ParseCoordinateNotation_PawnTwoSquarePush_CorrectMoveReturned")
{
    ParserTest("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 b kq - 0 1",
               "c7c5",
               NewMove(kC7, kC5, kBlackPawn));
}

TEST_CASE("ParseCoordinateNotation_NormalRookMove_CorrectMoveReturned")
{
    ParserTest("r3k3/1pp1q1P1/Q7/2n3pP/3PR3/2N5/1PP5/rB2K2R w Kq g6 0 1",
               "e4e6",
               NewMove(kE4, kE6, kWhiteRook));
}

TEST_CASE("ParseCoordinateNotation_NormalKnightMove_CorrectMoveReturned")
{
    ParserTest("r3k3/1pp1q1P1/Q7/2n3pP/3PR3/2N5/1PP5/rB2K2R w Kq g6 0 1",
               "c3a4",
               NewMove(kC3, kA4, kWhiteKnight));
}

TEST_CASE("ParseCoordinateNotation_KnightCapture_CorrectMoveReturned")
{
    ParserTest("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 b kq - 0 1",
               "a5c4",
               NewMove(kA5, kC4, kBlackKnight, kWhitePawn));
}

TEST_CASE("ParseCoordinateNotation_PawnCapture_CorrectMoveReturned")
{
    ParserTest("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 b kq - 0 1",
               "g7h6",
               NewMove(kG7, kH6, kBlackPawn, kWhiteKnight));
}

TEST_CASE("ParseCoordinateNotation_PawnPromotion_CorrectMoveReturned")
{
    ParserTest("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 b kq - 0 1",
               "b2b1q",
               NewMove(kB2, kB1, kBlackPawn, kNoPiece, kBlackQueen));
}

TEST_CASE("ParseCoordinateNotation_PawnUnderPromotionAndCapture_CorrectMoveReturned")
{
    ParserTest("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 b kq - 0 1",
               "b2a1n",
               NewMove(kB2, kA1, kBlackPawn, kWhiteRook, kBlackKnight));
}

TEST_CASE("ParseCoordinateNotation_PriseEnPassant_CorrectMoveReturned")
{
    ParserTest("r3k2r/Ppp2ppp/1b3nbN/nP1pP3/BBP5/q4N2/Pp1P2PP/R2Q1RK1 w kq d5 0 1",
               "e5d6",
               NewMove(kE5, kD6, kWhitePawn, kBlackPawn));
}

TEST_CASE("ParseCoordinateNotation_InvalidFromColumn_ExceptionRaised")
{
    ParserTestRaise("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
                    "n2e3");
}

TEST_CASE("ParseCoordinateNotation_InvalidFromRow_ExceptionRaised")
{
    ParserTestRaise("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
                    "e9e3");
}

TEST_CASE("ParseCoordinateNotation_InvalidToColumn_ExceptionRaised")
{
    ParserTestRaise("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
                    "e2z3");
}

TEST_CASE("ParseCoordinateNotation_InvalidToRow_ExceptionRaised")
{
    ParserTestRaise("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
                    "e2eg");
}

TEST_CASE("ParseCoordinateNotation_InvalidPromotion_ExceptionRaised")
{
    ParserTestRaise("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 b kq - 0 1",
                    "b2b1h");
}

TEST_CASE("ParseCoordinateNotation_ExtraCharacterAfterPromotion_ExceptionRaised")
{
    ParserTestRaise("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 b kq - 0 1",
                    "b2b1qq");
}

TEST_CASE("ParseCoordinateNotation_NoPieceOnFrom_ExceptionRaised")
{
    ParserTestRaise("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
                    "e3e4");
}

/////////////////////////////////////////////////////////////////////////////////////////
//                           RenderCoordinateNotation tests                            //
/////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("RenderCoordinateNotation_KingSideCastling_CorrectStringReturned")
{
    Move move = NewMove(kE1, kG1, kWhiteKing, kNoPiece, kNoPiece, kKingSideCastle);
    std::string actual = RenderCoordinateNotation(move);
    REQUIRE("O-O" == actual);
}

TEST_CASE("RenderCoordinateNotation_QueenSideCastling_CorrectStringReturned")
{
    Move move = NewMove(kE1, kC1, kWhiteKing, kNoPiece, kNoPiece, kQueenSideCastle);
    std::string actual = RenderCoordinateNotation(move);
    REQUIRE("O-O-O" == actual);
}

TEST_CASE("RenderCoordinateNotation_SimpleKnightMove_CorrectStringReturned")
{
    Move move = NewMove(kB1, kC3, kWhiteKnight);
    std::string actual = RenderCoordinateNotation(move);
    REQUIRE("b1c3" == actual);
}

TEST_CASE("RenderCoordinateNotation_PawnPromotion_CorrectStringReturned")
{
    Move move = NewMove(kB7, kB8, kWhitePawn, kNoPiece, kRook);
    std::string actual = RenderCoordinateNotation(move);
    REQUIRE("b7b8r" == actual);
}