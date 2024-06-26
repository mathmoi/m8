/// @file   Board.hpp
/// @author Mathieu Pagé
/// @date   Feburary 2015
/// @brief  Contains the definition of the Board type that represent the state 
///         of a chess board.

#ifndef M8_BOARD_HPP_
#define M8_BOARD_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>

#include "m8common/Bb.hpp"

#include "eval/GamePhase.hpp"
#include "eval/PieceSqTable.hpp"

#include "transposition/Zobrist.hpp"

#include "Move.hpp"
#include "Piece.hpp"
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

    /// Type used for variables containing a castle type
    typedef std::uint8_t CastleType;

    /// @name Castle flags constant
    /// Constant used as flags for the castling rights
    /// @{
    const CastleType kNoCastling = 0;
    const CastleType kQueenSideCastle = 1;
    const CastleType kKingSideCastle = 2;
    /// @}

    /// FEN string representing the starting position
    const std::string kStartingPositionFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    /// Type for the information used to unmake a move.
    typedef std::uint32_t UnmakeInfo;

    /// Represent the state of a chess board. This include the pieces positions,
    /// the side to move next, the castling rights and the en-passant square.
    class Board
    {
    public:
        
        /// Constructor from an X-FEN string. An X-FEN string is a fen string
        /// with added functionalities to be used in Fischer Random Chess. A FEN
        /// string is a string that fully represent the state of a chess board.
        ///
        /// For more information check here:
        /// http://chessprogramming.wikispaces.com/Forsyth-Edwards+Notation
        ///
        /// @param fen   X-FEN string that represent the board state to recreate.
        /// @param psqt  Pointer to a piece-square table to use for material evaluation 
        ///              by the board. The board is responsible for this part of the
        ///              evaluation for performance reasons.
        Board(const std::string_view fen);

        /// Index operator. Returns the piece that is on a given square.
        ///
        /// @param index Index of the square we want to get the piece from.
        /// @returns The piece that is on the square passed in parameter.
        inline Piece operator[](std::size_t index) const;

        /// Accessor for the side to move.
        ///
        /// @returns The side to move next.
        inline Color side_to_move() const { return side_to_move_; };

        /// Switch the side to move
        inline void SwitchSideToMove();

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
        /// @param castle_type Index of the column to get, must be kQueendSide or kKingSide.
        /// @returns A column.
        inline Colmn casle_colmn(CastleType castle_type) const;

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
        inline void set_colmn_enpas(Colmn colmn_enpas)
        {
            // We "remove" the en passant column from the hash key if it was set.
            if (IsColmnOnBoard(colmn_enpas_))
            {
                hash_key_ ^= transposition::gZobristEnPassant[colmn_enpas_];
            }

            colmn_enpas_ = colmn_enpas;

            // We add the en passant column to the hash key if it's set.
            if (IsColmnOnBoard(colmn_enpas_))
            {
                hash_key_ ^= transposition::gZobristEnPassant[colmn_enpas_];
            }
        };

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

        /// Returns the position of the king of the given color.
        inline Sq king_sq(Color color) const { return GetLsb(bb_piece(NewPiece(kKing, color))); };

        /// Returns a fen string representing the current position on the board.
        std::string fen() const;

        /// Return the hash key of the current position
        transposition::ZobristKey hash() const { return hash_key_; }

        /// Value of the material on the board. Based on the piece-square table values.
        inline int material_value() const;

        /// Returns true if the position can be claimed as a draw because of a repetition 
        /// or the 50 move rule.
        ///
        /// This method returns true if a position is repeated twice instead of three
        /// times. This is done so that in the search, if the engine find it can repeat a
        /// position twice, it will immediately evaluate it as a draw without having to
        /// search four more plies to detect the 3-fold repetition. The idea is that if
        /// the opponents let's us repeat the position once, it will let us repeat twice.
        /// In the same way, if we can't prevent a second repetition it means we can't 
        /// prevent a third one either.
        inline bool is_draw() const;

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

        /// Number of moves since the last pawn push or the last capture.
        std::uint32_t half_move_clock_;

        ///  Number of moves played.
        std::uint32_t full_move_clock_;

        /// Evaluation of the piece-square table in the middle game.
        EvalType material_middle_game_;

        /// Evaluation of the piece-square table in the middle end game.
        EvalType material_end_game_;

        /// Value to extrapolate the game phase
        eval::GamePhaseEstimate game_phase_estimate_;

        /// Hash of the current position
        transposition::ZobristKey hash_key_;

        /// History of the position during the game
        std::vector<transposition::ZobristKey> positions_history_;

        /// Initialize the board with no pieces.
        void Clear();

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

        /// Generate the piece placement part of fen string.
        /// 
        /// @param out  Stream on which to generate the field.
        void GenerateXFenPiecePlacement(std::ostream& out) const;

        /// Generate the active colour field of a fen string
        /// 
        /// @param out  Stream on which to generate the field.
        void GenerateXFenActiveColour(std::ostream& out) const;

        /// Generate one character of the castling field of a XFen string.
        /// 
        /// @param out      Stream on which to generate the field.
        /// @param color    Color of the castle to generate
        /// @param castle   Side of the castle to generate<
        /// @returns        Returns true to indicate that the castling right was allowed,
        ///                 false otherwise.
        bool GenerateXFenCastling(std::ostream& out, Color color, CastleType castle) const;

        /// Generate the castling field of a fen string
        /// 
        /// @param out      Stream on which to generate the field.
        void GenerateXFenCastling(std::ostream& out) const;

        /// Generate the en passant field of a fen string
        /// 
        /// @param out      Stream on which to generate the field.
        void GenerateXFenEnPassant(std::ostream& out) const;

        /// Generate the halfmove clock and the fullmove number fields of a fen string
        /// 
        /// @oaram out      Stream on which to generate the field.
        void GenerateXFenClocks(std::ostream& out) const;

        /// Remove the appropriate castling right if a rook is captured.
        ///
        /// @param taken Piece taken.
        /// @param to    Target square of the move.
        inline void RemoveCastlingRookCaptured(Piece taken, Sq to);
    };

    /// Overloading the << operator for an output stream and a Board. This 
    /// allow to output a representation of the board on a stream.
    ///
    /// @param out   Stream
    /// @param board Board to output
    /// @returns     A reference to the stream received in parameter
    std::ostream& operator<<(std::ostream& out, const Board& board);

    inline Piece Board::operator[](std::size_t index) const
    {
        // A : l'index est valide
        assert(index < kNumSqOnBoard);

        return board_[index];
    }

    inline void Board::SwitchSideToMove()
    {
        hash_key_ ^= transposition::gZobristBlackToMove;
        side_to_move_ = OpposColor(side_to_move_);
    }

    inline void Board::set_side_to_move(Color side_to_move)
    {
        // A : The side to move is a valid color
        assert(IsColor(side_to_move));

        if (side_to_move != side_to_move_)
        {
            hash_key_ ^= transposition::gZobristBlackToMove;
        }

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

    inline Bb Board::bb_occupied() const
    {
        return bb_color_[kWhite] | bb_color_[kBlack];
    }

    inline bool Board::casle(Color color, std::uint8_t casle_right) const
    {
        // A : The color and castling rights are valids
        assert(IsColor(color));
        assert(casle_right == kQueenSideCastle || casle_right == kKingSideCastle);

        return (casle_flag_ & (casle_right << color << color)) != 0;
    }

    inline void Board::set_casle(Color color, std::uint8_t casle_right, bool value)
    {
        // A : The inputs are valids
        assert(IsColor(color));
        assert(casle_right == kQueenSideCastle || casle_right == kKingSideCastle);

        hash_key_ ^= transposition::gZobristCastling[casle_flag_];

        std::uint8_t mask = casle_right << color << color;
        casle_flag_ ^= (-static_cast<std::uint8_t>(value) ^ casle_flag_) & mask;

        hash_key_ ^= transposition::gZobristCastling[casle_flag_];
    }

    inline Colmn Board::casle_colmn(CastleType castle_type) const
    {
        // A : The index is 0 or 1.
        assert(castle_type == kKingSideCastle || castle_type == kQueenSideCastle);

        return casle_colmn_[castle_type - 1];
    }

    inline int Board::material_value() const
    {
        int middle_game_fraction = std::min(game_phase_estimate_, eval::kGamePhaseEstimateMax);
        int end_game_fraction = eval::kGamePhaseEstimateMax - middle_game_fraction;
        return (material_middle_game_ * middle_game_fraction + material_end_game_ * end_game_fraction) / static_cast<int>(eval::kGamePhaseEstimateMax);
    }

    inline bool Board::is_draw() const
    {
        // If there has been more than 50 reversibles moves (100 half moves) the position is draw.
        if (100 <= half_move_clock_)
        {
            return true;
        }

        // We check for repetition in the position_history_ table. We don't need to check
        // past the half_move_clock_, because a repetition can't occure after an
        // irreversible move. We only check each other position, because a repetition
        // can't occure when the same side is not on move.
        auto min_index = std::max(static_cast<std::int32_t>(positions_history_.size()) - static_cast<std::int32_t>(half_move_clock_), 0);
        for (int i = positions_history_.size() - 2; min_index <= i; i -= 2)
        {
            if (positions_history_[i] == hash_key_)
            {
                return true;
            }
        }

        return false;
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

        game_phase_estimate_  += eval::kPiecePhaseEstimate[piece];
        material_middle_game_ += eval::gPieceSqTable[static_cast<int>(eval::GamePhase::MiddleGame)][piece][sq];
        material_end_game_    += eval::gPieceSqTable[static_cast<int>(eval::GamePhase::EndGame)][piece][sq];

        hash_key_ ^= transposition::gZobristTable[piece][sq];
    }

    inline void Board::RemovePiece(Sq sq)
    {
        // A : sq is a valid square and the square is not empty.
        assert(IsSqOnBoard(sq));
        assert(IsPiece(board_[sq]));

        Piece piece = board_[sq];

        Color color = GetColor(piece);
        UnsetBit(bb_color_[color], sq);

        UnsetBit(bb_piece_[piece], sq);

        game_phase_estimate_  -= eval::kPiecePhaseEstimate[piece];
        material_middle_game_ -= eval::gPieceSqTable[static_cast<int>(eval::GamePhase::MiddleGame)][piece][sq];
        material_end_game_    -= eval::gPieceSqTable[static_cast<int>(eval::GamePhase::EndGame)][piece][sq];

        hash_key_ ^= transposition::gZobristTable[piece][sq];

        board_[sq] = kNoPiece;
    }

    inline void Board::MovePiece(Sq from, Sq to, Piece piece)
    {
        // A : from and to are valid squares. From contains a piece and to is empty.
        assert(IsSqOnBoard(from));
        assert(IsSqOnBoard(to));
        assert(IsPiece(board_[from]));
        assert(board_[from] == piece);
        assert(board_[to] == kNoPiece);

        board_[from] = kNoPiece;
        board_[to] = piece;

        Bb diff = GetSingleBitBb(from);
        SetBit(diff, to);

        bb_color_[GetColor(piece)] ^= diff;
        bb_piece_[piece] ^= diff;

        material_middle_game_ += eval::gPieceSqTable[static_cast<int>(eval::GamePhase::MiddleGame)][piece][to]
                               - eval::gPieceSqTable[static_cast<int>(eval::GamePhase::MiddleGame)][piece][from];
        material_end_game_    += eval::gPieceSqTable[static_cast<int>(eval::GamePhase::EndGame)][piece][to]
                               - eval::gPieceSqTable[static_cast<int>(eval::GamePhase::EndGame)][piece][from];

        hash_key_ ^= transposition::gZobristTable[piece][to] 
                  ^  transposition::gZobristTable[piece][from] ;
    }

    inline void Board::MovePiece(Sq from, Sq to)
    {
        Piece piece = board_[from];
        MovePiece(from, to, piece);
    }

    inline void Board::MakeSimpleMove(Sq from, Sq to, Piece piece, Piece taken)
    {
        assert(IsSqOnBoard(from));
        assert(IsSqOnBoard(to));
        assert(IsPiece(piece));
        assert(board_[from] == piece);
        assert(board_[to] == taken);

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
        if (board_[to] != taken)
        {
            assert(board_[to] == kNoPiece);

            Sq pos_taken = NewSq(GetColmn(to), GetRow(from));

            assert(board_[pos_taken] == NewPiece(kPawn, OpposColor(GetColor(piece))));

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
        if (std::abs(to - from) == 16)
        {
            set_colmn_enpas(GetColmn(to));
        }
    }

    inline void Board::MakeCastlingMove(Sq from, Sq to, Piece piece, CastleType castle)
    {
        // A : The castling is allowed
        assert(this->casle(side_to_move_, castle));
        assert(side_to_move_ == GetColor(piece));

        Piece rook = NewPiece(kRook, side_to_move_);
        Colmn rook_colmn = casle_colmn_[castle - 1];
        Row row = GetRow(from);
        Sq rook_from = NewSq(rook_colmn, row);
        Sq rook_to = NewSq(castle == kKingSideCastle ? kF1 : kD1, row);

        RemovePiece(from);
        RemovePiece(rook_from);
        AddPiece(to, piece);
        AddPiece(rook_to, rook);
    }

    inline void Board::MakeRookMove(Sq from, Sq to, Piece piece, Piece taken)
    {
        assert(GetPieceType(piece) == kRook);

        Row first_row = GetColorWiseRow(side_to_move_, kRow1);

        if (from == NewSq(casle_colmn_[0], first_row))
        {
            set_casle(side_to_move_, kQueenSideCastle, false);
        }
        else if (from == NewSq(casle_colmn_[1], first_row))
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

    inline void Board::RemoveCastlingRookCaptured(Piece taken, Sq to)
    {
        if (IsPiece(taken) && GetPieceType(taken) == kRook)
        {
            Color color = GetColor(taken);
            Row origin_row = GetColorWiseRow(color, kRow1);

            if (GetRow(to) == origin_row)
            {
                Colmn colmn = GetColmn(to);
                if (colmn == casle_colmn(kQueenSideCastle))
                {
                    set_casle(color, kQueenSideCastle, false);
                }
                if (colmn == casle_colmn(kKingSideCastle))
                {
                    set_casle(color, kKingSideCastle, false);
                }
            }
        }
    }

    inline UnmakeInfo Board::Make(Move move)
    {
        assert(GetColor(GetPiece(move)) == side_to_move_);

        positions_history_.push_back(hash_key_);

        Sq from = GetFrom(move);
        Sq to = GetTo(move);
        Piece piece = GetPiece(move);
        Piece taken = GetPieceTaken(move);
        PieceType piece_type = GetPieceType(piece);

        UnmakeInfo unmake_info = colmn_enpas_ << 24 | casle_flag_ << 20 | half_move_clock_;

        // If the side to move is black increment the move number
        full_move_clock_ += side_to_move_;

        ++half_move_clock_;
        set_colmn_enpas(kInvalColmn);

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

        RemoveCastlingRookCaptured(taken, to);

        SwitchSideToMove();

        return unmake_info;
    }

    inline void Board::UnmakeSimpleMove(Sq from, Sq to, Piece piece, Piece taken)
    {
        assert(IsSqOnBoard(from));
        assert(IsSqOnBoard(to));
        assert(IsPiece(piece));
        assert(board_[to] == piece);
        assert(board_[from] == kNoPiece);

        MovePiece(to, from, piece);

        if (taken != kNoPiece)
        {
            AddPiece(to, taken);
        }
    }

    inline void Board::UnmakeCastlingMove(Sq from, Sq to, Piece piece, CastleType castle)
    {
        Piece rook = NewPiece(kRook, OpposColor(side_to_move_));
        Colmn rook_colmn = casle_colmn_[castle - 1];
        Row row = GetRow(from);
        Sq rook_from = NewSq(rook_colmn, row);
        Sq rook_to = NewSq(castle == kKingSideCastle ? kF1 : kD1, row);

        RemovePiece(to);
        RemovePiece(rook_to);
        AddPiece(from, piece);
        AddPiece(rook_from, rook);
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
        Row row_enpas = GetColorWiseRow(OpposColor(side_to_move_), kRow6);

        if (IsColmnOnBoard(colmn_enpas_) &&
            taken == NewPiece(kPawn, side_to_move_) &&
            to == NewSq(colmn_enpas_, row_enpas))
        {
            UnmakeSimpleMove(from, to, piece, kNoPiece);
            AddPiece(NewSq(colmn_enpas_, GetColorWiseRow(OpposColor(side_to_move_), kRow5)), taken);
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
        assert(GetColor(GetPiece(move)) == OpposColor(side_to_move_));

        positions_history_.pop_back();

        Sq from = GetFrom(move);
        Sq to = GetTo(move);
        Piece piece = GetPiece(move);
        Piece taken = GetPieceTaken(move);
        PieceType piece_type = GetPieceType(piece);

        half_move_clock_ = unmake_info & 0xFFFFF;
        set_colmn_enpas(unmake_info >> 24);

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

        SwitchSideToMove();

        hash_key_ ^= transposition::gZobristCastling[casle_flag_];
        casle_flag_ = (unmake_info >> 20) & 0xF;
        hash_key_ ^= transposition::gZobristCastling[casle_flag_];

        // If the side to move is black decrement the move number.
        full_move_clock_ -= side_to_move_;
    }
}

#endif // M8_BOARD_HPP_