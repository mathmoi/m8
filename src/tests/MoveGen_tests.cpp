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

    std::vector<Move> expected_moves = {NewMove(Sq::E5(), Sq::F7(), kWhiteKnight),
                                        NewMove(Sq::E5(), Sq::G6(), kWhiteKnight),
                                        NewMove(Sq::E5(), Sq::G4(), kWhiteKnight),
                                        NewMove(Sq::E5(), Sq::F3(), kWhiteKnight),
                                        NewMove(Sq::E5(), Sq::D3(), kWhiteKnight),
                                        NewMove(Sq::E5(), Sq::C4(), kWhiteKnight),
                                        NewMove(Sq::E5(), Sq::C6(), kWhiteKnight),
                                        NewMove(Sq::E5(), Sq::D7(), kWhiteKnight)};

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

    std::vector<Move> expected_moves = { NewMove(Sq::E5(), Sq::F7(), kWhiteKnight),
                                         NewMove(Sq::E5(), Sq::G6(), kWhiteKnight),
                                         NewMove(Sq::E5(), Sq::G4(), kWhiteKnight),
                                         NewMove(Sq::E5(), Sq::D3(), kWhiteKnight),
                                         NewMove(Sq::E5(), Sq::C4(), kWhiteKnight),
                                         NewMove(Sq::E5(), Sq::C6(), kWhiteKnight),
                                         NewMove(Sq::E5(), Sq::D7(), kWhiteKnight) };

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
    REQUIRE(moves[0] == NewMove(Sq::E5(), Sq::F3(), kWhiteKnight, kBlackPawn));
}

TEST_CASE("GenerateKnighMoves_TwoKnightsOnBoard_EightMovesReturned")
{
    Board board("8/8/8/4N3/8/5p2/8/6N1 b - - 1 1");
    MoveList moves;
    Move* next_move = moves.data();

    std::vector<Move> expected_moves = { NewMove(Sq::E5(), Sq::F7(), kWhiteKnight),
                                         NewMove(Sq::E5(), Sq::G6(), kWhiteKnight),
                                         NewMove(Sq::E5(), Sq::G4(), kWhiteKnight),
                                         NewMove(Sq::E5(), Sq::D3(), kWhiteKnight),
                                         NewMove(Sq::E5(), Sq::C4(), kWhiteKnight),
                                         NewMove(Sq::E5(), Sq::C6(), kWhiteKnight),
                                         NewMove(Sq::E5(), Sq::D7(), kWhiteKnight),
                                         NewMove(Sq::G1(), Sq::E2(), kWhiteKnight),
                                         NewMove(Sq::G1(), Sq::H3(), kWhiteKnight)};

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

    std::vector<Move> expected_moves = { NewMove(Sq::E5(), Sq::F3(), kWhiteKnight, kBlackPawn),
                                         NewMove(Sq::G1(), Sq::F3(), kWhiteKnight, kBlackPawn) };

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
      { NewMove(Sq::E3(), Sq::E4(), kWhiteKing),
        NewMove(Sq::E3(), Sq::F4(), kWhiteKing),
        NewMove(Sq::E3(), Sq::F2(), kWhiteKing),
        NewMove(Sq::E3(), Sq::E2(), kWhiteKing),
        NewMove(Sq::E3(), Sq::D2(), kWhiteKing),
        NewMove(Sq::E3(), Sq::D3(), kWhiteKing),
        NewMove(Sq::E3(), Sq::D4(), kWhiteKing)};

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
    { NewMove(Sq::E3(), Sq::F3(), kWhiteKing, kBlackPawn)};

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
        NewMove(Sq::D3(), Sq::D4(), kWhitePawn),
        NewMove(Sq::F4(), Sq::F5(), kWhitePawn)
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
        NewMove(Sq::D7(), Sq::D6(), kBlackPawn),
        NewMove(Sq::D7(), Sq::D5(), kBlackPawn)
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
        NewMove(Sq::D7(), Sq::D6(), kBlackPawn)
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
        NewMove(Sq::E4(), Sq::D5(), kWhitePawn, kBlackKnight)
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
        NewMove(Sq::E4(), Sq::F5(), kWhitePawn, kBlackKnight)
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
        NewMove(Sq::B7(), Sq::B8(), kWhitePawn, kNoPiece, kWhiteQueen),
        NewMove(Sq::B7(), Sq::B8(), kWhitePawn, kNoPiece, kWhiteRook),
        NewMove(Sq::B7(), Sq::B8(), kWhitePawn, kNoPiece, kWhiteKnight),
        NewMove(Sq::B7(), Sq::B8(), kWhitePawn, kNoPiece, kWhiteBishop)
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
        NewMove(Sq::B7(), Sq::C8(), kWhitePawn, kBlackRook, kWhiteQueen),
        NewMove(Sq::B7(), Sq::C8(), kWhitePawn, kBlackRook, kWhiteRook),
        NewMove(Sq::B7(), Sq::C8(), kWhitePawn, kBlackRook, kWhiteKnight),
        NewMove(Sq::B7(), Sq::C8(), kWhitePawn, kBlackRook, kWhiteBishop)
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

    result = MoveGen::GenerateRookAttacks(occ, Sq::E5());

    REQUIRE(UINT64_C(0x0010102F10101010) == result);
}

TEST_CASE("m8_rook_attacks__empty_board__return_expected_value")
{
    Bb occ = UINT64_C(0x0000000000000080);
    Bb result;

    result = MoveGen::GenerateRookAttacks(occ, Sq::H1());

    REQUIRE(UINT64_C(0x808080808080807F) == result);
}

TEST_CASE("m8_bishop_attacks__called__return_expected_value")
{
    Bb occ = UINT64_C(0xFFDFEB20EFCFAF7F);
    Bb result;

    result = MoveGen::GenerateBishopAttacks(occ, Sq::D5());

    REQUIRE(UINT64_C(0x4022140014204080) == result);
}

TEST_CASE("GenerateRookMoves__called_for_non_captures__return_all_non_captures_rook_moves")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    MoveList moves;
    Move* next_move = moves.data();

    std::vector<Move> expected_moves =
    {
        NewMove(Sq::A8(), Sq::B8(), kBlackRook),
        NewMove(Sq::A8(), Sq::C8(), kBlackRook),
        NewMove(Sq::A8(), Sq::D8(), kBlackRook),
        NewMove(Sq::H8(), Sq::F8(), kBlackRook),
        NewMove(Sq::H8(), Sq::G8(), kBlackRook),
        NewMove(Sq::H8(), Sq::H7(), kBlackRook),
        NewMove(Sq::H8(), Sq::H6(), kBlackRook),
        NewMove(Sq::H8(), Sq::H5(), kBlackRook),
        NewMove(Sq::H8(), Sq::H4(), kBlackRook),
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
        NewMove(Sq::A8(), Sq::A7(), kBlackRook, kWhiteQueen),
        NewMove(Sq::H8(), Sq::H2(), kBlackRook, kWhitePawn),
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
        NewMove(Sq::A6(), Sq::B7(), kBlackBishop),
        NewMove(Sq::A6(), Sq::C8(), kBlackBishop),
        NewMove(Sq::A6(), Sq::B5(), kBlackBishop),
        NewMove(Sq::A6(), Sq::C4(), kBlackBishop),
        NewMove(Sq::A6(), Sq::D3(), kBlackBishop),
        NewMove(Sq::G7(), Sq::F8(), kBlackBishop),
        NewMove(Sq::G7(), Sq::H6(), kBlackBishop)
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
        NewMove(Sq::A6(), Sq::E2(), kBlackBishop, kWhiteBishop)
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
        NewMove(Sq::F3(), Sq::D3(), kWhiteQueen),
        NewMove(Sq::F3(), Sq::E3(), kWhiteQueen),
        NewMove(Sq::F3(), Sq::G3(), kWhiteQueen),
        NewMove(Sq::F3(), Sq::F4(), kWhiteQueen),
        NewMove(Sq::F3(), Sq::F5(), kWhiteQueen),
        NewMove(Sq::F3(), Sq::G4(), kWhiteQueen),
        NewMove(Sq::F3(), Sq::H5(), kWhiteQueen),
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
        NewMove(Sq::F3(), Sq::F6(), kWhiteQueen, kBlackKnight),
        NewMove(Sq::F3(), Sq::H3(), kWhiteQueen, kBlackPawn)
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

    Bb attacks = move_gen.AttacksTo(Sq::E5());

    REQUIRE(attacks == UINT64_C(0x003C64824C289000));
}

TEST_CASE("AttacksTo__lots_of_squares_attack_e5__nothings_attack_a1")
{
    Board board("4k3/2bnqn2/2n2pn1/1Q5r/2NP2N1/3N1N2/4R2B/4K3 w - - 0 1");

    MoveGen move_gen(board);

    Bb attacks = move_gen.AttacksTo(Sq::A1());

    REQUIRE(attacks == Bb::Empty());
}

TEST_CASE("AttacksTo__both_kings_attack_e5__both_attacks_identified")
{
    Board board("8/8/4k3/8/4K3/8/8/8 w - - 0 1");

    MoveGen move_gen(board);

    Bb attacks = move_gen.AttacksTo(Sq::E5());

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
        NewMove(Sq::B1(), Sq::A2(), kWhiteKing),
        NewMove(Sq::B1(), Sq::B2(), kWhiteKing),
        NewMove(Sq::B1(), Sq::C2(), kWhiteKing),
        NewMove(Sq::B1(), Sq::C1(), kWhiteKing),
        NewCastlingMove(Sq::B1(), Sq::G1(), kWhiteKing, kKingSideCastle),
        NewCastlingMove(Sq::B1(), Sq::C1(), kWhiteKing, kQueenSideCastle)
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

    Move expected_move = NewCastlingMove(Sq::E1(), Sq::C1(), kWhiteKing, kQueenSideCastle);
    
    MoveGen move_gen(board);

    next_move = move_gen.GenerateKingMoves(kWhite, false, next_move);

    REQUIRE(Contains(moves.data(), next_move, expected_move));
}

TEST_CASE("GenerateCastlingMoves__position_traversed_by_king_attacked__castling_impossible")
{
    Board board("1k6/8/3r4/8/8/8/8/R3K3 w Q - 0 1");
    MoveList moves;
    Move* next_move = moves.data();

    Move unexpected_move = NewCastlingMove(Sq::E1(), Sq::C1(), kWhiteKing, kQueenSideCastle);

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

    Move unexpected_move = NewCastlingMove(Sq::E1(), Sq::C1(), kWhiteKing, kQueenSideCastle);

    MoveGen move_gen(board);

    next_move = move_gen.GenerateKingMoves(kWhite, false, next_move);

    REQUIRE((next_move - moves.data()) == 5);
    REQUIRE(!Contains(moves.data(), next_move, unexpected_move));
}