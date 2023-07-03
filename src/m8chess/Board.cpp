/// @file   Board.cpp
/// @author Mathieu Pagé
/// @date   Feburary 2015
/// @brief  Contains the definition of methods of the Board class that 
///         represent the state of a chess board.

#include <sstream>

#include "../m8common/m8common.hpp"

#include "Board.hpp"
#include "Const.hpp"

namespace m8
{
    Board::Board(const std::string& fen,
                 eval::PieceSqTablePtr psqt)
        : psqt_(psqt),
          material_value_(0)
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
                Piece piece = GetPieceFromChar(*it);
                if (IsPiece(piece))
                {
                    // If the current position is invalid we throw an exception
                    if (!IsColmnOnBoard(colmn) || !IsRowOnBoard(row))
                    {
                        throw InvalFenError("Invalid piece placement section in fen string");
                    }

                    Sq sq = NewSq(colmn, row);
                    AddPiece(sq, piece);
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
                    sq_king = king_sq(color);
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

        // Consume space characters
        while (it < fen.end() && *it == ' ')
        {
            ++it;
        }

        // If we are not at the end of the string the next field is the 
        // move clock.
        if (it < fen.end())
        {
            std::string str;
            while (it < fen.end() &&
                '0' <= *it && *it <= '9')
            {
                str += *it;
                ++it;
            }

            full_move_clock_ = ConvertTo<std::uint32_t>(str);
        }
    }

    void Board::GenerateXFenPiecePlacement(std::ostream& out) const
    {
        uint32_t emptySquares = 0;

        for (Row row = kRow8; IsRowOnBoard(row); --row)
        {
            for (Colmn colmn = kColmnA; IsColmnOnBoard(colmn); ++colmn)
            {
                Sq sq = NewSq(colmn, row);
                Piece piece = (*this)[sq];
                if (IsPiece(piece))
                {
                    if (emptySquares > 0)
                    {
                        out << emptySquares;
                        emptySquares = 0;
                    }

                    out << GetCharFromPiece(piece);
                }
                else
                {
                    ++emptySquares;
                }
            }

            if (emptySquares > 0)
            {
                out << emptySquares;
                emptySquares = 0;
            }

            if (row > kRow1)
            {
                out << '/';
            }
        }
    }

    void Board::GenerateXFenActiveColour(std::ostream& out) const
    {
        out << (this->side_to_move() == kWhite ? 'w' : 'b');
    }

    bool Board::GenerateXFenCastling(std::ostream& out, Color color, CastleType castle) const
    {
        bool result = this->casle(color, castle);

        if (result)
        {
            Bb candidate_rooks = this->bb_piece(NewPiece(kRook, color));
            candidate_rooks &= kBbRow[GetColorWiseRow(color, kRow1)];
            Sq sq_outter_most_rook = (castle == kKingSideCastle ? GetMsb(candidate_rooks) : GetLsb(candidate_rooks));
            Colmn column_outter_most_rook = GetColmn(sq_outter_most_rook);

            char c;
            if (this->casle_colmn(castle) == column_outter_most_rook)
            {
                c = (castle == kKingSideCastle ? 'K' : 'Q');
            }
            else
            {
                c = static_cast<char>('A' + this->casle_colmn(castle));
            }

            if (color == kBlack)
            {
                c = static_cast<char>(tolower(c));
            }
            out << c;
        }

        return result;
    }

    void Board::GenerateXFenCastling(std::ostream& out) const
    {
        bool any_castle;

        any_castle = GenerateXFenCastling(out, kWhite, kKingSideCastle);
        any_castle |= GenerateXFenCastling(out, kWhite, kQueenSideCastle);
        any_castle |= GenerateXFenCastling(out, kBlack, kKingSideCastle);
        any_castle |= GenerateXFenCastling(out, kBlack, kQueenSideCastle);

        if (!any_castle)
        {
            out << '-';
        }
    }

    void Board::GenerateXFenEnPassant(std::ostream& out) const
    {
        if (IsColmnOnBoard(colmn_enpas_))
        {
            out << static_cast<char>('a' + colmn_enpas_)
                << static_cast<char>('1' + GetColorWiseRow(side_to_move_, kRow5));
        }
        else
        {
            out << '-';
        }
    }

    void Board::GenerateXFenClocks(std::ostream& out) const
    {
        out << half_move_clock_ << ' ' <<full_move_clock_;
    }

    std::string Board::fen() const
    {
        std::ostringstream out;

        GenerateXFenPiecePlacement(out);
        out << ' ';

        GenerateXFenActiveColour(out);
        out << ' ';

        GenerateXFenCastling(out);
        out << ' ';
        
        GenerateXFenEnPassant(out);
        out << ' ';

        GenerateXFenClocks(out);

        return out.str();
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
            for (auto piece_type : all_piece_types)
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
        full_move_clock_ = 0;
    }

    void DisplayPiece(std::ostream& out, Piece piece)
    {
        assert(IsPiece(piece));

        out << (GetColor(piece) == kBlack ? '=' : ' ')
            << GetCharFromPieceType(GetPieceType(piece))
            << (GetColor(piece) == kBlack ? '=' : ' ');
    }

    void DisplayEmptySq(std::ostream& out, Sq sq)
    {
        auto row = GetRow(sq);
        auto column = GetColmn(sq);

        if (((row & 1) == 1) == ((column & 1) == 1))
        {
            out << " . ";
        }
        else
        {
            out << "   ";
        }
    }

    void DisplaySq(std::ostream& out, Sq sq, Piece piece)
    {
        if (IsPiece(piece))
        {
            DisplayPiece(out, piece);
        }
        else
        {
            DisplayEmptySq(out, sq);
        }
    }

    /// Display the top row of the board. The top row contains an indicator if blacks 
    /// are on move. There is also indicator that indicated the remaining valid 
    /// castling starting locations for blacks.
    void DisplayColorRow(std::ostream& out, const Board& board, Color color)
    {
        out << (board.side_to_move() == color ? "=>" : "  ");
        for (auto column = kColmnA; IsColmnOnBoard(column); ++column)
        {
            out << "+-"
                << ((board.casle_colmn(kQueenSideCastle) == column && board.casle(color, kQueenSideCastle))
                    || (board.casle_colmn(kKingSideCastle) == column && board.casle(color, kKingSideCastle)) ? 'X' : '-')
                << '-';
        }
        out << "+\n";
    }

    void DisplayPriseEnPassantIndicator(std::ostream& out, const Board& board)
    {
        Colmn enpas = board.colmn_enpas();
        if (IsColmnOnBoard(enpas))
        {
            auto spaces = 4 + enpas * 4;
            for (int x = 0; x < spaces; ++x)
            {
                out << ' ';
            }

            out << "^\n";
        }
    }

    void DisplayHalfmoveClock(std::ostream& out, const Board& board)
    {
        out << "  (halfmove clock : " << board.half_move_clock() << ")\n";
    }

    void DisplayBoardContent(std::ostream& out, const Board& board)
    {
        for (auto row = kRow8; IsRowOnBoard(row); --row)
        {
            out << GetRowNumber(row) << ' ';

            for (auto column = kColmnA; IsColmnOnBoard(column); ++column)
            {
                out << '|';

                Sq sq = NewSq(column, row);
                Piece piece = board[sq];
                DisplaySq(out, sq, piece);
            }
            out << "|\n";

            if (kRow1 < row)
            {
                out << "  +---+---+---+---+---+---+---+---+\n";
            }
        }
    }

    void DisplayColumnsChar(std::ostream& out)
    {
        out << "    a   b   c   d   e   f   g   h";
    }

    std::ostream& operator<<(std::ostream& out, const Board& board)
    {
        DisplayHalfmoveClock(out, board);
        DisplayColorRow(out, board, kBlack);
        DisplayBoardContent(out, board);
        DisplayColorRow(out, board, kWhite);
        DisplayPriseEnPassantIndicator(out, board);
        DisplayColumnsChar(out);

        return out;
    }

    int Board::CalculateMaterialValue() const
    {
        int value = 0;
        for (auto piece : all_pieces)
        {
            Bb bb = bb_piece_[piece];
            while (bb)
            {
                Sq sq = RemoveLsb(bb);
                value += (*psqt_)[piece][sq];
            }
        }

        return value;
    }

    void Board::set_psqt(eval::PieceSqTablePtr psqt)
    {
        psqt_ = psqt;
        material_value_ = CalculateMaterialValue();
    }
}