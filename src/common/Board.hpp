/// @file   Board.hpp
/// @author Mathieu Pagé
/// @date   Feburary 2015
/// @brief  Contains the definition of the Board type that represent the state 
///         of a chess board.

#ifndef M8_BOARD_HPP_
#define M8_BOARD_HPP_

#include <array>
#include <cstdint>

#include "Piece.hpp"
#include "Bb.hpp"
#include "Sq.hpp"

namespace m8
{
   /// Exception type thrown when we try to parse an invalid fen string.
   class InvalFenError : public std::runtime_error
   {
   public:
      /// Constructor
      InvalFenError(const std::string& what_arg)
         : std::runtime_error(what_arg)
      {};
   };

   /// @name Castle flags constant
   /// Constant used as flags for the castling rights
   /// @{
   const std::uint8_t kQueenSideCasle = 1;
   const std::uint8_t kKingSideCasle = 2;
   /// @}

   /// Represent the state of a chess board. This include the pieces positions,
   /// the side to move next, the castling rights and the en-passant square.
   class Board
   {
   public:

      /// Default constructor
      Board();

      /// Constructor from an X-FEN string. An X-FEN string is a fen string
      /// with added functionalities to be used in Fischer Random Chess. A FEN
      /// string is a string that fully represent the state of a chess board.
      ///
      /// For more information check here:
      /// http://chessprogramming.wikispaces.com/Forsyth-Edwards+Notation
      ///
      /// @param fen X-FEN string that represent the board state to recreate.
      Board(const std::string&  fen);

      /// Index operator. Returns the piece that is on a given square.
      ///
      /// @param index Index of the square we want to get the piece from.
      /// @returns The piece that is on the square passed in parameter.
      inline Piece operator[](std::size_t index) const;

      /// Accessor for the side to move.
      ///
      /// @returns The side to move next.
      inline Color side_to_move() const { return side_to_move_; };

      /// Mutator for the side to move.
      ///
      /// @param side_to_move the color of the new side to move.
      inline void set_side_to_move(Color side_to_move);

      /// Accessor for the pieces bitboards.
      ///
      /// @param piece Piece for wich we want to retreive the bitboard.
      /// @returns A bitboard with bits to 1 for every position where there is
      ///          the piece specified in parameter.
      inline Bb bb_piece(Piece piece) const;

      /// Accessor for the colors bitboards.
      ///
      /// @param color Color for wich we want to retreive the bitboard.
      /// @returns A bitboard with bits to 1 for every position where there is
      ///          a piece of the color specified in parameter.
      inline Bb bb_color(Color color) const;

      /// Accessor for the castlings rights.
      /// 
      /// @param color Side for which we want the castling rights
      /// @param castle_right Castling right to read. Valid values are 
      ///        kQueenSideCasle and kKingSideCasle
      /// @returns True if the color specified still has the castling right 
      ///          specified.
      inline bool casle(Color color, std::uint8_t casle_right) const;

      /// mutator for the castling rights.
      ///
      /// @param color Color for which we want to mutate the castling rights.
      /// @param casle_right Castling right to mutate.
      /// @param value New value of the casling right.
      inline void set_casle(Color color, std::uint8_t casle_right, bool value);

      /// Accesor for the castling columns.
      ///
      /// @param indx Index of the column to get, must be 0 or 1 for the queen 
      ///             side castling column and the king side castling columns 
      ///             respectively.
      /// @returns A column.
      inline Colmn casle_colmn(std::size_t indx) const;

      /// Accessor for the column of the pawn that can be captured en passant.
      ///
      /// @returns A column. If no piece can be captured en passant an invalid 
      ///          column is returned.
      inline Colmn colmn_enpas() const { return colmn_enpas_; };

      /// Mutator for the column en passant.
      ///
      /// @param colmn_enpass Column of the piece that can be captured en 
      ///        passant or an invalid column if no piece can be captured en 
      ///        passant.
      inline void set_colmn_enpas(Colmn colmn_enpas) { colmn_enpas_ = colmn_enpas; };

      /// Accessor for the half move clock.
      ///
      /// @returns The number of moves since the last pawn push or the 
      ///          last capture.
      inline std::uint32_t half_move_clock() const { return half_move_clock_; };

      /// Mutator for the half move clock.
      ///
      /// @param value New value for the half move clock.
      inline void set_half_move_clock(std::uint32_t value) { half_move_clock_ = value; };

      /// Add a piece to the board. The square where we add the piece must be 
      /// empty.
      ///
      /// @param sq Square where the piece will be added.
      /// @param piece Piece to add.
      inline void AddPiece(Sq sq, Piece piece);

      /// Remove a piece from the board.
      ///
      /// @param sq Square of the piece to remove.
      inline void RemovePiece(Sq sq);

      /// Move a piece on the board.
      ///
      /// @param from Position of the piece before the move.
      /// @param to Postion of the piece after the move.
      inline void MovePiece(Sq from, Sq to);

   private:

      /// Initialize the board with no pieces.
      void Clear();

      /// Array containing the piece on each square of the board.
      std::array<Piece, kNumSqOnBoard> board_;

      /// Color that has to move next
      Color side_to_move_;

      /// Bitboard for each piece
      std::array<Bb, kMaxPiece + 1> bb_piece_;

      /// Bitboard for each color
      std::array<Bb, 2> bb_color_;

      /// Original column of the two rooks. This is used to determine which 
      /// rook can castle because in Chess960 the rook may not start on 
      /// the column a and h.
      std::array<Colmn, 2> casle_colmn_;

      /// Integer containing flags indicating the remaining castling privileges.
      /// 
      /// bit description
      /// --- -----------------------
      ///  1  White king side castle
      ///  2  White queen side castle
      ///  3  Black king side castle
      ///  4  Black quenn side castle
      std::uint8_t casle_flag_;

      /// Column of the piece that can be captured en-passant. IF no piece can 
      /// be captured en passant col_enpas_ has an invalid column value.
      Colmn colmn_enpas_;

      // Number of moves since the last pawn push or the last capture.
      std::uint32_t half_move_clock_;
   };
   
   inline Piece Board::operator[](std::size_t index) const
   {
      // A : l'index est valide
      assert(0 <= index && index < kNumSqOnBoard);

      return board_[index];
   }

   inline void Board::set_side_to_move(Color side_to_move)
   {
      // A : The side to move is a valid color
      assert(IsColor(side_to_move));

      side_to_move_ = side_to_move;
   }

   inline Bb Board::bb_piece(Piece piece) const
   {
      // A : The piece is valid
      assert(IsPiece(piece));

      return bb_piece_[piece];
   }

   inline Bb Board::bb_color(Color color) const
   {
      // A : The color is valid
      assert(IsColor(color));

      return bb_color_[color];
   }

   inline bool Board::casle(Color color, std::uint8_t casle_right) const
   {
      // A : The color and castling rights are valids
      assert(IsColor(color));
      assert(casle_right == kQueenSideCasle || casle_right == kKingSideCasle);

      return (casle_flag_ & (casle_right << color << color)) != 0;
   }

   inline void Board::set_casle(Color color, std::uint8_t casle_right, bool value)
   {
      // A : The inputs are valids
      assert(IsColor(color));
      assert(casle_right == kQueenSideCasle || casle_right == kKingSideCasle);

      std::uint8_t mask = casle_right << color << color;
      casle_flag_ ^= (-static_cast<std::uint8_t>(value) ^ casle_flag_) & mask;
   }

   inline Colmn Board::casle_colmn(std::size_t indx) const
   {
      // A : The index is 0 or 1.
      assert(indx == 0 || indx == 1);

      return casle_colmn_[indx];
   }

   inline void Board::AddPiece(Sq sq, Piece piece)
   {
      // A : sq is a valid square, piece is a valid piece and the destination
      //     square is empty
      assert(IsSqOnBoard(sq));
      assert(IsPiece(piece));
      assert(board_[sq] == kNoPiece);

      board_[sq] = piece;

      SetBit(bb_piece_[piece], sq);

      Color color = GetColor(piece);
      SetBit(bb_color_[color], sq);
   }

   inline void Board::RemovePiece(Sq sq)
   {
      // A : sq is a valid square and the square is not empty.
      assert(IsSqOnBoard(sq));
      assert(IsPiece(board_[sq]));

      Color color = GetColor(board_[sq]);
      UnsetBit(bb_color_[color], sq);

      UnsetBit(bb_piece_[board_[sq]], sq);

      board_[sq] = kNoPiece;
   }

   inline void Board::MovePiece(Sq from, Sq to)
   {
      // A : from and to are valid squares. From contains a piece and to is empty.
      assert(IsSqOnBoard(from));
      assert(IsSqOnBoard(to));
      assert(IsPiece(board_[from]));
      assert(board_[to] == kNoPiece);

      Piece piece = board_[from];
      board_[from] = kNoPiece;
      board_[to] = piece;

      Bb diff = UINT64_C(0);
      SetBit(diff, from);
      SetBit(diff, to);

      bb_color_[GetColor(piece)] ^= diff;
      bb_piece_[piece] ^= diff;
   }
}

#endif // M8_BOARD_HPP_