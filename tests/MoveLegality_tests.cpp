/// @file MoveLegality_tests.cpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date August 2023

#include "catch2/catch_all.hpp"

#include "m8chess/MoveLegality.hpp"

using namespace m8;

TEST_CASE("IsPseudoLegal_WrongSideOnMove_ReturnsFalse")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
    Move move = NewMove(kF6, kG4, kBlackKnight);
    bool expected = false;

    bool actual = IsPseudoLegal(board, move);

    REQUIRE(expected == actual);
}

TEST_CASE("IsPseudoLegal_WrongPieceOnSquare_ReturnsFalse")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
    Move move = NewMove(kF3, kG3, kWhiteRook);
    bool expected = false;

    bool actual = IsPseudoLegal(board, move);

    REQUIRE(expected == actual);
}

TEST_CASE("IsPseudoLegal_NoPieceOnSquare_ReturnsFalse")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
    Move move = NewMove(kE3, kD3, kWhiteRook);
    bool expected = false;

    bool actual = IsPseudoLegal(board, move);

    REQUIRE(expected == actual);
}

TEST_CASE("IsPseudoLegal_PieceTakenNotOnTarget_ReturnsFalse")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
    Move move = NewMove(kA2, kB3, kWhitePawn, kBlackBishop);
    bool expected = false;

    bool actual = IsPseudoLegal(board, move);

    REQUIRE(expected == actual);
}

TEST_CASE("IsPseudoLegal_WrongPieceOnTarget_ReturnsFalse")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
    Move move = NewMove(kG2, kH3, kWhitePawn, kBlackBishop);
    bool expected = false;

    bool actual = IsPseudoLegal(board, move);

    REQUIRE(expected == actual);
}

TEST_CASE("IsPseudoLegal_IllegalCastling_ReturnsFalse")
{
    Board board(kStartingPositionFEN);
    Move move = kNullMove;
    bool expected = false;

    SECTION ("White to move")
    {
        board = Board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w - - ");

        SECTION ("White castle queen side") { move = NewMove(kE1, kC1, kWhiteKing, kNoPiece, kNoPiece, kQueenSideCastle); }
        SECTION ("White castle king  side") { move = NewMove(kE1, kG1, kWhiteKing, kNoPiece, kNoPiece, kKingSideCastle); }
    }

    SECTION ("Black to move")
    {
        board = Board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b - - ");

        SECTION ("Black castle queen side") { move = NewMove(kE8, kC8, kBlackKing, kNoPiece, kNoPiece, kQueenSideCastle); }
        SECTION ("Black castle king  side") { move = NewMove(kE8, kG8, kBlackKing, kNoPiece, kNoPiece, kKingSideCastle); }
    }

    bool actual = IsPseudoLegal(board, move);

    REQUIRE(expected == actual);
}

TEST_CASE("IsPseudoLegal_CastlingWhileInCheck_ReturnsFalse")
{
    Board board("4k3/8/8/8/1q6/8/8/R3K2R w KQ - 0 1");
    Move move = NewMove(kE1, kG1, kWhiteKing, kNoPiece, kNoPiece, kKingSideCastle);;
    bool expected = false;

    bool actual = IsPseudoLegal(board, move);

    REQUIRE(expected == actual);
}

TEST_CASE("IsPseudoLegal_CastlingWhileTravelingSquareAttacked_ReturnsFalse")
{
    Board board("3rk3/8/8/8/8/8/8/R3K2R w KQ - 0 1");
    Move move = NewMove(kE1, kC1, kWhiteKing, kNoPiece, kNoPiece, kQueenSideCastle);
    bool expected = false;

    bool actual = IsPseudoLegal(board, move);

    REQUIRE(expected == actual);
}

TEST_CASE("IsPseudoLegal_CastlingWhileTargetSquareAttacked_ReturnsFalse")
{
    Board board("2r1k3/8/8/8/8/8/8/R3K2R w KQ - 0 1");
    Move move = NewMove(kE1, kC1, kWhiteKing, kNoPiece, kNoPiece, kQueenSideCastle);
    bool expected = false;

    bool actual = IsPseudoLegal(board, move);

    REQUIRE(expected == actual);
}

TEST_CASE("IsPseudoLegal_CastlingWhileTravelSquareOccupied_ReturnsFalse")
{
    Board board("4k3/8/8/8/8/8/8/R2QK2R w KQ - 0 1");
    Move move = NewMove(kE1, kC1, kWhiteKing, kNoPiece, kNoPiece, kQueenSideCastle);
    bool expected = false;

    bool actual = IsPseudoLegal(board, move);

    REQUIRE(expected == actual);
}

TEST_CASE("IsPseudoLegal_PawnTwoMoveWhileSquareInFrontOccupied_ReturnsFalse")
{
    Board board("4k3/8/8/8/8/5p2/5P2/4K3 w - - 0 1");
    Move move = NewMove(kF2, kF4, kWhitePawn);
    bool expected = false;

    bool actual = IsPseudoLegal(board, move);

    REQUIRE(expected == actual);
}

TEST_CASE("IsPseudoLegal_PawnCapturesForward_ReturnsFalse")
{
    Board board("4k3/8/8/8/8/5p2/5P2/4K3 w - - 0 1");
    Move move = NewMove(kF2, kF4, kWhitePawn, kBlackPawn);
    bool expected = false;

    bool actual = IsPseudoLegal(board, move);

    REQUIRE(expected == actual);
}

TEST_CASE("IsPseudoLegal_SliderMoveThroughOccupiedSq_ReturnsFalse")
{
    Board board("4k3/8/8/3p4/8/8/8/3QK3 w - - 0 1");
    Move move = NewMove(kD1, kD6, kWhiteQueen);
    bool expected = false;

    bool actual = IsPseudoLegal(board, move);

    REQUIRE(expected == actual);
}

TEST_CASE("IsPseudoLegal_PriseEnPassantWhenNotAllowed_ReturnsFalse")
{
    Board board("4k3/8/8/4Pp2/8/8/8/4K3 w - - 0 1");
    Move move = NewMove(kE5, kF6, kWhitePawn, kBlackPawn);
    bool expected = false;

    bool actual = IsPseudoLegal(board, move);

    REQUIRE(expected == actual);
}

TEST_CASE("IsPseudoLegal_SimpleLegalMove_ReturnsTrue")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
    Move move = NewMove(kC3, kA4, kWhiteKnight);
    bool expected = true;

    bool actual = IsPseudoLegal(board, move);

    REQUIRE(expected == actual);
}

TEST_CASE("IsPseudoLegal_PawnCapture_ReturnsTrue")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
    Move move = NewMove(kG2, kH3, kWhitePawn, kBlackPawn);
    bool expected = true;

    bool actual = IsPseudoLegal(board, move);

    REQUIRE(expected == actual);
}

TEST_CASE("IsPseudoLegal_SimpleCaptures_ReturnsTrue")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
    Move move = NewMove(kF3, kH3, kWhiteQueen, kBlackPawn);
    bool expected = true;

    bool actual = IsPseudoLegal(board, move);

    REQUIRE(expected == actual);
}

TEST_CASE("IsPseudoLegal_OneSquarePawnMove_ReturnsTrue")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
    Move move = NewMove(kA2, kA3, kWhitePawn);
    bool expected = true;

    bool actual = IsPseudoLegal(board, move);

    REQUIRE(expected == actual);
}

TEST_CASE("IsPseudoLegal_TwoSquarePawnMove_ReturnsTrue")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
    Move move = NewMove(kA2, kA4, kWhitePawn);
    bool expected = true;

    bool actual = IsPseudoLegal(board, move);

    REQUIRE(expected == actual);
}

TEST_CASE("IsPseudoLegal_PawnPromotion_ReturnsTrue")
{
    Board board("4k3/6P1/8/8/8/8/8/4K3 w - - 0 1");
    Move move = NewMove(kG7, kG8, kWhitePawn, kNoPiece, kWhiteQueen);
    bool expected = true;

    bool actual = IsPseudoLegal(board, move);

    REQUIRE(expected == actual);
}

TEST_CASE("IsPseudoLegal_PawnCapturePromotion_ReturnsTrue")
{
    Board board("4k2r/6P1/8/8/8/8/8/4K3 w - - 0 1");
    Move move = NewMove(kG7, kH8, kWhitePawn, kBlackRook, kWhiteQueen);
    bool expected = true;

    bool actual = IsPseudoLegal(board, move);

    REQUIRE(expected == actual);
}

TEST_CASE("IsPseudoLegal_PriseEnPassant_ReturnsTrue")
{
    Board board("4k3/8/8/4Pp2/8/8/8/4K3 w - f5 0 1");
    Move move = NewMove(kE5, kF6, kWhitePawn, kBlackPawn);
    bool expected = true;

    bool actual = IsPseudoLegal(board, move);

    REQUIRE(expected == actual);
}

TEST_CASE("IsPseudoLegal_LegalCastling_ReturnsTrue")
{
    Board board(kStartingPositionFEN);
    Move move = kNullMove;
    bool expected = true;

    SECTION ("White to move")
    {
        board = Board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");

        SECTION ("White castle queen side") { move = NewMove(kE1, kC1, kWhiteKing, kNoPiece, kNoPiece, kQueenSideCastle); }
        SECTION ("White castle king  side") { move = NewMove(kE1, kG1, kWhiteKing, kNoPiece, kNoPiece, kKingSideCastle); }
    }

    SECTION ("Black to move")
    {
        board = Board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq - ");

        SECTION ("Black castle queen side") { move = NewMove(kE8, kC8, kBlackKing, kNoPiece, kNoPiece, kQueenSideCastle); }
        SECTION ("Black castle king  side") { move = NewMove(kE8, kG8, kBlackKing, kNoPiece, kNoPiece, kKingSideCastle); }
    }

    bool actual = IsPseudoLegal(board, move);

    REQUIRE(expected == actual);
}

TEST_CASE("IsPseudoLegal_LegalFischerCastling_ReturnsTrue")
{
    Board board(kStartingPositionFEN);
    Move move = kNullMove;
    bool expected = true;

    SECTION ("White to move")
    {
        board = Board("rk2r3/4p3/8/8/8/8/8/RK2R3 w AEae - 0 1");

        SECTION ("White castle queen side") { move = NewMove(kB1, kC1, kWhiteKing, kNoPiece, kNoPiece, kQueenSideCastle); }
        SECTION ("White castle king  side") { move = NewMove(kB1, kG1, kWhiteKing, kNoPiece, kNoPiece, kKingSideCastle); }
    }

    SECTION ("Black to move")
    {
        board = Board("rk2r3/4p3/8/8/8/8/8/RK2R3 b AEae - 0 1");

        SECTION ("Black castle queen side") { move = NewMove(kB8, kC8, kBlackKing, kNoPiece, kNoPiece, kQueenSideCastle); }
        SECTION ("Black castle king  side") { move = NewMove(kB8, kG8, kBlackKing, kNoPiece, kNoPiece, kKingSideCastle); }
    }

    bool actual = IsPseudoLegal(board, move);

    REQUIRE(expected == actual);
}