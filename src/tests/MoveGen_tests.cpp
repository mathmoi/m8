/// @file   MoveGen_tests.cpp
/// @author Mathieu Pagé
/// @date   December 2015
/// @brief  Contains tests of the types and functionalities in MoveGen.hpp.

#include <vector>

#include "catch.hpp"

#include "../m8chess/Board.hpp"
#include "../m8chess/MoveGen.hpp"
#include "../m8chess/Move.hpp"
#include "../m8common/Utils.hpp"

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

    std::vector<Move> expected_moves = {NewMove(kE5, kF7, kWhiteKnight),
                                        NewMove(kE5, kG6, kWhiteKnight),
                                        NewMove(kE5, kG4, kWhiteKnight),
                                        NewMove(kE5, kF3, kWhiteKnight),
                                        NewMove(kE5, kD3, kWhiteKnight),
                                        NewMove(kE5, kC4, kWhiteKnight),
                                        NewMove(kE5, kC6, kWhiteKnight),
                                        NewMove(kE5, kD7, kWhiteKnight)};

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

    std::vector<Move> expected_moves = { NewMove(kE5, kF7, kWhiteKnight),
                                         NewMove(kE5, kG6, kWhiteKnight),
                                         NewMove(kE5, kG4, kWhiteKnight),
                                         NewMove(kE5, kD3, kWhiteKnight),
                                         NewMove(kE5, kC4, kWhiteKnight),
                                         NewMove(kE5, kC6, kWhiteKnight),
                                         NewMove(kE5, kD7, kWhiteKnight) };

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
    REQUIRE(moves[0] == NewMove(kE5, kF3, kWhiteKnight, kBlackPawn));
}

TEST_CASE("GenerateKnighMoves_TwoKnightsOnBoard_EightMovesReturned")
{
    Board board("8/8/8/4N3/8/5p2/8/6N1 b - - 1 1");
    MoveList moves;
    Move* next_move = moves.data();

    std::vector<Move> expected_moves = { NewMove(kE5, kF7, kWhiteKnight),
                                         NewMove(kE5, kG6, kWhiteKnight),
                                         NewMove(kE5, kG4, kWhiteKnight),
                                         NewMove(kE5, kD3, kWhiteKnight),
                                         NewMove(kE5, kC4, kWhiteKnight),
                                         NewMove(kE5, kC6, kWhiteKnight),
                                         NewMove(kE5, kD7, kWhiteKnight),
                                         NewMove(kG1, kE2, kWhiteKnight),
                                         NewMove(kG1, kH3, kWhiteKnight)};

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

    std::vector<Move> expected_moves = { NewMove(kE5, kF3, kWhiteKnight, kBlackPawn),
                                         NewMove(kG1, kF3, kWhiteKnight, kBlackPawn) };

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
    Board board("4k3/8/8/8/8/8/3RRR2/3RKR2 b - - 1 1");
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
        NewMove(kE4, kD5, kWhitePawn, kBlackKnight)
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
        NewMove(kE4, kF5, kWhitePawn, kBlackKnight)
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
        NewMove(kB7, kB8, kWhitePawn, kNoPiece, kWhiteQueen),
        NewMove(kB7, kB8, kWhitePawn, kNoPiece, kWhiteRook),
        NewMove(kB7, kB8, kWhitePawn, kNoPiece, kWhiteKnight),
        NewMove(kB7, kB8, kWhitePawn, kNoPiece, kWhiteBishop)
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
        NewMove(kB7, kC8, kWhitePawn, kBlackRook, kWhiteQueen),
        NewMove(kB7, kC8, kWhitePawn, kBlackRook, kWhiteRook),
        NewMove(kB7, kC8, kWhitePawn, kBlackRook, kWhiteKnight),
        NewMove(kB7, kC8, kWhitePawn, kBlackRook, kWhiteBishop)
    };

    MoveGen move_gen(board);

    next_move = move_gen.GeneratePawnCaptures(kWhite, next_move);

    REQUIRE((next_move - moves.data()) == expected_moves.size());
    for (auto expected : expected_moves)
    {
        REQUIRE(Contains(moves.data(), next_move, expected));
    }
}

TEST_CASE("GenerateRookAttacks_called_ReturnExpectedValue")
{
    Bb occ = UINT64_C(0xFFDFEF20EFCFAF7F);
    Bb result;

    result = MoveGen::GenerateRookAttacks(occ, kE5);

    REQUIRE(UINT64_C(0x0010102F10101010) == result);
}

TEST_CASE("m8_rook_attacks__empty_board__return_expected_value")
{
    Bb occ = UINT64_C(0x0000000000000080);
    Bb result;

    result = MoveGen::GenerateRookAttacks(occ, kH1);

    REQUIRE(UINT64_C(0x808080808080807F) == result);
}

TEST_CASE("m8_bishop_attacks__called__return_expected_value")
{
    Bb occ = UINT64_C(0xFFDFEB20EFCFAF7F);
    Bb result;

    result = MoveGen::GenerateBishopAttacks(occ, kD5);

    REQUIRE(UINT64_C(0x4022140014204080) == result);
}

TEST_CASE("GenerateRookMoves__called_for_non_captures__return_all_non_captures_rook_moves")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    MoveList moves;
    Move* next_move = moves.data();

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

    MoveGen move_gen(board);

    next_move = move_gen.GenerateRookMoves(kBlack, false, next_move);

    REQUIRE((next_move - moves.data()) == expected_moves.size());
    for (auto expected : expected_moves)
    {
        REQUIRE(Contains(moves.data(), next_move, expected));
    }
}

TEST_CASE("GenerateRookMoves__called_for_captures__return_all_captures_rook_moves")
{
    Board board("r3k2r/Q1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q2/PPPBBPPP/R3K2R w KQkq - 0 1");
    MoveList moves;
    Move* next_move = moves.data();

    std::vector<Move> expected_moves =
    {
        NewMove(kA8, kA7, kBlackRook, kWhiteQueen),
        NewMove(kH8, kH2, kBlackRook, kWhitePawn),
    };

    MoveGen move_gen(board);

    next_move = move_gen.GenerateRookMoves(kBlack, true, next_move);

    REQUIRE((next_move - moves.data()) == expected_moves.size());
    for (auto expected : expected_moves)
    {
        REQUIRE(Contains(moves.data(), next_move, expected));
    }
}

TEST_CASE("GenerateBishopMoves__called_for_non_captures__return_all_non_captures_bishop_moves")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    MoveList moves;
    Move* next_move = moves.data();

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

    MoveGen move_gen(board);

    next_move = move_gen.GenerateBishopMoves(kBlack, false, next_move);

    REQUIRE((next_move - moves.data()) == expected_moves.size());
    for (auto expected : expected_moves)
    {
        REQUIRE(Contains(moves.data(), next_move, expected));
    }
}

TEST_CASE("GenerateBishopMoves__called_for_captures__return_all_captures_bishop_moves")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    MoveList moves;
    Move* next_move = moves.data();

    std::vector<Move> expected_moves =
    {
        NewMove(kA6, kE2, kBlackBishop, kWhiteBishop)
    };

    MoveGen move_gen(board);

    next_move = move_gen.GenerateBishopMoves(kBlack, true, next_move);

    REQUIRE((next_move - moves.data()) == expected_moves.size());
    for (auto expected : expected_moves)
    {
        REQUIRE(Contains(moves.data(), next_move, expected));
    }
}

TEST_CASE("GenerateQueenMoves__called_for_non_captures__return_all_non_captures_queen_moves")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    MoveList moves;
    Move* next_move = moves.data();

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

    MoveGen move_gen(board);

    next_move = move_gen.GenerateQueenMoves(kWhite, false, next_move);

    REQUIRE((next_move - moves.data()) == expected_moves.size());
    for (auto expected : expected_moves)
    {
        REQUIRE(Contains(moves.data(), next_move, expected));
    }
}

TEST_CASE("GenerateQueenMoves__called_for_captures__return_all_captures_queen_moves")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    MoveList moves;
    Move* next_move = moves.data();

    std::vector<Move> expected_moves =
    {
        NewMove(kF3, kF6, kWhiteQueen, kBlackKnight),
        NewMove(kF3, kH3, kWhiteQueen, kBlackPawn)
    };

    MoveGen move_gen(board);

    next_move = move_gen.GenerateQueenMoves(kWhite, true, next_move);

    REQUIRE((next_move - moves.data()) == expected_moves.size());
    for (auto expected : expected_moves)
    {
        REQUIRE(Contains(moves.data(), next_move, expected));
    }
}

TEST_CASE("AttacksTo__lots_of_squares_attack_e5__all_attacks_identified")
{
    Board board("4k3/2bnqn2/2n2pn1/1Q5r/2NP2N1/3N1N2/4R2B/4K3 w - - 0 1");

    MoveGen move_gen(board);

    Bb attacks = move_gen.AttacksTo(kE5);

    REQUIRE(attacks == UINT64_C(0x003C64824C289000));
}

TEST_CASE("AttacksTo__lots_of_squares_attack_e5__nothings_attack_a1")
{
    Board board("4k3/2bnqn2/2n2pn1/1Q5r/2NP2N1/3N1N2/4R2B/4K3 w - - 0 1");

    MoveGen move_gen(board);

    Bb attacks = move_gen.AttacksTo(kA1);

    REQUIRE(attacks == Bb::Empty());
}

TEST_CASE("AttacksTo__both_kings_attack_e5__both_attacks_identified")
{
    Board board("8/8/4k3/8/4K3/8/8/8 w - - 0 1");

    MoveGen move_gen(board);

    Bb attacks = move_gen.AttacksTo(kE5);

    REQUIRE(attacks == UINT64_C(0x0000100010000000));
}

TEST_CASE("IsInCheck__white_king_attack_by_rook__return_true")
{
    Board board("8/8/4k3/8/r3K2R/8/8/8 w - - 0 1");
    MoveGen move_gen(board);

    bool in_check = move_gen.IsInCheck(kWhite);

    REQUIRE(in_check == true);
}

TEST_CASE("IsInCheck__black_king_is_not_in_check__return_false")
{
    Board board("8/8/4k3/8/r3K2R/8/8/8 w - - 0 1");
    MoveGen move_gen(board);

    bool in_check = move_gen.IsInCheck(kBlack);

    REQUIRE(in_check == false);
}

TEST_CASE("IsInCheck__white_king_is_protected_by_own_piece__return_false")
{
    Board board("8/8/4k3/8/r2QK2R/8/8/8 w - - 0 1");
    MoveGen move_gen(board);

    bool in_check = move_gen.IsInCheck(kWhite);

    REQUIRE(in_check == false);
}

TEST_CASE("GenerateCastlingMoves__chess960_position__correct_moves_including_castling returned")
{
    Board board("rk2r3/8/8/8/8/8/4P3/RK2R3 w KQkq - 0 1");
    MoveList moves;
    Move* next_move = moves.data();

    std::vector<Move> expected_moves =
    {
        NewMove(kB1, kA2, kWhiteKing),
        NewMove(kB1, kB2, kWhiteKing),
        NewMove(kB1, kC2, kWhiteKing),
        NewMove(kB1, kC1, kWhiteKing),
        NewCastlingMove(kB1, kG1, kWhiteKing, kKingSideCastle),
        NewCastlingMove(kB1, kC1, kWhiteKing, kQueenSideCastle)
    };

    MoveGen move_gen(board);

    next_move = move_gen.GenerateKingMoves(kWhite, false, next_move);

    REQUIRE((next_move - moves.data()) == expected_moves.size());
    for (auto expected : expected_moves)
    {
        REQUIRE(Contains(moves.data(), next_move, expected));
    }
}

TEST_CASE("GenerateCastlingMoves__position_traversed_by_rook_attacked__castling_possible")
{
    Board board("1k6/8/1r6/8/8/8/8/R3K3 w Q - 0 1");
    MoveList moves;
    Move* next_move = moves.data();

    Move expected_move = NewCastlingMove(kE1, kC1, kWhiteKing, kQueenSideCastle);
    
    MoveGen move_gen(board);

    next_move = move_gen.GenerateKingMoves(kWhite, false, next_move);

    REQUIRE(Contains(moves.data(), next_move, expected_move));
}

TEST_CASE("GenerateCastlingMoves__position_traversed_by_king_attacked__castling_impossible")
{
    Board board("1k6/8/3r4/8/8/8/8/R3K3 w Q - 0 1");
    MoveList moves;
    Move* next_move = moves.data();

    Move unexpected_move = NewCastlingMove(kE1, kC1, kWhiteKing, kQueenSideCastle);

    MoveGen move_gen(board);

    next_move = move_gen.GenerateKingMoves(kWhite, false, next_move);

    REQUIRE((next_move - moves.data()) == 5);
    REQUIRE(!Contains(moves.data(), next_move, unexpected_move));
}

TEST_CASE("GenerateCastlingMoves__positions_traveled_by_rook_occupied__castling_impossible")
{
    Board board("1k6/8/8/8/8/8/8/RB2K3 w Q - 0 1");
    MoveList moves;
    Move* next_move = moves.data();

    Move unexpected_move = NewCastlingMove(kE1, kC1, kWhiteKing, kQueenSideCastle);

    MoveGen move_gen(board);

    next_move = move_gen.GenerateKingMoves(kWhite, false, next_move);

    REQUIRE((next_move - moves.data()) == 5);
    REQUIRE(!Contains(moves.data(), next_move, unexpected_move));
}