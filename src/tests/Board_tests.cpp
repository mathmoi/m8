/// @file   Board_tests.cpp
/// @author Mathieu Pagé
/// @date   Feburary 2015
/// @brief  Contains tests of the Board type and functionalities.

#include "catch.hpp"

#include <cstdint>

#include "../common/Board.hpp"

using namespace m8;
using namespace std;

TEST_CASE("Test Board()")
{
   Board board;

   REQUIRE(board.side_to_move() == kWhite);

   for (Sq sq = kA1; IsSqOnBoard(sq); ++sq)
   {
      REQUIRE(board[sq] == kNoPiece);
   }

   for (Color color = kWhite; IsColor(color); ++color)
   {
      for (PieceType piece_type = kMinPieceType; IsPieceType(piece_type); ++piece_type)
      {
         REQUIRE(board.bb_piece(NewPiece(piece_type, color)) == kEmptyBb);
      }
   }

   REQUIRE(board.bb_color(kWhite) == kEmptyBb);
   REQUIRE(board.bb_color(kBlack) == kEmptyBb);

   REQUIRE(board.casle(kWhite, kQueenSideCasle) == false);
   REQUIRE(board.casle(kWhite, kKingSideCasle) == false);
   REQUIRE(board.casle(kBlack, kQueenSideCasle) == false);
   REQUIRE(board.casle(kBlack, kKingSideCasle) == false);

   REQUIRE(IsColmnOnBoard(board.colmn_enpas()) == false);
}

TEST_CASE("Test Board(fen)")
{
   // Initial postion test
   Board board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

   REQUIRE(board[kA8] == kBlackRook);
   REQUIRE(board[kB8] == kBlackKnight);
   REQUIRE(board[kC8] == kBlackBishop);
   REQUIRE(board[kD8] == kBlackQueen);
   REQUIRE(board[kE8] == kBlackKing);
   REQUIRE(board[kF8] == kBlackBishop);
   REQUIRE(board[kG8] == kBlackKnight);
   REQUIRE(board[kH8] == kBlackRook);

   REQUIRE(board[kA7] == kBlackPawn);
   REQUIRE(board[kB7] == kBlackPawn);
   REQUIRE(board[kC7] == kBlackPawn);
   REQUIRE(board[kD7] == kBlackPawn);
   REQUIRE(board[kE7] == kBlackPawn);
   REQUIRE(board[kF7] == kBlackPawn);
   REQUIRE(board[kG7] == kBlackPawn);
   REQUIRE(board[kH7] == kBlackPawn);

   for (Row row = kRow6; row >= kRow3; --row)
   {
      for (Colmn colmn = kColmnA; IsColmnOnBoard(colmn); ++colmn)
      {
         Sq sq = NewSq(colmn, row);
         REQUIRE(board[sq] == kNoPiece);
      }
   }

   REQUIRE(board[kA2] == kWhitePawn);
   REQUIRE(board[kB2] == kWhitePawn);
   REQUIRE(board[kC2] == kWhitePawn);
   REQUIRE(board[kD2] == kWhitePawn);
   REQUIRE(board[kE2] == kWhitePawn);
   REQUIRE(board[kF2] == kWhitePawn);
   REQUIRE(board[kG2] == kWhitePawn);
   REQUIRE(board[kH2] == kWhitePawn);

   REQUIRE(board[kA1] == kWhiteRook);
   REQUIRE(board[kB1] == kWhiteKnight);
   REQUIRE(board[kC1] == kWhiteBishop);
   REQUIRE(board[kD1] == kWhiteQueen);
   REQUIRE(board[kE1] == kWhiteKing);
   REQUIRE(board[kF1] == kWhiteBishop);
   REQUIRE(board[kG1] == kWhiteKnight);
   REQUIRE(board[kH1] == kWhiteRook);

   REQUIRE(board.side_to_move() == kWhite);

   REQUIRE(board.casle(kWhite, kQueenSideCasle) == true);
   REQUIRE(board.casle(kWhite, kKingSideCasle) == true);
   REQUIRE(board.casle(kBlack, kQueenSideCasle) == true);
   REQUIRE(board.casle(kBlack, kKingSideCasle) == true);

   REQUIRE(board.casle_colmn(0) == kColmnA);
   REQUIRE(board.casle_colmn(1) == kColmnH);

   REQUIRE(board.colmn_enpas() == kInvalColmn);
}

TEST_CASE("Test Board(fen) 2")
{
   // Chess360 castling test
   Board board("rn2k1r1/ppp1pp1p/3p2p1/5bn1/P7/2N2B2/1PPPPP2/2BNK1RR w Gkq - 4 11");

   REQUIRE(board.casle(kWhite, kQueenSideCasle) == false);
   REQUIRE(board.casle(kWhite, kKingSideCasle) == true);
   REQUIRE(board.casle(kBlack, kQueenSideCasle) == true);
   REQUIRE(board.casle(kBlack, kKingSideCasle) == true);

   REQUIRE(board.casle_colmn(0) == kColmnA);
   REQUIRE(board.casle_colmn(1) == kColmnG);

   REQUIRE(board.half_move_clock() == 4);
}

TEST_CASE("Test Board(fen) 3")
{
   // Chess360 castling test
   Board board("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");

   REQUIRE(board.colmn_enpas() == kColmnE);
}

TEST_CASE("Test Board(fen) 4")
{
   // Chess360 castling test
   Board board("rn2k1r1/ppp1pp1p/3p2p1/5bn1/P7/2N2B2/1PPPPP2/2BNK1RR w");

   REQUIRE(board.side_to_move() == kWhite);
}

TEST_CASE("Test side_to_move()")
{
   Board board;

   REQUIRE(board.side_to_move() == kWhite);

   board.set_side_to_move(kWhite);
   REQUIRE(board.side_to_move() == kWhite);

   board.set_side_to_move(kBlack);
   REQUIRE(board.side_to_move() == kBlack);

   board.set_side_to_move(kBlack);
   REQUIRE(board.side_to_move() == kBlack);

   board.set_side_to_move(kWhite);
   REQUIRE(board.side_to_move() == kWhite);
}

TEST_CASE("Test colmn_enpas()")
{
   Board board;

   REQUIRE(!IsColmnOnBoard(board.colmn_enpas()));

   board.set_colmn_enpas(kColmnD);
   REQUIRE(board.colmn_enpas() == kColmnD);

   board.set_colmn_enpas(kInvalColmn);
   REQUIRE(!IsColmnOnBoard(board.colmn_enpas()));
}

TEST_CASE("Test casle()")
{
   Board board;

   SECTION("white queen side castle")
   {
      board.set_casle(kWhite, kQueenSideCasle, false);
      REQUIRE(board.casle(kWhite, kQueenSideCasle) == false);

      board.set_casle(kWhite, kQueenSideCasle, false);
      REQUIRE(board.casle(kWhite, kQueenSideCasle) == false);
      REQUIRE(board.casle(kWhite, kKingSideCasle) == false);
      REQUIRE(board.casle(kBlack, kQueenSideCasle) == false);
      REQUIRE(board.casle(kBlack, kKingSideCasle) == false);

      board.set_casle(kWhite, kQueenSideCasle, true);
      REQUIRE(board.casle(kWhite, kQueenSideCasle) == true);
      REQUIRE(board.casle(kWhite, kKingSideCasle) == false);
      REQUIRE(board.casle(kBlack, kQueenSideCasle) == false);
      REQUIRE(board.casle(kBlack, kKingSideCasle) == false);
   }

   SECTION("white king side castle")
   {
      board.set_casle(kWhite, kKingSideCasle, false);
      REQUIRE(board.casle(kWhite, kKingSideCasle) == false);

      board.set_casle(kWhite, kKingSideCasle, false);
      REQUIRE(board.casle(kWhite, kQueenSideCasle) == false);
      REQUIRE(board.casle(kWhite, kKingSideCasle) == false);
      REQUIRE(board.casle(kBlack, kQueenSideCasle) == false);
      REQUIRE(board.casle(kBlack, kKingSideCasle) == false);

      board.set_casle(kWhite, kKingSideCasle, true);
      REQUIRE(board.casle(kWhite, kQueenSideCasle) == false);
      REQUIRE(board.casle(kWhite, kKingSideCasle) == true);
      REQUIRE(board.casle(kBlack, kQueenSideCasle) == false);
      REQUIRE(board.casle(kBlack, kKingSideCasle) == false);
   }

   SECTION("black queen side castle")
   {
      board.set_casle(kBlack, kQueenSideCasle, false);
      REQUIRE(board.casle(kBlack, kQueenSideCasle) == false);

      board.set_casle(kBlack, kQueenSideCasle, false);
      REQUIRE(board.casle(kWhite, kQueenSideCasle) == false);
      REQUIRE(board.casle(kWhite, kKingSideCasle) == false);
      REQUIRE(board.casle(kBlack, kQueenSideCasle) == false);
      REQUIRE(board.casle(kBlack, kKingSideCasle) == false);

      board.set_casle(kBlack, kQueenSideCasle, true);
      REQUIRE(board.casle(kWhite, kQueenSideCasle) == false);
      REQUIRE(board.casle(kWhite, kKingSideCasle) == false);
      REQUIRE(board.casle(kBlack, kQueenSideCasle) == true);
      REQUIRE(board.casle(kBlack, kKingSideCasle) == false);
   }

   SECTION("black king side castle")
   {
      board.set_casle(kBlack, kKingSideCasle, false);
      REQUIRE(board.casle(kBlack, kKingSideCasle) == false);

      board.set_casle(kBlack, kKingSideCasle, false);
      REQUIRE(board.casle(kWhite, kQueenSideCasle) == false);
      REQUIRE(board.casle(kWhite, kKingSideCasle) == false);
      REQUIRE(board.casle(kBlack, kQueenSideCasle) == false);
      REQUIRE(board.casle(kBlack, kKingSideCasle) == false);

      board.set_casle(kBlack, kKingSideCasle, true);
      REQUIRE(board.casle(kWhite, kQueenSideCasle) == false);
      REQUIRE(board.casle(kWhite, kKingSideCasle) == false);
      REQUIRE(board.casle(kBlack, kQueenSideCasle) == false);
      REQUIRE(board.casle(kBlack, kKingSideCasle) == true);
   }
}

TEST_CASE("Test AddPiece()")
{
   Board board;
   Piece white_pawn = NewPiece(kPawn, kWhite);
   Piece black_queen = NewPiece(kQueen, kBlack);
   Piece white_queen = NewPiece(kQueen, kWhite);
   Bb bb;

   board.AddPiece(kB2, white_pawn);
   board.AddPiece(kD8, black_queen);

   REQUIRE(board[kB2] == white_pawn);
   REQUIRE(board[kD8] == black_queen);

   bb = Bb(0);
   SetBit(bb, kB2);
   REQUIRE(board.bb_color(kWhite) == bb);
   REQUIRE(board.bb_piece(white_pawn) == bb);

   bb = Bb(0);
   SetBit(bb, kD8);
   REQUIRE(board.bb_color(kBlack) == bb);
   REQUIRE(board.bb_piece(black_queen) == bb);

   board.AddPiece(kA2, white_pawn);

   bb = Bb(0);
   SetBit(bb, kB2);
   SetBit(bb, kA2);
   REQUIRE(board.bb_color(kWhite) == bb);
   REQUIRE(board.bb_piece(white_pawn) == bb);

   board.AddPiece(kC2, white_queen);
   REQUIRE(board.bb_piece(white_pawn) == bb);

   SetBit(bb, kC2);
   REQUIRE(board.bb_color(kWhite) == bb);
}

TEST_CASE("Test RemovePiece()")
{
   Board board;
   Piece white_pawn = NewPiece(kPawn, kWhite);

   board.AddPiece(kA2, white_pawn);
   board.AddPiece(kB2, white_pawn);
   board.AddPiece(kC2, white_pawn);

   board.RemovePiece(kB2);

   REQUIRE(board[kB2] == kNoPiece);
   REQUIRE(GetBit(board.bb_color(kWhite), kB2) == false);
   REQUIRE(GetBit(board.bb_piece(white_pawn), kB2) == false);
}

TEST_CASE("Test MovePiece()")
{
   Board board;
   Piece white_pawn = NewPiece(kPawn, kWhite);

   board.AddPiece(kA2, white_pawn);
   board.MovePiece(kA2, kA3);

   REQUIRE(board[kA2] == kNoPiece);
   REQUIRE(GetBit(board.bb_color(kWhite), kA2) == false);
   REQUIRE(GetBit(board.bb_piece(white_pawn), kA2) == false);

   REQUIRE(board[kA3] == white_pawn);
   REQUIRE(GetBit(board.bb_color(kWhite), kA3) == true);
   REQUIRE(GetBit(board.bb_piece(white_pawn), kA3) == true);
}