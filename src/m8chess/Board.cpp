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
    Board::Board()
        : side_to_move_(0)
    {
        Clear();
    }

    Board::Board(const std::string&  fen)
        : side_to_move_(0)
    {
        Clear();

        std::string::const_iterator it = fen.begin();

        // First we consume the piece placement section
        Column column = Column::A();
        Row row = Row::_8();
        while (it < fen.end() && *it != ' ')
        {
            // If the character is a digit we increment the current column by this
            // digit value.
            if (*it >= '0' && *it <= '9')
            {
                column += *it - '0';
            }
            else
            {
                // If the character is a piece we add it to the board at the 
                // current position.
                auto it_piece = char_to_piece_map.find(*it);
                if (it_piece != char_to_piece_map.end())
                {
                    // If the current position is invalid we throw an exception
                    if (!column.IsOnBoard() || !row.IsOnBoard())
                    {
                        throw InvalFenError("Invalid piece placement section in fen string");
                    }

                    Sq sq(column, row);
                    AddPiece(sq, it_piece->second);
                    column = column.MoveRight();
                }
                else
                {
                    // If the character is a '/' we move to the next row
                    if (*it == '/')
                    {
                        column = Column::A();
                        row = row.MoveDown();
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
                set_side_to_move(Color::White());
                ++it;
            }
            else
            {
                if (*it == 'b')
                {
                    set_side_to_move(Color::Black());
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
                Column column;
                uint8_t casle_right;
                Bb bb_rook;
                Sq sq_rook;
                Sq sq_king;

                auto color = isupper(*it) ? Color::White() : Color::Black();
                bb_rook = bb_piece(NewPiece(kRook, color)) & GetRowBb(Row::_1().color_wise(color).value());

                if (*it == 'Q' || *it == 'q')
                {
                    sq_rook = bb_rook.GetLSB();
                    casle_right = kQueenSideCastle;
                }
                else if (*it == 'K' || *it == 'k')
                {
                    sq_rook = bb_rook.GetMSB();
                    casle_right = kKingSideCastle;
                }
                else if ((tolower(*it) >= 'a' && tolower(*it) <= 'h'))
                {
                    sq_rook = Sq(tolower(*it) - 'a', Row::_1().color_wise(color));
                    sq_king = bb_piece(NewPiece(kKing, color)).GetLSB();
                    casle_right = (sq_rook.column() < sq_king.column() ? kQueenSideCastle : kKingSideCastle);
                }
                else
                {
                    // We are unable to parse the castling rights, we raise an exception;
                    throw InvalFenError("Unable to read the castinling rights from the fen string.");
                }

                column = sq_rook.column();
                casle_colmn_[casle_right == kQueenSideCastle ? 0 : 1] = column;
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

        for (Row row = Row::_8(); row.IsOnBoard(); row = row.MoveDown())
        {
            for (Column column = Column::A(); column.IsOnBoard(); column = column.MoveRight())
            {
                Sq sq(column, row);
                Piece piece = (*this)[sq];
                if (IsPiece(piece))
                {
                    if (emptySquares > 0)
                    {
                        out << emptySquares;
                        emptySquares = 0;
                    }

                    out << piece_to_char_map.find(piece)->second;
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

            if (row > Row::_1())
            {
                out << '/';
            }
        }
    }

    void Board::GenerateXFenActiveColour(std::ostream& out) const
    {
        out << (this->side_to_move() == Color::White() ? 'w' : 'b');
    }

    bool Board::GenerateXFenCastling(std::ostream& out, Color color, CastleType castle) const
    {
        size_t castle_index = castle - 1;
        bool result = this->casle(color, castle);

        if (result)
        {
            Bb candidate_rooks = this->bb_piece(NewPiece(kRook, color));
            candidate_rooks &= kBbRow[Row::_1().color_wise(color).value()];
            Sq sq_outter_most_rook = (castle == kKingSideCastle ? candidate_rooks.GetMSB() : candidate_rooks.GetLSB());
            Column column_outter_most_rook = sq_outter_most_rook.column();

            char c;
            if (this->casle_colmn(castle_index) == column_outter_most_rook)
            {
                c = (castle == kKingSideCastle ? 'K' : 'Q');
            }
            else
            {
                c = static_cast<char>('A' + this->casle_colmn(castle_index).value());
            }

            if (color == Color::Black())
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

        any_castle = GenerateXFenCastling(out, Color::White(), kKingSideCastle);
        any_castle |= GenerateXFenCastling(out, Color::White(), kQueenSideCastle);
        any_castle |= GenerateXFenCastling(out, Color::Black(), kKingSideCastle);
        any_castle |= GenerateXFenCastling(out, Color::Black(), kQueenSideCastle);

        if (!any_castle)
        {
            out << '-';
        }
    }

    void Board::GenerateXFenEnPassant(std::ostream& out) const
    {
        if (colmn_enpas_.IsOnBoard())
        {
            out << static_cast<char>('a' + colmn_enpas_.value())
                << static_cast<char>('1' + Row::_5().color_wise(side_to_move_).value());
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
        side_to_move_ = Color::White();

        // Initialize the board_
        for (Sq sq = 0; sq.IsOnBoard(); sq = sq.MoveNext())
        {
            board_[sq.value()] = kNoPiece;
        }

        // Initialize the pieces bitboards
        for (Color color = Color::First(); color.IsColor(); color = color.Next())
        {
            for (PieceType piece_type = kMinPieceType; IsPieceType(piece_type); ++piece_type)
            {
                Piece piece = NewPiece(piece_type, color);
                bb_piece_[piece] = Bb::Empty();
            }
        }

        // Initialize the color bitboards
        bb_color_[Color::White().value()] = Bb::Empty();
        bb_color_[Color::Black().value()] = Bb::Empty();

        // Initialize the castle columns. By default we use the regular chess columns.
        casle_colmn_[0] = Column::A();
        casle_colmn_[1] = Column::H();

        // Initialize the castle flags, the column of the en-passant capture and 
        // the half move clock.
        casle_flag_ = 0;
        colmn_enpas_ = Column::Invalid();
        half_move_clock_ = 0;
        full_move_clock_ = 0;
    }

    void DisplayPiece(std::ostream& out, Piece piece)
    {
        assert(IsPiece(piece));

        out << (GetColor(piece) == Color::Black() ? '=' : ' ')
            << static_cast<char>(toupper(piece_to_char_map.find(piece)->second))
            << (GetColor(piece) == Color::Black() ? '=' : ' ');
    }

    void DisplayEmptySq(std::ostream& out, Sq sq)
    {
        auto row = sq.row();
        auto column = sq.column();

        if (((row.value() & 1) == 1) == ((column.value() & 1) == 1))
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
        for (auto column = Column::A(); column.IsOnBoard(); column = column.MoveRight())
        {
            out << "+-"
                << ((board.casle_colmn(0) == column && board.casle(color, kQueenSideCastle))
                    || (board.casle_colmn(1) == column && board.casle(color, kKingSideCastle)) ? 'X' : '-')
                << '-';
        }
        out << "+\n";
    }

    void DisplayPriseEnPassantIndicator(std::ostream& out, const Board& board)
    {
        Column enpas = board.colmn_enpas();
        if (enpas.IsOnBoard())
        {
            auto spaces = 4 + enpas.value() * 4;
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
        for (auto row = Row::_8(); row.IsOnBoard(); row = row.MoveDown())
        {
            out << row.number() << ' ';

            for (auto column = Column::A(); column.IsOnBoard(); column = column.MoveRight())
            {
                out << '|';

                Sq sq(column, row);
                Piece piece = board[sq];
                DisplaySq(out, sq, piece);
            }
            out << "|\n";

            if (Row::_1() < row)
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
        DisplayColorRow(out, board, Color::Black());
        DisplayBoardContent(out, board);
        DisplayColorRow(out, board, Color::White());
        DisplayPriseEnPassantIndicator(out, board);
        DisplayColumnsChar(out);

        return out;
    }
}