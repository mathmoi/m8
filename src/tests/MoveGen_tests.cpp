/// @file   MoveGen_tests.cpp
/// @author Mathieu Pagé
/// @date   December 2015
/// @brief  Contains tests of the types and functionalities in MoveGen.hpp.

#include <vector>

#include "catch.hpp"

#include "../common/Board.hpp"
#include "../common/MoveGen.hpp"
#include "../common/Move.hpp"
#include "../common/Utils.hpp"

using namespace m8;

TEST_CASE("GenerateKnighMoves_NoKnightOfTheColor_ZeroMovesReturned")
{
    Board board("8/8/8/4N3/8/8/8/8 w - - 1 1 ");
    MoveList moves;
    Move* next_move = moves.data();

    MoveGen move_gen(board);

    next_move = move_gen.GenerateKnightMoves(kBlack, false, next_move);

    REQUIRE((next_move - moves.data()) == 0);
}

TEST_CASE("GenerateKnighMoves_NoPossibleMoves_ZeroMovesReturned")
{
    Board board("4k3/8/8/8/8/6R1/5R2/4K2N b - - 1 1");
    MoveList moves;
    Move* next_move = moves.data();

    MoveGen move_gen(board);

    next_move = move_gen.GenerateKnightMoves(kWhite, false, next_move);

    REQUIRE((next_move - moves.data()) == 0);
}

TEST_CASE("GenerateKnighMoves_SingleKnightMidleOfBoard_EightMovesReturned")
{
    Board board("8/8/8/4N3/8/8/8/8 w - - 1 1 ");
    MoveList moves;
    Move* next_move = moves.data();

    std::vector<Move> expected_moves = {NewMove(kE5, kF7, kWhiteKnigt),
                                        NewMove(kE5, kG6, kWhiteKnigt),
                                        NewMove(kE5, kG4, kWhiteKnigt),
                                        NewMove(kE5, kF3, kWhiteKnigt),
                                        NewMove(kE5, kD3, kWhiteKnigt),
                                        NewMove(kE5, kC4, kWhiteKnigt),
                                        NewMove(kE5, kC6, kWhiteKnigt),
                                        NewMove(kE5, kD7, kWhiteKnigt)};

    MoveGen move_gen(board);

    next_move = move_gen.GenerateKnightMoves(kWhite, false, next_move);

    REQUIRE((next_move - moves.data()) == 8);
    for (auto expected : expected_moves)
    {
        REQUIRE(Contains(moves.data(), next_move, expected));
    }
}

TEST_CASE("GenerateKnighMoves_SingleKnightMidleOfBoardOneMoveObstructed_SevenMovesReturned")
{
    Board board("8/8/8/4N3/8/5p2/8/8 b - - 1 1");
    MoveList moves;
    Move* next_move = moves.data();

    std::vector<Move> expected_moves = { NewMove(kE5, kF7, kWhiteKnigt),
                                         NewMove(kE5, kG6, kWhiteKnigt),
                                         NewMove(kE5, kG4, kWhiteKnigt),
                                         NewMove(kE5, kD3, kWhiteKnigt),
                                         NewMove(kE5, kC4, kWhiteKnigt),
                                         NewMove(kE5, kC6, kWhiteKnigt),
                                         NewMove(kE5, kD7, kWhiteKnigt) };

    MoveGen move_gen(board);

    next_move = move_gen.GenerateKnightMoves(kWhite, false, next_move);

    REQUIRE((next_move - moves.data()) == 7);
    for (auto expected : expected_moves)
    {
        REQUIRE(Contains(moves.data(), next_move, expected));
    }
}

TEST_CASE("GenerateKnighCaptures_SingleKnightMidleOfBoardOnePawnToCapture_OneMoveReturned")
{
    Board board("8/8/8/4N3/8/5p2/8/8 b - - 1 1");
    MoveList moves;
    Move* next_move = moves.data();

    MoveGen move_gen(board);

    next_move = move_gen.GenerateKnightMoves(kWhite, true, next_move);

    REQUIRE((next_move - moves.data()) == 1);
    REQUIRE(moves[0] == NewMove(kE5, kF3, kWhiteKnigt, kBlackPawn));
}

TEST_CASE("GenerateKnighMoves_TwoKnightsOnBoard_EightMovesReturned")
{
    Board board("8/8/8/4N3/8/5p2/8/6N1 b - - 1 1");
    MoveList moves;
    Move* next_move = moves.data();

    std::vector<Move> expected_moves = { NewMove(kE5, kF7, kWhiteKnigt),
                                         NewMove(kE5, kG6, kWhiteKnigt),
                                         NewMove(kE5, kG4, kWhiteKnigt),
                                         NewMove(kE5, kD3, kWhiteKnigt),
                                         NewMove(kE5, kC4, kWhiteKnigt),
                                         NewMove(kE5, kC6, kWhiteKnigt),
                                         NewMove(kE5, kD7, kWhiteKnigt),
                                         NewMove(kG1, kE2, kWhiteKnigt),
                                         NewMove(kG1, kH3, kWhiteKnigt)};

    MoveGen move_gen(board);

    next_move = move_gen.GenerateKnightMoves(kWhite, false, next_move);

    REQUIRE((next_move - moves.data()) == 9);
    for (auto expected : expected_moves)
    {
        REQUIRE(Contains(moves.data(), next_move, expected));
    }
}

TEST_CASE("GenerateKnighMoves_TwoKnightsOnBoardAbleToCaptureSinglePawn_TwoCapturesReturned")
{
    Board board("8/8/8/4N3/8/5p2/8/6N1 b - - 1 1");
    MoveList moves;
    Move* next_move = moves.data();

    std::vector<Move> expected_moves = { NewMove(kE5, kF3, kWhiteKnigt, kBlackPawn),
                                         NewMove(kG1, kF3, kWhiteKnigt, kBlackPawn) };

    MoveGen move_gen(board);

    next_move = move_gen.GenerateKnightMoves(kWhite, true, next_move);

    REQUIRE((next_move - moves.data()) == 2);
    for (auto expected : expected_moves)
    {
        REQUIRE(Contains(moves.data(), next_move, expected));
    }
}

TEST_CASE("GenerateKingMoves_NoKing_ZeroMovesReturned")
{
    Board board("8/8/8/8/8/K7/8/8 w - - 1 1 ");
    MoveList moves;
    Move* next_move = moves.data();

    MoveGen move_gen(board);

    next_move = move_gen.GenerateKingMoves(kBlack, false, next_move);

    REQUIRE((next_move - moves.data()) == 0);
}

TEST_CASE("GenerateKingMoves_AllMovesBlockedByOwnPiece_ZeroMovesReturned")
{
    Board board("4k3/8/8/8/8/8/3RRR2/3RKR2 b KQ - 1 1");
    MoveList moves;
    Move* next_move = moves.data();

    MoveGen move_gen(board);

    next_move = move_gen.GenerateKingMoves(kWhite, false, next_move);

    REQUIRE((next_move - moves.data()) == 0);
}

TEST_CASE("GenerateKingMoves_KingMidleBoardOneCapturePossible_SevenMovesReturned")
{
    Board board("4k3/8/8/8/8/4Kp2/8/8 b - - 1 1");
    MoveList moves;
    Move* next_move = moves.data();

    std::vector<Move> expected_moves = 
      { NewMove(kE3, kE4, kWhiteKing),
        NewMove(kE3, kF4, kWhiteKing),
        NewMove(kE3, kF2, kWhiteKing),
        NewMove(kE3, kE2, kWhiteKing),
        NewMove(kE3, kD2, kWhiteKing),
        NewMove(kE3, kD3, kWhiteKing),
        NewMove(kE3, kD4, kWhiteKing)};

    MoveGen move_gen(board);

    next_move = move_gen.GenerateKingMoves(kWhite, false, next_move);

    REQUIRE((next_move - moves.data()) == 7);
    for (auto expected : expected_moves)
    {
        REQUIRE(Contains(moves.data(), next_move, expected));
    }
}

TEST_CASE("GenerateKingMoves_KingMidleBoardOneCapturePossible_SingleCaptureReturned")
{
    Board board("4k3/8/8/8/8/4Kp2/8/8 b - - 1 1");
    MoveList moves;
    Move* next_move = moves.data();

    std::vector<Move> expected_moves =
    { NewMove(kE3, kF3, kWhiteKing, kBlackPawn)};

    MoveGen move_gen(board);

    next_move = move_gen.GenerateKingMoves(kWhite, true, next_move);

    REQUIRE((next_move - moves.data()) == expected_moves.size());
    for (auto expected : expected_moves)
    {
        REQUIRE(Contains(moves.data(), next_move, expected));
    }
}

TEST_CASE("GeneratePawnMoves_NoPawnsOfTheRequestedColor_NoMoves")
{
    Board board("3k4/3p4/8/8/8/8/8/3K4 w - - 0 1");
    MoveList moves;
    Move* next_move = moves.data();

    std::vector<Move> expected_moves = {};

    MoveGen move_gen(board);

    next_move = move_gen.GeneratePawnMoves(kWhite, next_move);

    REQUIRE((next_move - moves.data()) == expected_moves.size());
    for (auto expected : expected_moves)
    {
        REQUIRE(Contains(moves.data(), next_move, expected));
    }
}

TEST_CASE("GeneratePawnMoves_TwoPawnsThatCanMoveForward_TwoMoves")
{
    Board board("4k3/8/8/8/5P2/3P4/8/4K3 w - - 0 1");
    MoveList moves;
    Move* next_move = moves.data();

    std::vector<Move> expected_moves = 
    {
        NewMove(kD3, kD4, kWhitePawn),
        NewMove(kF4, kF5, kWhitePawn)
    };

    MoveGen move_gen(board);

    next_move = move_gen.GeneratePawnMoves(kWhite, next_move);

    REQUIRE((next_move - moves.data()) == expected_moves.size());
    for (auto expected : expected_moves)
    {
        REQUIRE(Contains(moves.data(), next_move, expected));
    }
}

TEST_CASE("GeneratePawnMoves_BlockedPawn_NoMoves")
{
    Board board("4k3/8/8/8/3R4/3P4/8/4K3 w - - 0 1");
    MoveList moves;
    Move* next_move = moves.data();

    std::vector<Move> expected_moves = {};

    MoveGen move_gen(board);

    next_move = move_gen.GeneratePawnMoves(kWhite, next_move);

    REQUIRE((next_move - moves.data()) == expected_moves.size());
}

TEST_CASE("GeneratePawnMoves_PawnOnStartingRow_TwoMoves")
{
    Board board("4k3/3p4/8/8/8/8/8/4K3 w - - 0 1");
    MoveList moves;
    Move* next_move = moves.data();

    std::vector<Move> expected_moves =
    {
        NewMove(kD7, kD6, kBlackPawn),
        NewMove(kD7, kD5, kBlackPawn)
    };

    MoveGen move_gen(board);

    next_move = move_gen.GeneratePawnMoves(kBlack, next_move);

    REQUIRE((next_move - moves.data()) == expected_moves.size());
    for (auto expected : expected_moves)
    {
        REQUIRE(Contains(moves.data(), next_move, expected));
    }
}

TEST_CASE("GeneratePawnMoves_PawnOnStartingRowDoubleMoveBlocked_OneMoves")
{
    Board board("4k3/3p4/8/3n4/8/8/8/4K3 w - - 0 1");
    MoveList moves;
    Move* next_move = moves.data();

    std::vector<Move> expected_moves =
    {
        NewMove(kD7, kD6, kBlackPawn)
    };

    MoveGen move_gen(board);

    next_move = move_gen.GeneratePawnMoves(kBlack, next_move);

    REQUIRE((next_move - moves.data()) == expected_moves.size());
    for (auto expected : expected_moves)
    {
        REQUIRE(Contains(moves.data(), next_move, expected));
    }
}

TEST_CASE("GeneratePawnMoves_PawnOnStartingRowBlocked_NoMoves")
{
    Board board("4k3/3p4/3n4/8/8/8/8/4K3 w - - 0 1");
    MoveList moves;
    Move* next_move = moves.data();

    std::vector<Move> expected_moves = {};

    MoveGen move_gen(board);

    next_move = move_gen.GeneratePawnMoves(kBlack, next_move);

    REQUIRE((next_move - moves.data()) == expected_moves.size());
    for (auto expected : expected_moves)
    {
        REQUIRE(Contains(moves.data(), next_move, expected));
    }
}

TEST_CASE("GeneratePawnCaptures_NoPawnOfTheRequestedColor_NoMoves")
{
    Board board("4k3/8/8/3n4/8/8/8/4K3 b - - 0 1");
    MoveList moves;
    Move* next_move = moves.data();

    MoveGen move_gen(board);

    next_move = move_gen.GeneratePawnCaptures(kBlack, next_move);

    REQUIRE((next_move - moves.data()) == 0);
}

TEST_CASE("GeneratePawnCaptures_OneCapturesAvailableOnLeft_OneMovesReturned")
{
    Board board("4k3/8/8/3n4/4P3/8/8/4K3 w - - 0 1 ");
    MoveList moves;
    Move* next_move = moves.data();

    std::vector<Move> expected_moves =
    {
        NewMove(kE4, kD5, kWhitePawn, kBlackKnigt)
    };

    MoveGen move_gen(board);

    next_move = move_gen.GeneratePawnCaptures(kWhite, next_move);

    REQUIRE((next_move - moves.data()) == expected_moves.size());
    for (auto expected : expected_moves)
    {
        REQUIRE(Contains(moves.data(), next_move, expected));
    }
}

TEST_CASE("GeneratePawnCaptures_OneCapturesAvailableOnRight_OneMovesReturned")
{
    Board board("4k3/8/8/5n2/4P3/8/8/4K3 w - - 0 1 ");
    MoveList moves;
    Move* next_move = moves.data();

    std::vector<Move> expected_moves =
    {
        NewMove(kE4, kF5, kWhitePawn, kBlackKnigt)
    };

    MoveGen move_gen(board);

    next_move = move_gen.GeneratePawnCaptures(kWhite, next_move);

    REQUIRE((next_move - moves.data()) == expected_moves.size());
    for (auto expected : expected_moves)
    {
        REQUIRE(Contains(moves.data(), next_move, expected));
    }
}

TEST_CASE("GeneratePawnCaptures_OnePawnInPositionToPromote_FourMovesReturned")
{
    Board board("4k3/1P6/8/8/8/8/8/4K3 w - - 0 1");
    MoveList moves;
    Move* next_move = moves.data();

    std::vector<Move> expected_moves =
    {
        NewMove(kB7, kB8, kWhitePawn, kNoPiece, kBlackQueen),
        NewMove(kB7, kB8, kWhitePawn, kNoPiece, kBlackRook),
        NewMove(kB7, kB8, kWhitePawn, kNoPiece, kBlackKnigt),
        NewMove(kB7, kB8, kWhitePawn, kNoPiece, kBlackBishp)
    };

    MoveGen move_gen(board);

    next_move = move_gen.GeneratePawnCaptures(kWhite, next_move);

    REQUIRE((next_move - moves.data()) == expected_moves.size());
    for (auto expected : expected_moves)
    {
        REQUIRE(Contains(moves.data(), next_move, expected));
    }
}

TEST_CASE("GeneratePawnCaptures_OnePawnInPositionToCaptureAndPromote_FourMovesReturned")
{
    Board board("1Rr1k3/1P6/8/8/8/8/8/4K3 w - - 0 1 ");
    MoveList moves;
    Move* next_move = moves.data();

    std::vector<Move> expected_moves =
    {
        NewMove(kB7, kC8, kWhitePawn, kBlackRook, kBlackQueen),
        NewMove(kB7, kC8, kWhitePawn, kBlackRook, kBlackRook),
        NewMove(kB7, kC8, kWhitePawn, kBlackRook, kBlackKnigt),
        NewMove(kB7, kC8, kWhitePawn, kBlackRook, kBlackBishp)
    };

    MoveGen move_gen(board);

    next_move = move_gen.GeneratePawnCaptures(kWhite, next_move);

    REQUIRE((next_move - moves.data()) == expected_moves.size());
    for (auto expected : expected_moves)
    {
        REQUIRE(Contains(moves.data(), next_move, expected));
    }
}