/// @file   Board.cpp
/// @author Mathieu Pagé
/// @date   Feburary 2015
/// @brief  Contains the definition of methods of the Board class that 
///         represent the state of a chess board.

#include "Board.hpp"
#include "Const.hpp"
#include "Utils.hpp"

namespace m8
{
   Board::Board()
   {
      Clear();
   }

   Board::Board(const std::string&  fen)
   {
      Clear();

      std::string::const_iterator it = fen.begin();

      // First we consume the piece placement section
      Colmn colmn = kColmnA;
      Row row = kRow8;
      while (it < fen.end() && *it != ' ')
      {
         // If the character is a digit we increment the current column by this
         // digit value.
         if (*it >= '0' && *it <= '9')
         {
            colmn += *it - '0';
         }
         else
         {
            // If the character is a piece we add it to the board at the 
            // current position.
            auto it_piece = char_to_piece_map.find(*it);
            if (it_piece != char_to_piece_map.end())
            {
               // If the current position is invalid we throw an exception
               if (!IsColmnOnBoard(colmn) || !IsRowOnBoard(row))
               {
                  throw InvalFenError("Invalid piece placement section in fen string");
               }

               Sq sq = NewSq(colmn, row);
               AddPiece(sq, it_piece->second);
               ++colmn;
            }
            else
            {
               // If the character is a '/' we move to the next row
               if (*it == '/')
               {
                  colmn = kColmnA;
                  --row;
               }
               else
               {
                  // If we get here we did not recognize the character and we 
                  // raise an exception.
                  throw InvalFenError("Invalid character in fen string.");
               }
            }
         }

         // Move to the next character
         ++it;
      }

      // Consume space characters
      while (it < fen.end() && *it == ' ')
      {
         ++it;
      }

      // If we are at the end of the string we assume it's white's turn.
      // Otherwise we try to read it from the next character.
      if (it != fen.end())
      {
         if (*it == 'w')
         {
            set_side_to_move(kWhite);
            ++it;
         }
         else
         {
            if (*it == 'b')
            {
               set_side_to_move(kBlack);
               ++it;
            }
            else
            {
               // We are unable to read the side to move, we raise an exception;
               throw InvalFenError("Unable to read the side to move in the fen string.");
            }
         }
      }

      // Consume space characters
      while (it < fen.end() && *it == ' ')
      {
         ++it;
      }

      // If we are not at the end of the stream the next field is the castling 
      // rights field.
      while (it < fen.end() && *it != ' ')
      {
          if (*it != '-')
          {
              Color color;
              Colmn colmn;
              uint8_t casle_right;
              Bb bb_rook;
              Sq sq_rook;
              Sq sq_king;

              color = isupper(*it) ? kWhite : kBlack;
              bb_rook = bb_piece(NewPiece(kRook, color)) & GetRowBb(color * kRow8);

              if (*it == 'Q' || *it == 'q')
              {
                  sq_rook = GetLsb(bb_rook);
                  casle_right = kQueenSideCastle;
              }
              else if (*it == 'K' || *it == 'k')
              {
                  sq_rook = GetMsb(bb_rook);
                  casle_right = kKingSideCastle;
              }
              else if ((tolower(*it) >= 'a' && tolower(*it) <= 'h'))
              {
                  sq_rook = NewSq(tolower(*it) - 'a', color * kRow8);
                  sq_king = GetLsb(bb_piece(NewPiece(kKing, color)));
                  casle_right = (sq_rook < sq_king ? kQueenSideCastle : kKingSideCastle);
              }
              else
              {
                  // We are unable to parse the castling rights, we raise an exception;
                  throw InvalFenError("Unable to read the castinling rights from the fen string.");
              }

              colmn = GetColmn(sq_rook);
              casle_colmn_[casle_right == kQueenSideCastle ? 0 : 1] = colmn;
              set_casle(color, casle_right, true);
          }

         ++it;
      }

      // Consume space characters
      while (it < fen.end() && *it == ' ')
      {
         ++it;
      }

      // If we are not at the end of the string the next field is the position 
      // of the piece that can be taken en-passant.
      if (it < fen.end())
      {
         if (*it != '-')
         {
            colmn_enpas_ = (*it - 'a');
         }

         // Skip characters up to the end or a space
         while (it < fen.end() && *it != ' ')
         {
            ++it;
         }
      }

      // Consume space characters
      while (it < fen.end() && *it == ' ')
      {
         ++it;
      }

      // If we are not at the end of the string the next field is the 
      // halfmove clock.
      if (it < fen.end())
      {
         std::string str;
         while (it < fen.end() &&
                '0' <= *it && *it <= '9')
         {
            str += *it;
            ++it;
         }

         half_move_clock_ = ConvertTo<std::uint32_t>(str);
      }
   }

   void Board::Clear()
   {
      // Initialize the side to move
      side_to_move_ = kWhite;

      // Initialize the board_
      for (Sq sq = 0; IsSqOnBoard(sq); ++sq)
      {
         board_[sq] = kNoPiece;
      }

      // Initialize the pieces bitboards
      for (Color color = kWhite; IsColor(color); ++color)
      {
         for (PieceType piece_type = kMinPieceType; IsPieceType(piece_type); ++piece_type)
         {
            Piece piece = NewPiece(piece_type, color);
            bb_piece_[piece] = kEmptyBb;
         }
      }

      // Initialize the color bitboards
      bb_color_[kWhite] = kEmptyBb;
      bb_color_[kBlack] = kEmptyBb;

      // Initialize the castle columns. By default we use the regular chess columns.
      casle_colmn_[0] = kColmnA;
      casle_colmn_[1] = kColmnH;

      // Initialize the castle flags, the column of the en-passant capture and 
      // the half move clock.
      casle_flag_ = 0;
      colmn_enpas_ = kInvalColmn;
      half_move_clock_ = 0;
   }
}