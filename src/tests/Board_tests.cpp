/// @file   Board_tests.cpp
/// @author Mathieu Pag�
/// @date   Feburary 2015
/// @brief  Contains tests of the Board type and functionalities.

#include "catch.hpp"

#include <cstdint>

#include "../m8chess/Board.hpp"
#include "../m8chess/Move.hpp"

using namespace m8;
using namespace std;

TEST_CASE("Test Board()")
{
   Board board;

   REQUIRE(board.side_to_move() == Color::White());

   for (Sq sq = Sq::A1(); sq.IsOnBoard(); sq = sq.MoveNext())
   {
      REQUIRE(board[sq] == kNoPiece);
   }

   for (Color color = Color::First(); color.IsColor(); color =  color.Next())
   {
      for (PieceType piece_type = kMinPieceType; IsPieceType(piece_type); ++piece_type)
      {
         REQUIRE(board.bb_piece(NewPiece(piece_type, color)) == Bb::Empty());
      }
   }

   REQUIRE(board.UINT64_Color(Color::White()) == Bb::Empty());
   REQUIRE(board.UINT64_Color(Color::Black()) == Bb::Empty());

   REQUIRE(board.casle(Color::White(), kQueenSideCastle) == false);
   REQUIRE(board.casle(Color::White(), kKingSideCastle) == false);
   REQUIRE(board.casle(Color::Black(), kQueenSideCastle) == false);
   REQUIRE(board.casle(Color::Black(), kKingSideCastle) == false);

   REQUIRE(board.colmn_enpas().IsOnBoard() == false);
}

TEST_CASE("Test Board(fen)")
{
   // Initial postion test
   Board board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

   REQUIRE(board[Sq::A8()] == kBlackRook);
   REQUIRE(board[Sq::B8()] == kBlackKnight);
   REQUIRE(board[Sq::C8()] == kBlackBishop);
   REQUIRE(board[Sq::D8()] == kBlackQueen);
   REQUIRE(board[Sq::E8()] == kBlackKing);
   REQUIRE(board[Sq::F8()] == kBlackBishop);
   REQUIRE(board[Sq::G8()] == kBlackKnight);
   REQUIRE(board[Sq::H8()] == kBlackRook);

   REQUIRE(board[Sq::A7()] == kBlackPawn);
   REQUIRE(board[Sq::B7()] == kBlackPawn);
   REQUIRE(board[Sq::C7()] == kBlackPawn);
   REQUIRE(board[Sq::D7()] == kBlackPawn);
   REQUIRE(board[Sq::E7()] == kBlackPawn);
   REQUIRE(board[Sq::F7()] == kBlackPawn);
   REQUIRE(board[Sq::G7()] == kBlackPawn);
   REQUIRE(board[Sq::H7()] == kBlackPawn);

   for (Row row = Row::_6(); row >= Row::_3(); row = row.MoveDown())
   {
      for (Column colmn = Column::A(); colmn.IsOnBoard(); colmn = colmn.MoveRight())
      {
         Sq sq(colmn, row);
         REQUIRE(board[sq] == kNoPiece);
      }
   }

   REQUIRE(board[Sq::A2()] == kWhitePawn);
   REQUIRE(board[Sq::B2()] == kWhitePawn);
   REQUIRE(board[Sq::C2()] == kWhitePawn);
   REQUIRE(board[Sq::D2()] == kWhitePawn);
   REQUIRE(board[Sq::E2()] == kWhitePawn);
   REQUIRE(board[Sq::F2()] == kWhitePawn);
   REQUIRE(board[Sq::G2()] == kWhitePawn);
   REQUIRE(board[Sq::H2()] == kWhitePawn);

   REQUIRE(board[Sq::A1()] == kWhiteRook);
   REQUIRE(board[Sq::B1()] == kWhiteKnight);
   REQUIRE(board[Sq::C1()] == kWhiteBishop);
   REQUIRE(board[Sq::D1()] == kWhiteQueen);
   REQUIRE(board[Sq::E1()] == kWhiteKing);
   REQUIRE(board[Sq::F1()] == kWhiteBishop);
   REQUIRE(board[Sq::G1()] == kWhiteKnight);
   REQUIRE(board[Sq::H1()] == kWhiteRook);

   REQUIRE(board.side_to_move() == Color::White());

   REQUIRE(board.casle(Color::White(), kQueenSideCastle) == true);
   REQUIRE(board.casle(Color::White(), kKingSideCastle) == true);
   REQUIRE(board.casle(Color::Black(), kQueenSideCastle) == true);
   REQUIRE(board.casle(Color::Black(), kKingSideCastle) == true);

   REQUIRE(board.casle_colmn(0) == Column::A());
   REQUIRE(board.casle_colmn(1) == Column::H());

   REQUIRE(board.colmn_enpas() == Column::Invalid());
}

TEST_CASE("Test Board(fen) 2")
{
   // Chess360 castling test
   Board board("rn2k1r1/ppp1pp1p/3p2p1/5bn1/P7/2N2B2/1PPPPP2/2BNK1RR w Gkq - 4 11");

   REQUIRE(board.casle(Color::White(), kQueenSideCastle) == false);
   REQUIRE(board.casle(Color::White(), kKingSideCastle) == true);
   REQUIRE(board.casle(Color::Black(), kQueenSideCastle) == true);
   REQUIRE(board.casle(Color::Black(), kKingSideCastle) == true);

   REQUIRE(board.casle_colmn(0) == Column::A());
   REQUIRE(board.casle_colmn(1) == Column::G());

   REQUIRE(board.half_move_clock() == 4);
}

TEST_CASE("Test Board(fen) 3")
{
   // Chess360 castling test
   Board board("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");

   REQUIRE(board.colmn_enpas() == Column::E());
}

TEST_CASE("Test Board(fen) 4")
{
   // Chess360 castling test
   Board board("rn2k1r1/ppp1pp1p/3p2p1/5bn1/P7/2N2B2/1PPPPP2/2BNK1RR w");

   REQUIRE(board.side_to_move() == Color::White());
}

TEST_CASE("Test side_to_move()")
{
   Board board;

   REQUIRE(board.side_to_move() == Color::White());

   board.set_side_to_move(Color::White());
   REQUIRE(board.side_to_move() == Color::White());

   board.set_side_to_move(Color::Black());
   REQUIRE(board.side_to_move() == Color::Black());

   board.set_side_to_move(Color::Black());
   REQUIRE(board.side_to_move() == Color::Black());

   board.set_side_to_move(Color::White());
   REQUIRE(board.side_to_move() == Color::White());
}

TEST_CASE("Test colmn_enpas()")
{
   Board board;

   REQUIRE(!board.colmn_enpas().IsOnBoard());

   board.set_colmn_enpas(Column::D());
   REQUIRE(board.colmn_enpas() == Column::D());

   board.set_colmn_enpas(Column::Invalid());
   REQUIRE(!board.colmn_enpas().IsOnBoard());
}

TEST_CASE("Test casle()")
{
   Board board;

   SECTION("white queen side castle")
   {
      board.set_casle(Color::White(), kQueenSideCastle, false);
      REQUIRE(board.casle(Color::White(), kQueenSideCastle) == false);

      board.set_casle(Color::White(), kQueenSideCastle, false);
      REQUIRE(board.casle(Color::White(), kQueenSideCastle) == false);
      REQUIRE(board.casle(Color::White(), kKingSideCastle) == false);
      REQUIRE(board.casle(Color::Black(), kQueenSideCastle) == false);
      REQUIRE(board.casle(Color::Black(), kKingSideCastle) == false);

      board.set_casle(Color::White(), kQueenSideCastle, true);
      REQUIRE(board.casle(Color::White(), kQueenSideCastle) == true);
      REQUIRE(board.casle(Color::White(), kKingSideCastle) == false);
      REQUIRE(board.casle(Color::Black(), kQueenSideCastle) == false);
      REQUIRE(board.casle(Color::Black(), kKingSideCastle) == false);
   }

   SECTION("white king side castle")
   {
      board.set_casle(Color::White(), kKingSideCastle, false);
      REQUIRE(board.casle(Color::White(), kKingSideCastle) == false);

      board.set_casle(Color::White(), kKingSideCastle, false);
      REQUIRE(board.casle(Color::White(), kQueenSideCastle) == false);
      REQUIRE(board.casle(Color::White(), kKingSideCastle) == false);
      REQUIRE(board.casle(Color::Black(), kQueenSideCastle) == false);
      REQUIRE(board.casle(Color::Black(), kKingSideCastle) == false);

      board.set_casle(Color::White(), kKingSideCastle, true);
      REQUIRE(board.casle(Color::White(), kQueenSideCastle) == false);
      REQUIRE(board.casle(Color::White(), kKingSideCastle) == true);
      REQUIRE(board.casle(Color::Black(), kQueenSideCastle) == false);
      REQUIRE(board.casle(Color::Black(), kKingSideCastle) == false);
   }

   SECTION("black queen side castle")
   {
      board.set_casle(Color::Black(), kQueenSideCastle, false);
      REQUIRE(board.casle(Color::Black(), kQueenSideCastle) == false);

      board.set_casle(Color::Black(), kQueenSideCastle, false);
      REQUIRE(board.casle(Color::White(), kQueenSideCastle) == false);
      REQUIRE(board.casle(Color::White(), kKingSideCastle) == false);
      REQUIRE(board.casle(Color::Black(), kQueenSideCastle) == false);
      REQUIRE(board.casle(Color::Black(), kKingSideCastle) == false);

      board.set_casle(Color::Black(), kQueenSideCastle, true);
      REQUIRE(board.casle(Color::White(), kQueenSideCastle) == false);
      REQUIRE(board.casle(Color::White(), kKingSideCastle) == false);
      REQUIRE(board.casle(Color::Black(), kQueenSideCastle) == true);
      REQUIRE(board.casle(Color::Black(), kKingSideCastle) == false);
   }

   SECTION("black king side castle")
   {
      board.set_casle(Color::Black(), kKingSideCastle, false);
      REQUIRE(board.casle(Color::Black(), kKingSideCastle) == false);

      board.set_casle(Color::Black(), kKingSideCastle, false);
      REQUIRE(board.casle(Color::White(), kQueenSideCastle) == false);
      REQUIRE(board.casle(Color::White(), kKingSideCastle) == false);
      REQUIRE(board.casle(Color::Black(), kQueenSideCastle) == false);
      REQUIRE(board.casle(Color::Black(), kKingSideCastle) == false);

      board.set_casle(Color::Black(), kKingSideCastle, true);
      REQUIRE(board.casle(Color::White(), kQueenSideCastle) == false);
      REQUIRE(board.casle(Color::White(), kKingSideCastle) == false);
      REQUIRE(board.casle(Color::Black(), kQueenSideCastle) == false);
      REQUIRE(board.casle(Color::Black(), kKingSideCastle) == true);
   }
}

TEST_CASE("Test AddPiece()")
{
   Board board;
   Piece white_pawn = NewPiece(kPawn, Color::White());
   Piece black_queen = NewPiece(kQueen, Color::Black());
   Piece white_queen = NewPiece(kQueen, Color::White());
   Bb bb;

   board.AddPiece(Sq::B2(), white_pawn);
   board.AddPiece(Sq::D8(), black_queen);

   REQUIRE(board[Sq::B2()] == white_pawn);
   REQUIRE(board[Sq::D8()] == black_queen);

   bb = Bb(0);
   bb.Set(Sq::B2().value());
   REQUIRE(board.UINT64_Color(Color::White()) == bb);
   REQUIRE(board.bb_piece(white_pawn) == bb);

   bb = Bb(0);
   bb.Set(Sq::D8().value());
   REQUIRE(board.UINT64_Color(Color::Black()) == bb);
   REQUIRE(board.bb_piece(black_queen) == bb);

   board.AddPiece(Sq::A2(), white_pawn);

   bb = Bb(0);
   bb.Set(Sq::B2().value());
   bb.Set(Sq::A2().value());
   REQUIRE(board.UINT64_Color(Color::White()) == bb);
   REQUIRE(board.bb_piece(white_pawn) == bb);

   board.AddPiece(Sq::C2(), white_queen);
   REQUIRE(board.bb_piece(white_pawn) == bb);

   bb.Set(Sq::C2().value());
   REQUIRE(board.UINT64_Color(Color::White()) == bb);
}

TEST_CASE("Test RemovePiece()")
{
   Board board;
   Piece white_pawn = NewPiece(kPawn, Color::White());

   board.AddPiece(Sq::A2(), white_pawn);
   board.AddPiece(Sq::B2(), white_pawn);
   board.AddPiece(Sq::C2(), white_pawn);

   board.RemovePiece(Sq::B2());

   REQUIRE(board[Sq::B2()] == kNoPiece);
   REQUIRE(board.UINT64_Color(Color::White())[Sq::B2().value()] == false);
   REQUIRE(board.bb_piece(white_pawn)[Sq::B2().value()] == false);
}

TEST_CASE("Test MovePiece()")
{
   Board board;
   Piece white_pawn = NewPiece(kPawn, Color::White());

   board.AddPiece(Sq::A2(), white_pawn);
   board.MovePiece(Sq::A2(), Sq::A3());

   REQUIRE(board[Sq::A2()] == kNoPiece);
   REQUIRE(board.UINT64_Color(Color::White())[Sq::A2().value()] == false);
   REQUIRE(board.bb_piece(white_pawn)[Sq::A2().value()] == false);

   REQUIRE(board[Sq::A3()] == white_pawn);
   REQUIRE(board.UINT64_Color(Color::White())[Sq::A3().value()] == true);
   REQUIRE(board.bb_piece(white_pawn)[Sq::A3().value()] == true);
}

TEST_CASE("Make__simple_move__piece_is_moved")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    Move move = NewMove(Sq::F3(), Sq::G4(), kWhiteQueen);
    
    board.Make(move);

    REQUIRE(board[Sq::F3()] == kNoPiece);
    REQUIRE(board[Sq::G4()] == kWhiteQueen);
    REQUIRE(board.colmn_enpas() == Column::Invalid());
    REQUIRE(board.half_move_clock() == 1);
    REQUIRE(board.side_to_move() == Color::Black());
}

TEST_CASE("Make__simple_capture__piece_is_moved")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    Move move = NewMove(Sq::E5(), Sq::F7(), kWhiteKnight, kBlackPawn);
    
    board.Make(move);

    REQUIRE(board[Sq::E5()] == kNoPiece);
    REQUIRE(board[Sq::F7()] == kWhiteKnight);
    REQUIRE(board.colmn_enpas() == Column::Invalid());
    REQUIRE(board.half_move_clock() == 0);
    REQUIRE(board.side_to_move() == Color::Black());
}

TEST_CASE("Make__king_side_castling__piece_are_moved")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    Move move = NewCastlingMove(Sq::E1(), Sq::G1(), kWhiteKing, kKingSideCastle);
    
    board.Make(move);

    REQUIRE(board[Sq::E1()] == kNoPiece);
    REQUIRE(board[Sq::H1()] == kNoPiece);
    REQUIRE(board[Sq::G1()] == kWhiteKing);
    REQUIRE(board[Sq::F1()] == kWhiteRook);
    REQUIRE(board.colmn_enpas() == Column::Invalid());
    REQUIRE(board.half_move_clock() == 1);
    REQUIRE(board.side_to_move() == Color::Black());
}

TEST_CASE("Make__queen_side_castling__piece_are_moved")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    Move move = NewCastlingMove(Sq::E1(), Sq::C1(), kWhiteKing, kQueenSideCastle);
    
    board.Make(move);

    REQUIRE(board[Sq::E1()] == kNoPiece);
    REQUIRE(board[Sq::A1()] == kNoPiece);
    REQUIRE(board[Sq::C1()] == kWhiteKing);
    REQUIRE(board[Sq::D1()] == kWhiteRook);
    REQUIRE(board.colmn_enpas() == Column::Invalid());
    REQUIRE(board.half_move_clock() == 1);
    REQUIRE(board.side_to_move() == Color::Black());
}

TEST_CASE("Make__pawn_move__piece_is_moved")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    Move move = NewMove(Sq::D5(), Sq::D6(), kWhitePawn);
    
    board.Make(move);

    REQUIRE(board[Sq::D5()] == kNoPiece);
    REQUIRE(board[Sq::D6()] == kWhitePawn);
    REQUIRE(board.colmn_enpas() == Column::Invalid());
    REQUIRE(board.half_move_clock() == 0);
    REQUIRE(board.side_to_move() == Color::Black());
}

TEST_CASE("Make__pawn_moved_two_square__piece_is_moved_and_en_passant_flag_set")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    Move move = NewMove(Sq::A2(), Sq::A4(), kWhitePawn);
    
    board.Make(move);

    REQUIRE(board[Sq::A2()] == kNoPiece);
    REQUIRE(board[Sq::A4()] == kWhitePawn);
    REQUIRE(board.colmn_enpas() == Column::A());
    REQUIRE(board.half_move_clock() == 0);
    REQUIRE(board.side_to_move() == Color::Black());
}

TEST_CASE("Make__pawn_capture__piece_are_moved")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    Move move = NewMove(Sq::D5(), Sq::E6(), kWhitePawn, kBlackPawn);
    
    board.Make(move);

    REQUIRE(board[Sq::D5()] == kNoPiece);
    REQUIRE(board[Sq::E6()] == kWhitePawn);
    REQUIRE(board.colmn_enpas() == Column::Invalid());
    REQUIRE(board.half_move_clock() == 0);
    REQUIRE(board.side_to_move() == Color::Black());
}

TEST_CASE("Make__pawn_capture_en_passant__piece_are_moved")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/Pp2P3/2N2Q1p/1PPBBPPP/R3K2R b KQkq a3 0 1");
    Move move = NewMove(Sq::B4(), Sq::A3(), kBlackPawn, kWhitePawn);
    
    board.Make(move);

    REQUIRE(board[Sq::B4()] == kNoPiece);
    REQUIRE(board[Sq::A3()] == kBlackPawn);
    REQUIRE(board[Sq::A4()] == kNoPiece);
    REQUIRE(board.colmn_enpas() == Column::Invalid());
    REQUIRE(board.half_move_clock() == 0);
    REQUIRE(board.side_to_move() == Color::White());
}

TEST_CASE("Make__pawn_promotion__piece_is_promoted")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/Pp2P3/2N2Q2/1PPBBPpP/R3K2R b KQkq - 0 2");
    Move move = NewMove(Sq::G2(), Sq::H1(), kBlackPawn, kWhiteRook, kBlackKnight);
    
    board.Make(move);

    REQUIRE(board[Sq::G2()] == kNoPiece);
    REQUIRE(board[Sq::H1()] == kBlackKnight);
    REQUIRE(board.colmn_enpas() == Column::Invalid());
    REQUIRE(board.half_move_clock() == 0);
    REQUIRE(board.side_to_move() == Color::White());
}

TEST_CASE("Make__king_move__castling_flags_are_cleared")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    Move move = NewMove(Sq::E1(), Sq::F1(), kWhiteKing);
    
    board.Make(move);

    REQUIRE(board[Sq::E1()] == kNoPiece);
    REQUIRE(board[Sq::F1()] == kWhiteKing);
    REQUIRE(board.colmn_enpas() == Column::Invalid());
    REQUIRE(board.half_move_clock() == 1);
    REQUIRE(board.side_to_move() == Color::Black());
    REQUIRE(!board.casle(Color::White(), kQueenSideCastle));
    REQUIRE(!board.casle(Color::White(), kKingSideCastle));
}

TEST_CASE("Make__king_side_rook_move__king_side_castling_flag_cleared")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    Move move = NewMove(Sq::H1(), Sq::G1(), kWhiteRook);
    
    board.Make(move);

    REQUIRE(board[Sq::H1()] == kNoPiece);
    REQUIRE(board[Sq::G1()] == kWhiteRook);
    REQUIRE(board.colmn_enpas() == Column::Invalid());
    REQUIRE(board.half_move_clock() == 1);
    REQUIRE(board.side_to_move() == Color::Black());
    REQUIRE(board.casle(Color::White(), kQueenSideCastle));
    REQUIRE(!board.casle(Color::White(), kKingSideCastle));
}

TEST_CASE("Make__queen_side_rook_move__queen_side_castling_flag_cleared")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    Move move = NewMove(Sq::A1(), Sq::B1(), kWhiteRook);
    
    board.Make(move);

    REQUIRE(board[Sq::A1()] == kNoPiece);
    REQUIRE(board[Sq::B1()] == kWhiteRook);
    REQUIRE(board.colmn_enpas() == Column::Invalid());
    REQUIRE(board.half_move_clock() == 1);
    REQUIRE(board.side_to_move() == Color::Black());
    REQUIRE(!board.casle(Color::White(), kQueenSideCastle));
    REQUIRE(board.casle(Color::White(), kKingSideCastle));
}

TEST_CASE("Unmake__simple_move__piece_is_replaced")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    Move move = NewMove(Sq::F3(), Sq::G4(), kWhiteQueen);
    UnmakeInfo unmake_info;

    unmake_info = board.Make(move);
    board.Unmake(move, unmake_info);

    REQUIRE(board[Sq::F3()] == kWhiteQueen);
    REQUIRE(board[Sq::G4()] == kNoPiece);
    REQUIRE(board.colmn_enpas() == Column::Invalid());
    REQUIRE(board.half_move_clock() == 0);
    REQUIRE(board.side_to_move() == Color::White());
}

TEST_CASE("Unmake__simple_capture__capture_is_unmade")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    Move move = NewMove(Sq::E5(), Sq::F7(), kWhiteKnight, kBlackPawn);
    UnmakeInfo unmake_info;

    unmake_info = board.Make(move);
    board.Unmake(move, unmake_info);

    REQUIRE(board[Sq::E5()] == kWhiteKnight);
    REQUIRE(board[Sq::F7()] == kBlackPawn);
    REQUIRE(board.colmn_enpas() == Column::Invalid());
    REQUIRE(board.half_move_clock() == 0);
    REQUIRE(board.side_to_move() == Color::White());
}

TEST_CASE("Unmake__king_side_castling__castling_is_unmade")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    Move move = NewCastlingMove(Sq::E1(), Sq::G1(), kWhiteKing, kKingSideCastle);
    UnmakeInfo unmake_info;

    unmake_info = board.Make(move);
    board.Unmake(move, unmake_info);

    REQUIRE(board[Sq::E1()] == kWhiteKing);
    REQUIRE(board[Sq::H1()] == kWhiteRook);
    REQUIRE(board[Sq::G1()] == kNoPiece);
    REQUIRE(board[Sq::F1()] == kNoPiece);
    REQUIRE(board.colmn_enpas() == Column::Invalid());
    REQUIRE(board.half_move_clock() == 0);
    REQUIRE(board.side_to_move() == Color::White());
}

TEST_CASE("Unmake__queen_side_castling__castling_is_unmade")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    Move move = NewCastlingMove(Sq::E1(), Sq::C1(), kWhiteKing, kQueenSideCastle);
    UnmakeInfo unmake_info;

    unmake_info = board.Make(move);
    board.Unmake(move, unmake_info);

    REQUIRE(board[Sq::E1()] == kWhiteKing);
    REQUIRE(board[Sq::A1()] == kWhiteRook);
    REQUIRE(board[Sq::C1()] == kNoPiece);
    REQUIRE(board[Sq::D1()] == kNoPiece);
    REQUIRE(board.colmn_enpas() == Column::Invalid());
    REQUIRE(board.half_move_clock() == 0);
    REQUIRE(board.side_to_move() == Color::White());
}

TEST_CASE("Unmake__pawn_move__moved_is_unmade")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    Move move = NewMove(Sq::D5(), Sq::D6(), kWhitePawn);
    UnmakeInfo unmake_info;

    unmake_info = board.Make(move);
    board.Unmake(move, unmake_info);

    REQUIRE(board[Sq::D5()] == kWhitePawn);
    REQUIRE(board[Sq::D6()] == kNoPiece);
    REQUIRE(board.colmn_enpas() == Column::Invalid());
    REQUIRE(board.half_move_clock() == 0);
    REQUIRE(board.side_to_move() == Color::White());
}

TEST_CASE("Unmake__pawn_moved_two_square__piece_is_moved")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    Move move = NewMove(Sq::A2(), Sq::A4(), kWhitePawn);
    UnmakeInfo unmake_info;

    unmake_info = board.Make(move);
    board.Unmake(move, unmake_info);

    REQUIRE(board[Sq::A2()] == kWhitePawn);
    REQUIRE(board[Sq::A4()] == kNoPiece);
    REQUIRE(board.colmn_enpas() == Column::Invalid());
    REQUIRE(board.half_move_clock() == 0);
    REQUIRE(board.side_to_move() == Color::White());
}

TEST_CASE("Unmake__pawn_capture__piece_are_moved")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    Move move = NewMove(Sq::D5(), Sq::E6(), kWhitePawn, kBlackPawn);
    UnmakeInfo unmake_info;

    unmake_info = board.Make(move);
    board.Unmake(move, unmake_info);

    REQUIRE(board[Sq::D5()] == kWhitePawn);
    REQUIRE(board[Sq::E6()] == kBlackPawn);
    REQUIRE(board.colmn_enpas() == Column::Invalid());
    REQUIRE(board.half_move_clock() == 0);
    REQUIRE(board.side_to_move() == Color::White());
}

TEST_CASE("Unmake__pawn_capture_en_passant__piece_are_moved")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/Pp2P3/2N2Q1p/1PPBBPPP/R3K2R b KQkq a3 0 1");
    Move move = NewMove(Sq::B4(), Sq::A3(), kBlackPawn, kWhitePawn);
    UnmakeInfo unmake_info;

    unmake_info = board.Make(move);
    board.Unmake(move, unmake_info);

    REQUIRE(board[Sq::B4()] == kBlackPawn);
    REQUIRE(board[Sq::A3()] == kNoPiece);
    REQUIRE(board[Sq::A4()] == kWhitePawn);
    REQUIRE(board.colmn_enpas() == Column::A());
    REQUIRE(board.half_move_clock() == 0);
    REQUIRE(board.side_to_move() == Color::Black());
}

TEST_CASE("Unmake__pawn_promotion__board_is_restored")
{
    Board board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/Pp2P3/2N2Q2/1PPBBPpP/R3K2R b KQkq - 0 2");
    Move move = NewMove(Sq::G2(), Sq::H1(), kBlackPawn, kWhiteRook, kBlackKnight);
    UnmakeInfo unmake_info;

    unmake_info = board.Make(move);
    board.Unmake(move, unmake_info);

    REQUIRE(board[Sq::G2()] == kBlackPawn);
    REQUIRE(board[Sq::H1()] == kWhiteRook);
    REQUIRE(board.colmn_enpas() == Column::Invalid());
    REQUIRE(board.half_move_clock() == 0);
    REQUIRE(board.side_to_move() == Color::Black());
}

