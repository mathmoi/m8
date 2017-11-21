/// @file   Board.hpp
/// @author Mathieu Pag�
/// @date   Feburary 2015
/// @brief  Contains the definition of the Board type that represent the state 
///         of a chess board.

#ifndef M8_BOARD_HPP_
#define M8_BOARD_HPP_

#include <array>
#include <cstdint>
#include <cstdlib>
#include <string>

#include "../m8common/m8common.hpp"

#include "Piece.hpp"
#include "Sq.hpp"
#include "Move.hpp"

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

    /// Type used for variables containing a castle type
    typedef std::uint8_t CastleType;

    /// @name Castle flags constant
    /// Constant used as flags for the castling rights
    /// @{
    const CastleType kQueenSideCastle = 1;
    const CastleType kKingSideCastle = 2;
    /// @}

    /// FEN string representing the starting position
    const std::string kStartingPositionFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    /// Type for the information used to unmake a move.
    typedef std::uint32_t UnmakeInfo; // TODO : Make a class?

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
        /// @param sq Index of the square we want to get the piece from.
        /// @returns The piece that is on the square passed in parameter.
        inline Piece operator[](Sq sq) const;

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
        inline Bb UINT64_Color(Color color) const;

        /// Accessor for the bitboard of all occupied squares
        ///
        /// @returns A bitboard with bits to 1 for every position where there is a piece.
        inline Bb bb_occupied() const;

        /// Accessor for the castlings rights.
        /// 
        /// @param color Side for which we want the castling rights
        /// @param castle_right Castling right to read. Valid values are 
        ///        kQueenSideCastle and kKingSideCastle
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
        inline Column casle_colmn(std::size_t indx) const;

        /// Accessor for the column of the pawn that can be captured en passant.
        ///
        /// @returns A column. If no piece can be captured en passant an invalid 
        ///          column is returned.
        inline Column colmn_enpas() const { return colmn_enpas_; };

        /// Mutator for the column en passant.
        ///
        /// @param colmn_enpass Column of the piece that can be captured en 
        ///        passant or an invalid column if no piece can be captured en 
        ///        passant.
        inline void set_colmn_enpas(Column colmn_enpas) { colmn_enpas_ = colmn_enpas; };

        /// Accessor for the half move clock.
        ///
        /// @returns The number of moves since the last pawn push or the 
        ///          last capture.
        inline std::uint32_t half_move_clock() const { return half_move_clock_; };

        /// Mutator for the half move clock.
        ///
        /// @param value New value for the half move clock.
        inline void set_half_move_clock(std::uint32_t value) { half_move_clock_ = value; };

        /// Accessor for the full move clock.
        ///
        /// @returns The number of moves played.
        inline std::uint32_t full_move_clock() const { return full_move_clock_; };

        /// Mutator for the full move clock.
        ///
        /// @param value New value for the full move clock.
        inline void set_full_move_clock(std::uint32_t value) { full_move_clock_ = value; };

        /// <summary>
        ///  Returns a fen string representing the current position on the board.
        /// </summary>
        /// <returns>A fen string</returns>
        std::string fen() const;

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

        /// Execute a move on the board.
        ///
        /// @param move Move to make
        /// @return Info that need to be passed to Unmake in order to unmake the move.
        inline UnmakeInfo Make(Move move);

        /// Unmake a move previously made on the board.
        ///
        /// @param move        Move to unmake.
        /// @param unmake_info Informations used to unmake the move produced when the move
        ///                    was made.
        inline void Unmake(Move move, UnmakeInfo unmake_info);

    private:

        /// Initialize the board with no pieces.
        void Clear();

        /// Array containing the piece on each square of the board.
        std::array<Piece, Sq::NumSqOnBoard> board_;

        /// Color that has to move next
        Color side_to_move_;

        /// Bitboard for each piece
        std::array<Bb, kMaxPiece + 1> bb_piece_;

        /// Bitboard for each color
        std::array<Bb, 2> bb_color_;

        /// Original column of the two rooks. This is used to determine which 
        /// rook can castle because in Chess960 the rook may not start on 
        /// the column a and h.
        std::array<Column, 2> casle_colmn_;

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
        Column colmn_enpas_;

        /// Number of moves since the last pawn push or the last capture.
        std::uint32_t half_move_clock_;

        /// <summary>
        ///  Number of moves played.
        /// </summary>
        std::uint32_t full_move_clock_;

        /// Move a piece on the board.
        ///
        /// @param from  Position of the piece before the move.
        /// @param to    Postion of the piece after the move.
        /// @param piece Piece moved.
        inline void MovePiece(Sq from, Sq to, Piece piece);

        /// Execute a move on the board.
        ///
        /// @param from  Origin of the move
        /// @param to    Destination of the move
        /// @param piece Piece moved
        /// @param taken Piece taken if the move is a capture
        inline void MakeSimpleMove(Sq from, Sq to, Piece piece, Piece taken);

        /// Execute a pawn move on the board.
        ///
        /// @param from       Origin of the move
        /// @param to         Destination of the move
        /// @param piece      Piece moved
        /// @param taken      Piece taken if the move is a capture
        /// @param promote_to Indicate that what piece to promote to if the move is a 
        ///                   promotion.
        inline void MakePawnMove(Sq from, Sq to, Piece piece, Piece taken, Piece promote_to);

        /// Execute a castling move on the board.
        ///
        /// @param from   Origin of the move
        /// @param to     Destination of the move
        /// @param piece  Piece moved (king)
        /// @param castle Side of the castling
        inline void MakeCastlingMove(Sq from, Sq to, Piece piece, CastleType castle);

        /// Execute a rook move on the board.
        ///
        /// @param from   Origin of the move
        /// @param to     Destination of the move
        /// @param piece  Piece moved (rook)
        /// @param taken  Piece taken if any.
        inline void MakeRookMove(Sq from, Sq to, Piece piece, Piece taken);

        /// Execute a king move on the board.
        ///
        /// @param from   Origin of the move
        /// @param to     Destination of the move
        /// @param piece  Piece moved (rook)
        /// @param taken  Piece taken if any.
        /// @param castle Side of the castling if any
        inline void MakeKingMove(Sq from, Sq to, Piece piece, Piece taken, CastleType castle);

        /// Unmake a move on the board.
        ///
        /// @param from  Origin of the move
        /// @param to    Destination of the move
        /// @param piece Piece moved
        /// @param taken Piece taken if the move is a capture
        inline void UnmakeSimpleMove(Sq from, Sq to, Piece piece, Piece taken);

        /// Unmake a castling move on the board.
        ///
        /// @param from   Origin of the move
        /// @param to     Destination of the move
        /// @param piece  Piece moved (king)
        /// @param castle Side of the castling
        inline void UnmakeCastlingMove(Sq from, Sq to, Piece piece, CastleType castle);

        /// Unmake a king move on the board.
        ///
        /// @param from   Origin of the move
        /// @param to     Destination of the move
        /// @param piece  Piece moved (rook)
        /// @param taken  Piece taken if any.
        /// @param castle Side of the castling if any
        inline void UnmakeKingMove(Sq from, Sq to, Piece piece, Piece taken, CastleType castle);

        /// Unmake a pawn move on the board.
        ///
        /// @param from       Origin of the move
        /// @param to         Destination of the move
        /// @param piece      Piece moved
        /// @param taken      Piece taken if the move is a capture
        /// @param promote_to Indicate that what piece to promote to if the move is a 
        ///                   promotion.
        inline void UnmakePawnMove(Sq from, Sq to, Piece piece, Piece taken, Piece promote_to);

        /// <summary>
        ///  Generate the piece placement part of fen string.
        /// </summary>
        /// <param name="out">
        ///  Stream on which to generate the field.
        /// </param>
        void GenerateXFenPiecePlacement(std::ostream& out) const;

        /// <summary>
        ///  Generate the active colour field of a fen string
        /// </summary>
        /// <param name="out">
        ///  Stream on which to generate the field.
        /// </param>
        void GenerateXFenActiveColour(std::ostream& out) const;

        /// <summary>
        ///  Generate one character of the castling field of a XFen string.
        /// </summary>
        /// <param name="out">
        ///  Stream on which to generate the field.
        /// </param>
        /// <param name="color">Color of the castle to generate</param>
        /// <param name="castle">Side of the castle to generate</param>
        /// <returns>
        ///  Returns true to indicate that the castling right was allowed, false 
        ///  otherwise.
        /// </returns>
        bool GenerateXFenCastling(std::ostream& out, Color color, CastleType castle) const;

        /// <summary>
        ///  Generate the castling field of a fen string
        /// </summary>
        /// <param name="out">
        ///  Stream on which to generate the field.
        /// </param>
        void GenerateXFenCastling(std::ostream& out) const;

        /// <summary>
        ///  Generate the en passant field of a fen string
        /// </summary>
        /// <param name="out">
        ///  Stream on which to generate the field.
        /// </param>
        void GenerateXFenEnPassant(std::ostream& out) const;

        /// <summary>
        ///  Generate the halfmove clock and the fullmove number fields of a fen string
        /// </summary>
        /// <param name="out">
        ///  Stream on which to generate the field.
        /// </param>
        void GenerateXFenClocks(std::ostream& out) const;

    };

    /// Overloading the << operator for an output stream and a Board. This 
    /// allow to output a representation of the board on a stream.
    ///
    /// @param out   Stream
    /// @param board Board to output
    /// @returns     A reference to the stream received in parameter
    std::ostream& operator<<(std::ostream& out, const Board& board);

    inline Piece Board::operator[](Sq sq) const
    {
        // A : l'index est valide
        assert(sq.IsOnBoard());

        return board_[sq.value()];
    }

    inline void Board::set_side_to_move(Color side_to_move)
    {
        // A : The side to move is a valid color
        assert(side_to_move.IsColor());

        side_to_move_ = side_to_move;
    }

    inline Bb Board::bb_piece(Piece piece) const
    {
        // A : The piece is valid
        assert(IsPiece(piece));

        return bb_piece_[piece];
    }

    inline Bb Board::UINT64_Color(Color color) const
    {
        // A : The color is valid
        assert(color.IsColor());

        return bb_color_[color.value()];
    }

    inline Bb Board::bb_occupied() const
    {
        return bb_color_[Color::White().value()] | bb_color_[Color::Black().value()];
    }

    inline bool Board::casle(Color color, std::uint8_t casle_right) const
    {
        // A : The color and castling rights are valids
        assert(color.IsColor());
        assert(casle_right == kQueenSideCastle || casle_right == kKingSideCastle);

        return (casle_flag_ & (casle_right << color.value() << color.value())) != 0;
    }

    inline void Board::set_casle(Color color, std::uint8_t casle_right, bool value)
    {
        // A : The inputs are valids
        assert(color.IsColor());
        assert(casle_right == kQueenSideCastle || casle_right == kKingSideCastle);

        std::uint8_t mask = casle_right << color.value() << color.value();
        casle_flag_ ^= (-static_cast<std::uint8_t>(value) ^ casle_flag_) & mask;
    }

    inline Column Board::casle_colmn(std::size_t indx) const
    {
        // A : The index is 0 or 1.
        assert(indx == 0 || indx == 1);

        return casle_colmn_[indx];
    }

    inline void Board::AddPiece(Sq sq, Piece piece)
    {
        // A : sq is a valid square, piece is a valid piece and the destination
        //     square is empty
        assert(sq.IsOnBoard());
        assert(IsPiece(piece));
        assert((*this)[sq] == kNoPiece);

        board_[sq.value()] = piece;

        bb_piece_[piece].Set(sq.value());

        Color color = GetColor(piece);
        bb_color_[color.value()].Set(sq.value());
    }

    inline void Board::RemovePiece(Sq sq)
    {
        // A : sq is a valid square and the square is not empty.
        assert(sq.IsOnBoard());
        assert(IsPiece((*this)[sq]));

        Color color = GetColor((*this)[sq]);
        bb_color_[color.value()].Unset(sq.value());

        bb_piece_[(*this)[sq]].Unset(sq.value());

        board_[sq.value()] = kNoPiece;
    }

    inline void Board::MovePiece(Sq from, Sq to, Piece piece)
    {
        // A : from and to are valid squares. From contains a piece and to is empty.
        assert(from.IsOnBoard());
        assert(to.IsOnBoard());
        assert(IsPiece(board_[from.value()]));
        assert(board_[from.value()] == piece);
        assert(board_[to.value()] == kNoPiece);

        board_[from.value()] = kNoPiece;
        board_[to.value()] = piece;

        Bb diff = Bb::GetSingleBitBb(from.value());
        diff.Set(to.value());

        bb_color_[GetColor(piece).value()] = bb_color_[GetColor(piece).value()] ^ diff;
        bb_piece_[piece] = bb_piece_[piece] ^ diff;
    }

    inline void Board::MovePiece(Sq from, Sq to)
    {
        Piece piece = board_[from.value()];
        MovePiece(from, to, piece);
    }

    inline void Board::MakeSimpleMove(Sq from, Sq to, Piece piece, Piece taken)
    {
        assert(from.IsOnBoard());
        assert(to.IsOnBoard());
        assert(IsPiece(piece));
        assert(board_[from.value()] == piece);
        assert(board_[to.value()] == taken);

        if (taken != kNoPiece)
        {
            RemovePiece(to);
            half_move_clock_ = 0;
        }

        MovePiece(from, to, piece);
    }
    
    inline void Board::MakePawnMove(Sq from, Sq to, Piece piece, Piece taken, Piece promote_to)
    {
        half_move_clock_ = 0;

        // If the piece taken is not on the target square it must be a prise-en-passant
        if (board_[to.value()] != taken)
        {
            assert(board_[to.value()] == kNoPiece);

            Sq pos_taken(to.column(), from.row());

            assert(board_[pos_taken.value()] == NewPiece(kPawn, GetColor(piece).opposite()));

            RemovePiece(pos_taken);
            MakeSimpleMove(from, to, piece, kNoPiece);
        }
        else if (IsPiece(promote_to))
        {
            if (taken != kNoPiece)
            {
                RemovePiece(to);
            }
            RemovePiece(from);
            AddPiece(to, promote_to);
        }
        else
        {
            MakeSimpleMove(from, to, piece, taken);
        }

        // If the move is a two square move we need to set the en-passant column.
        if (std::abs(to.value() - from.value()) == 16)
        {
            colmn_enpas_ = to.column();
        }
    }

    inline void Board::MakeCastlingMove(Sq from, Sq to, Piece piece, CastleType castle)
    {
        // A : The castling is allowed
        assert(this->casle(side_to_move_, castle));
        assert(side_to_move_ == GetColor(piece));

        Piece rook = NewPiece(kRook, side_to_move_);
        Column rook_colmn = casle_colmn_[castle - 1];
        Row row = from.row();
        Sq rook_from(rook_colmn, row);
        Sq rook_to(castle == kKingSideCastle ? Column::F() : Column::D(), row);

        MovePiece(from, to, piece);
        MovePiece(rook_from, rook_to, rook);
    }

    inline void Board::MakeRookMove(Sq from, Sq to, Piece piece, Piece taken)
    {
        assert(GetPieceType(piece) == kRook);

        Row first_row = Row::_1().color_wise(side_to_move_);

        if (from == Sq(casle_colmn_[0], first_row))
        {
            set_casle(side_to_move_, kQueenSideCastle, false);
        }
        else if (from == Sq(casle_colmn_[1], first_row))
        {
            set_casle(side_to_move_, kKingSideCastle, false);
        }

        MakeSimpleMove(from, to, piece, taken);
    }

    inline void Board::MakeKingMove(Sq from, Sq to, Piece piece, Piece taken, CastleType castle)
    {
        if (castle != 0)
        {
            MakeCastlingMove(from, to, piece, castle);
        }
        else
        {
            MakeSimpleMove(from, to, piece, taken);
        }

        this->set_casle(side_to_move_, kQueenSideCastle, false);
        this->set_casle(side_to_move_, kKingSideCastle, false);
    }

    inline UnmakeInfo Board::Make(Move move)
    {
        assert(GetColor(GetPiece(move)) == side_to_move_);

        Sq from = GetFrom(move);
        Sq to = GetTo(move);
        Piece piece = GetPiece(move);
        Piece taken = GetPieceTaken(move);
        PieceType piece_type = GetPieceType(piece);

        UnmakeInfo unmake_info = colmn_enpas_.value() << 24 | half_move_clock_;

        // If the side to move is black increment the move number
        full_move_clock_ += side_to_move_.value();

        ++half_move_clock_;
        colmn_enpas_ = Column::Invalid();

        switch (piece_type)
        {
        case kPawn:
        {
            Piece promote_to = GetPromoteTo(move);
            MakePawnMove(from, to, piece, taken, promote_to);
        }
            break;

        case kRook:
            MakeRookMove(from, to, piece, taken);
            break;

        case kKing:
        {
            CastleType castle = GetCastling(move);
            MakeKingMove(from, to, piece, taken, castle);
        }
            break;

        default:
            MakeSimpleMove(from, to, piece, taken);
            break;
        }

        side_to_move_ = side_to_move_.opposite();

        return unmake_info;
    }

    inline void Board::UnmakeSimpleMove(Sq from, Sq to, Piece piece, Piece taken)
    {
        assert(from.IsOnBoard());
        assert(to.IsOnBoard());
        assert(IsPiece(piece));
        assert(board_[to.value()] == piece);
        assert(board_[from.value()] == kNoPiece);

        MovePiece(to, from, piece);

        if (taken != kNoPiece)
        {
            AddPiece(to, taken);
        }
    }

    inline void Board::UnmakeCastlingMove(Sq from, Sq to, Piece piece, CastleType castle)
    {
        Piece rook = NewPiece(kRook, side_to_move_.opposite());
        Column rook_colmn = casle_colmn_[castle - 1];
        Row row = from.row();
        Sq rook_from(rook_colmn, row);
        Sq rook_to(castle == kKingSideCastle ? Column::F() : Column::D(), row);

        MovePiece(to, from, piece);
        MovePiece(rook_to, rook_from, rook);
    }

    inline void Board::UnmakeKingMove(Sq from, Sq to, Piece piece, Piece taken, CastleType castle)
    {
        if (castle != 0)
        {
            UnmakeCastlingMove(from, to, piece, castle);
        }
        else
        {
            UnmakeSimpleMove(from, to, piece, taken);
        }
    }

    inline void Board::UnmakePawnMove(Sq from, Sq to, Piece piece, Piece taken, Piece promote_to)
    {
        Row row_enpas = Row::_6().color_wise(side_to_move_.opposite());

        if (colmn_enpas_.IsOnBoard() &&
            taken == NewPiece(kPawn, side_to_move_) &&
            to == Sq(colmn_enpas_, row_enpas))
        {
            UnmakeSimpleMove(from, to, piece, kNoPiece);
            AddPiece(Sq(colmn_enpas_, Row::_5().color_wise(side_to_move_.opposite())), taken);
        }
        else if (IsPiece(promote_to))
        {
            RemovePiece(to);
            AddPiece(from, piece);
            
            if (taken != kNoPiece)
            {
                AddPiece(to, taken);
            }
        }
        else
        {
            UnmakeSimpleMove(from, to, piece, taken);
        }
    }

    inline void Board::Unmake(Move move, UnmakeInfo unmake_info)
    {
        assert(GetColor(GetPiece(move)) == side_to_move_.opposite());

        Sq from = GetFrom(move);
        Sq to = GetTo(move);
        Piece piece = GetPiece(move);
        Piece taken = GetPieceTaken(move);
        PieceType piece_type = GetPieceType(piece);

        half_move_clock_ = unmake_info & 0xFFFFFF;
        colmn_enpas_ = unmake_info >> 24;

        switch (piece_type)
        {
        case kPawn:
        {
            Piece promote_to = GetPromoteTo(move);
            UnmakePawnMove(from, to, piece, taken, promote_to);
        }
        break;

        case kKing:
        {
            CastleType castle = GetCastling(move);
            UnmakeKingMove(from, to, piece, taken, castle);
        }
        break;

        default:
            UnmakeSimpleMove(from, to, piece, taken);
            break;
        }

        side_to_move_ = side_to_move_.opposite();

        // If the side to move is black decrement the move number.
        full_move_clock_ -= side_to_move_.value();
    }
}

#endif // M8_BOARD_HPP_