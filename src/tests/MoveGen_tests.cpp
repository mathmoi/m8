/// @file   MoveGen_tests.cpp
/// @author Mathieu Pag�
/// @date   December 2015
/// @brief  Contains tests of the types and functionalities in MoveGen.hpp.

#include <vector>

#include "catch.hpp"

#include "../m8chess/movegen/MoveGeneration.hpp"

#include "../m8chess/Board.hpp"
#include "../m8chess/Move.hpp"
#include "../m8common/Utils.hpp"

using namespace m8;
using namespace m8::movegen;

inline bool Contains(Move expected, const MoveList& actual_moves)
{
    return actual_moves.end() != std::find_if(actual_moves.begin(),
                                              actual_moves.end(),
                                              [expected](const MoveEvalPair& pair){return pair.move == expected;});
}

inline void RequireSameMoves(const std::vector<Move>& expected_moves, const MoveList& actual_moves)
{
    REQUIRE (expected_moves.size() == actual_moves.size());

    for (auto expected_move : expected_moves)
    {
        REQUIRE(Contains(expected_move, actual_moves));
    }
}

TEST_CASE("GenerateKnighMoves_NoKnightOfTheColor_ZeroMovesReturned")
{
    Board board("8/8/8/4N3/8/8/8/8 w - - 1 1 ");
    MoveList moves;
    
    GenerateKnightMoves(board, kBlack, false, moves);

    REQUIRE(!moves.any());
}

TEST_CASE("GenerateKnighMoves_NoPossibleMoves_ZeroMovesReturned")
{
    Board board("4k3/8/8/8/8/6R1/5R2/4K2N b - - 1 1");
    MoveList moves;

    GenerateKnightMoves(board, kWhite, false, moves);

    REQUIRE(!moves.any());
}

TEST_CASE("GenerateKnighMoves_SingleKnightMidleOfBoard_EightMovesReturned")
{
    Board board("8/8/8/4N3/8/8/8/8 w - - 1 1 ");
    MoveList moves;

    std::vector<Move> expected_moves = {NewMove(kE5, kF7, kWhiteKnight),
                                        NewMove(kE5, kG6, kWhiteKnight),
                                        NewMove(kE5, kG4, kWhiteKnight),
                                        NewMove(kE5, kF3, kWhiteKnight),
                                        NewMove(kE5, kD3, kWhiteKnight),
                                        NewMove(kE5, kC4, kWhiteKnight),
                                        NewMove(kE5, kC6, kWhiteKnight),
                                        NewMove(kE5, kD7, kWhiteKnight)};

    GenerateKnightMoves(board, kWhite, false, moves);

    RequireSameMoves(expected_moves, moves);
}

TEST_CASE("GenerateKnighMoves_SingleKnightMidleOfBoardOneMoveObstructed_SevenMovesReturned")
{
    Board board("8/8/8/4N3/8/5p2/8/8 b - - 1 1");
    MoveList moves;

    std::vector<Move> expected_moves = { NewMove(kE5, kF7, kWhiteKnight),
                                         NewMove(kE5, kG6, kWhiteKnight),
                                         NewMove(kE5, kG4, kWhiteKnight),
                                         NewMove(kE5, kD3, kWhiteKnight),
                                         NewMove(kE5, kC4, kWhiteKnight),
                                         NewMove(kE5, kC6, kWhiteKnight),
                                         NewMove(kE5, kD7, kWhiteKnight) };

    GenerateKnightMoves(board, kWhite, false, moves);

    RequireSameMoves(expected_moves, moves);
}

TEST_CASE("GenerateKnighCaptures_SingleKnightMidleOfBoardOnePawnToCapture_OneMoveReturned")
{
    Board board("8/8/8/4N3/8/5p2/8/8 b - - 1 1");
    MoveList moves;

    std::vector<Move> expected_moves = { NewMove(kE5, kF3, kWhiteKnight, kBlackPawn) };

    GenerateKnightMoves(board, kWhite, true, moves);

    RequireSameMoves(expected_moves, moves);
}

TEST_CASE("GenerateKnighMoves_TwoKnightsOnBoard_EightMovesReturned")
{
    Board board("8/8/8/4N3/8/5p2/8/6N1 b - - 1 1");
    MoveList moves;

    std::vector<Move> expected_moves = { NewMove(kE5, kF7, kWhiteKnight),
                                         NewMove(kE5, kG6, kWhiteKnight),
                                         NewMove(kE5, kG4, kWhiteKnight),
                                         NewMove(kE5, kD3, kWhiteKnight),
                                         NewMove(kE5, kC4, kWhiteKnight),
                                         NewMove(kE5, kC6, kWhiteKnight),
                                         NewMove(kE5, kD7, kWhiteKnight),
                                         NewMove(kG1, kE2, kWhiteKnight),
                                         NewMove(kG1, kH3, kWhiteKnight)};

    GenerateKnightMoves(board, kWhite, false, moves);

    RequireSameMoves(expected_moves, moves);
}

TEST_CASE("GenerateKnighMoves_TwoKnightsOnBoardAbleToCaptureSinglePawn_TwoCapturesReturned")
{
    Board board("8/8/8/4N3/8/5p2/8/6N1 b - - 1 1");
    MoveList moves;

    std::vector<Move> expected_moves = { NewMove(kE5, kF3, kWhiteKnight, kBlackPawn),
                                         NewMove(kG1, kF3, kWhiteKnight, kBlackPawn) };

    GenerateKnightMoves(board, kWhite, true, moves);

    RequireSameMoves(expected_moves, moves);
}

TEST_CASE("GenerateKingMoves_NoKing_ZeroMovesReturned")
{
    Board board("8/8/8/8/8/K7/8/8 w - - 1 1 ");
    MoveList moves;

    GenerateKingMoves(board, kBlack, false, moves);

    REQUIRE(!moves.any());
}

TEST_CASE("GenerateKingMoves_AllMovesBlockedByOwnPiece_ZeroMovesReturned")
{
    Board board("4k3/8/8/8/8/8/3RRR2/3RKR2 b - - 1 1");
    MoveList moves;

    GenerateKingMoves(board, kWhite, false, moves);

    REQUIRE(!moves.any());
}

TEST_CASE("GenerateKingMoves_KingMidleBoardOneCapturePossible_SevenMovesReturned")
{
    Board board("4k3/8/8/8/8/4Kp2/8/8 b - - 1 1");
    MoveList moves;

    std::vector<Move> expected_moves = 
      { NewMove(kE3, kE4, kWhiteKing),
        NewMove(kE3, kF4, kWhiteKing),
        NewMove(kE3, kF2, kWhiteKing),
        NewMove(kE3, kE2, kWhiteKing),
        NewMove(kE3, kD2, kWhiteKing),
        NewMove(kE3, kD3, kWhiteKing),
        NewMove(kE3, kD4, kWhiteKing)};

    GenerateKingMoves(board, kWhite, false, moves);

    RequireSameMoves(expected_moves, moves);
}

TEST_CASE("GenerateKingMoves_KingMidleBoardOneCapturePossible_SingleCaptureReturned")
{
    Board board("4k3/8/8/8/8/4Kp2/8/8 b - - 1 1");
    MoveList moves;

    std::vector<Move> expected_moves =
    { NewMove(kE3, kF3, kWhiteKing, kBlackPawn)};

    GenerateKingMoves(board, kWhite, true, moves);

    RequireSameMoves(expected_moves, moves);
}

TEST_CASE("GeneratePawnMoves_NoPawnsOfTheRequestedColor_NoMoves")
{
    Board board("3k4/3p4/8/8/8/8/8/3K4 w - - 0 1");
    MoveList moves;

    std::vector<Move> expected_moves = {};

    GeneratePawnMoves(board, kWhite, moves);

    RequireSameMoves(expected_moves, moves);
}

TEST_CASE("GeneratePawnMoves_TwoPawnsThatCanMoveForward_TwoMoves")
{
    Board board("4k3/8/8/8/5P2/3P4/8/4K3 w - - 0 1");
    MoveList moves;

    std::vector<Move> expected_moves = 
    {
        NewMove(kD3, kD4, kWhitePawn),
        NewMove(kF4, kF5, kWhitePawn)
    };

    GeneratePawnMoves(board, kWhite, moves);

    RequireSameMoves(expected_moves, moves);
}

TEST_CASE("GeneratePawnMoves_BlockedPawn_NoMoves")
{
    Board board("4k3/8/8/8/3R4/3P4/8/4K3 w - - 0 1");
    MoveList moves;

    std::vector<Move> expected_moves = {};

    GeneratePawnMoves(board, kWhite, moves);

    RequireSameMoves(expected_moves, moves);
}

TEST_CASE("GeneratePawnMoves_PawnOnStartingRow_TwoMoves")
{
    Board board("4k3/3p4/8/8/8/8/8/4K3 w - - 0 1");
    MoveList moves;

    std::vector<Move> expected_moves =
    {
        NewMove(kD7, kD6, kBlackPawn),
        NewMove(kD7, kD5, kBlackPawn)
    };

    GeneratePawnMoves(board, kBlack, moves);

    RequireSameMoves(expected_moves, moves);
}

TEST_CASE("GeneratePawnMoves_PawnOnStartingRowDoubleMoveBlocked_OneMoves")
{
    Board board("4k3/3p4/8/3n4/8/8/8/4K3 w - - 0 1");
    MoveList moves;

    std::vector<Move> expected_moves =
    {
        NewMove(kD7, kD6, kBlackPawn)
    };

    GeneratePawnMoves(board, kBlack, moves);

    RequireSameMoves(expected_moves, moves);
}

TEST_CASE("GeneratePawnMoves_PawnOnStartingRowBlocked_NoMoves")
{
    Board board("4k3/3p4/3n4/8/8/8/8/4K3 w - - 0 1");
    MoveList moves;

    std::vector<Move> expected_moves = {};

    GeneratePawnMoves(board, kBlack, moves);

    RequireSameMoves(expected_moves, moves);
}

TEST_CASE("GeneratePawnCaptures_NoPawnOfTheRequestedColor_NoMoves")
{
    Board board("4k3/8/8/3n4/8/8/8/4K3 b - - 0 1");
    MoveList moves;

    GeneratePawnCaptures(board, kBlack, moves);

    REQUIRE(!moves.any());
}

TEST_CASE("GeneratePawnCaptures_OneCapturesAvailableOnLeft_OneMovesReturned")
{
    Board board("4k3/8/8/3n4/4P3/8/8/4K3 w - - 0 1 ");
    MoveList moves;

    std::vector<Move> expected_moves =
    {
        NewMove(kE4, kD5, kWhitePawn, kBlackKnight)
    };

    GeneratePawnCaptures(board, kWhite, moves);

    RequireSameMoves(expected_moves, moves);
}

TEST_CASE("GeneratePawnCaptures_OneCapturesAvailableOnRight_OneMovesReturned")
{
    Board board("4k3/8/8/5n2/4P3/8/8/4K3 w - - 0 1 ");
    MoveList moves;

    std::vector<Move> expected_moves =
    {
        NewMove(kE4, kF5, kWhitePawn, kBlackKnight)
    };

    GeneratePawnCaptures(board, kWhite, moves);

    RequireSameMoves(expected_moves, moves);
}

TEST_CASE("GeneratePawnCaptures_OnePawnInPositionToPromote_FourMovesReturned")
{
    Board board("4k3/1P6/8/8/8/8/8/4K3 w - - 0 1");
    MoveList moves;

    std::vector<Move> expected_moves =
    {
        NewMove(kB7, kB8, kWhitePawn, kNoPiece, kWhiteQueen),
        NewMove(kB7, kB8, kWhitePawn, kNoPiece, kWhiteRook),
        NewMove(kB7, kB8, kWhitePawn, kNoPiece, kWhiteKnight),
        NewMove(kB7, kB8, kWhitePawn, kNoPiece, kWhiteBishop)
    };

    GeneratePawnCaptures(board, kWhite, moves);

    RequireSameMoves(expected_moves, moves);
}

TEST_CASE("GeneratePawnCaptures_OnePawnInPositionToCaptureAndPromote_FourMovesReturned")
{
    Board board("1Rr1k3/1P6/8/8/8/8/8/4K3 w - - 0 1 ");
    MoveList moves;

    std::vector<Move> expected_moves =
    {
        NewMove(kB7, kC8, kWhitePawn, kBlackRook, kWhiteQueen),
        NewMove(kB7, kC8, kWhitePawn, kBlackRook, kWhiteRook),
        NewMove(kB7, kC8, kWhitePawn, kBlackRook, kWhiteKnight),
        NewMove(kB7, kC8, kWhitePawn, kBlackRook, kWhiteBishop)
    };

    GeneratePawnCaptures(board, kWhite, moves);

    RequireSameMoves(expected_moves, moves);
}

TEST_CASE("GeneratePawnCaptures_PriseEnPassantPossible_PriseEnPassantReturned")
{
    Board board("rnbqkbnr/p2ppppp/8/1Pp5/8/8/1PPPPPPP/RNBQKBNR w KQkq c6 0 1");
    MoveList moves;

    Move expected = NewMove(kB5, kC6, kWhitePawn, kBlackPawn);

    GeneratePawnCaptures(board, kWhite, moves);

    REQUIRE(Contains(expected, moves));
}

TEST_CASE("GenerateRookMoves__called_for_non_captures__return_all_non_captures_rook_moves")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    MoveList moves;

    std::vector<Move> expected_moves =
    {
        NewMove(kA8, kB8, kBlackRook),
        NewMove(kA8, kC8, kBlackRook),
        NewMove(kA8, kD8, kBlackRook),
        NewMove(kH8, kF8, kBlackRook),
        NewMove(kH8, kG8, kBlackRook),
        NewMove(kH8, kH7, kBlackRook),
        NewMove(kH8, kH6, kBlackRook),
        NewMove(kH8, kH5, kBlackRook),
        NewMove(kH8, kH4, kBlackRook),
    };

    GenerateRookMoves(board, kBlack, false, moves);

    RequireSameMoves(expected_moves, moves);
}

TEST_CASE("GenerateRookMoves__called_for_captures__return_all_captures_rook_moves")
{
    Board board("r3k2r/Q1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q2/PPPBBPPP/R3K2R w KQkq - 0 1");
    MoveList moves;

    std::vector<Move> expected_moves =
    {
        NewMove(kA8, kA7, kBlackRook, kWhiteQueen),
        NewMove(kH8, kH2, kBlackRook, kWhitePawn),
    };

    GenerateRookMoves(board, kBlack, true, moves);

    RequireSameMoves(expected_moves, moves);
}

TEST_CASE("GenerateBishopMoves__called_for_non_captures__return_all_non_captures_bishop_moves")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    MoveList moves;

    std::vector<Move> expected_moves =
    {
        NewMove(kA6, kB7, kBlackBishop),
        NewMove(kA6, kC8, kBlackBishop),
        NewMove(kA6, kB5, kBlackBishop),
        NewMove(kA6, kC4, kBlackBishop),
        NewMove(kA6, kD3, kBlackBishop),
        NewMove(kG7, kF8, kBlackBishop),
        NewMove(kG7, kH6, kBlackBishop)
    };

    GenerateBishopMoves(board, kBlack, false, moves);

    RequireSameMoves(expected_moves, moves);
}

TEST_CASE("GenerateBishopMoves__CaptureOnTheCornerOfBoard__CaptureInCornerReturned")
{
    Board board("rnbqkbnr/1ppppp1p/p5p1/8/8/1P6/PBPPPPPP/RN1QKBNR w KQkq - 0 1");
    MoveList moves;

    Move expected_move = NewMove(kB2, kH8, kWhiteBishop, kBlackRook);

    GenerateBishopMoves(board, kWhite, true, moves);

    REQUIRE(Contains(expected_move, moves));
}

TEST_CASE("GenerateBishopMoves__called_for_captures__return_all_captures_bishop_moves")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    MoveList moves;

    std::vector<Move> expected_moves =
    {
        NewMove(kA6, kE2, kBlackBishop, kWhiteBishop)
    };

    GenerateBishopMoves(board, kBlack, true, moves);

    RequireSameMoves(expected_moves, moves);
}

TEST_CASE("GenerateQueenMoves__called_for_non_captures__return_all_non_captures_queen_moves")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    MoveList moves;

    std::vector<Move> expected_moves =
    {
        NewMove(kF3, kD3, kWhiteQueen),
        NewMove(kF3, kE3, kWhiteQueen),
        NewMove(kF3, kG3, kWhiteQueen),
        NewMove(kF3, kF4, kWhiteQueen),
        NewMove(kF3, kF5, kWhiteQueen),
        NewMove(kF3, kG4, kWhiteQueen),
        NewMove(kF3, kH5, kWhiteQueen),
    };

    GenerateQueenMoves(board, kWhite, false, moves);

    RequireSameMoves(expected_moves, moves);
}

TEST_CASE("GenerateQueenMoves__called_for_captures__return_all_captures_queen_moves")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    MoveList moves;

    std::vector<Move> expected_moves =
    {
        NewMove(kF3, kF6, kWhiteQueen, kBlackKnight),
        NewMove(kF3, kH3, kWhiteQueen, kBlackPawn)
    };

    GenerateQueenMoves(board, kWhite, true, moves);

    RequireSameMoves(expected_moves, moves);
}

TEST_CASE("AttacksTo__lots_of_squares_attack_e5__all_attacks_identified")
{
    Board board("4k3/2bnqn2/2n2pn1/1Q5r/2NP2N1/3N1N2/4R2B/4K3 w - - 0 1");

    Bb attacks = AttacksTo(board, kE5);

    REQUIRE(attacks == BB_C(0x003C64824C289000));
}

TEST_CASE("AttacksTo__lots_of_squares_attack_e5__nothings_attack_a1")
{
    Board board("4k3/2bnqn2/2n2pn1/1Q5r/2NP2N1/3N1N2/4R2B/4K3 w - - 0 1");

    Bb attacks = AttacksTo(board, kA1);

    REQUIRE(attacks == kEmptyBb);
}

TEST_CASE("AttacksTo__both_kings_attack_e5__both_attacks_identified")
{
    Board board("8/8/4k3/8/4K3/8/8/8 w - - 0 1");

    Bb attacks = AttacksTo(board, kE5);

    REQUIRE(attacks == BB_C(0x0000100010000000));
}

TEST_CASE("GenerateCastlingMoves__chess960_position__correct_moves_including_castling returned")
{
    Board board("rk2r3/8/8/8/8/8/4P3/RK2R3 w KQkq - 0 1");
    MoveList moves;

    std::vector<Move> expected_moves =
    {
        NewMove(kB1, kA2, kWhiteKing),
        NewMove(kB1, kB2, kWhiteKing),
        NewMove(kB1, kC2, kWhiteKing),
        NewMove(kB1, kC1, kWhiteKing),
        NewCastlingMove(kB1, kG1, kWhiteKing, kKingSideCastle),
        NewCastlingMove(kB1, kC1, kWhiteKing, kQueenSideCastle)
    };

    GenerateKingMoves(board, kWhite, false, moves);

    RequireSameMoves(expected_moves, moves);
}

TEST_CASE("GenerateCastlingMoves__position_traversed_by_rook_attacked__castling_possible")
{
    Board board("1k6/8/1r6/8/8/8/8/R3K3 w Q - 0 1");
    MoveList moves;

    Move expected_move = NewCastlingMove(kE1, kC1, kWhiteKing, kQueenSideCastle);

    GenerateKingMoves(board, kWhite, false, moves);

    REQUIRE(Contains(expected_move, moves));
}

TEST_CASE("GenerateCastlingMoves__position_traversed_by_king_attacked__castling_impossible")
{
    Board board("1k6/8/3r4/8/8/8/8/R3K3 w Q - 0 1");
    MoveList moves;

    Move unexpected_move = NewCastlingMove(kE1, kC1, kWhiteKing, kQueenSideCastle);

    GenerateKingMoves(board, kWhite, false, moves);

    REQUIRE(5 == moves.size());
    REQUIRE(!Contains(unexpected_move, moves));
}

TEST_CASE("GenerateCastlingMoves__positions_traveled_by_rook_occupied__castling_impossible")
{
    Board board("1k6/8/8/8/8/8/8/RB2K3 w Q - 0 1");
    MoveList moves;

    Move unexpected_move = NewCastlingMove(kE1, kC1, kWhiteKing, kQueenSideCastle);

    GenerateKingMoves(board, kWhite, false, moves);

    REQUIRE(5 == moves.size());
    REQUIRE(!Contains(unexpected_move, moves));
}