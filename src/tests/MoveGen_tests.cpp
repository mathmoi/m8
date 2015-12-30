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
