/// @file   SAN.cpp
/// @author Mathieu Pagé
/// @date   Novembre 2017
/// @brief  Contains the definitions of method needed to parse and render moves in the
///         SAN notation.

#include <exception>
#include <sstream>

#include "SAN.hpp"
#include "Board.hpp"
#include "Direction.hpp"
#include "Checkmate.hpp"
#include "MoveGen.hpp"

namespace m8
{
    struct ParseInfo
    {
        std::string input;

        const char* begin;
        const char* end;

        Color side_to_move;

        Bb from_filter;

        Sq from;
        Sq to;
        Piece piece;
        Piece piece_taken;
        Piece promote_to;
        CastleType castling;

        inline ParseInfo(std::string p_input, Color p_side_to_move)
        {
            input = p_input;
            begin = input.c_str();
            end = begin + input.length();

            from = kInvalSq;
            to = kInvalSq;
            piece = kNoPiece;
            piece_taken = kNoPiece;
            promote_to = kNoPiece;
            castling = kNoCastling;

            side_to_move = p_side_to_move;

            from_filter = kFilledBb;
        };

        inline bool IsConsumed() const { return end <= begin; };
        inline std::string remaining() const { return std::string(begin, end - begin); };
        inline char first() const { return *begin; }
        inline char last() const { return *(end - 1); }
    };

    void PrepareCastlingMove(const m8::Board & board, m8::ParseInfo & info, CastleType castling)
    {
        Color side_to_move = board.side_to_move();
        Colmn colmn = castling == kQueenSideCastle ? kColmnC : kColmnG;

        info.piece = NewPiece(kKing, side_to_move);
        info.from = GetLsb(board.bb_piece(info.piece));
        info.to = NewSq(colmn, GetColorWiseRow(side_to_move, kRow1));
        info.castling = castling;
    }

    bool ParseCastlingMove(ParseInfo& info, const Board& board)
    {
        if (info.remaining() == "O-O-O")
        {
            PrepareCastlingMove(board, info, kQueenSideCastle);
            return true;
        }
        else if (info.remaining() == "O-O")
        {
            PrepareCastlingMove(board, info, kKingSideCastle);
            return true;
        }
        else
        {
            return false;
        }
    }

    Row ReadDestinationRow(ParseInfo& info)
    {
        if (info.IsConsumed())
        {
            throw InvalidSANMoveException("Unexpected end of string in SAN move.");
        }

        if (info.last() < '1' || info.last() > '8')
        {
            throw InvalidSANMoveException("Unexpected character in SAN move.");
        }

        Row row = NewRow(info.last());
        --info.end;

        return row;
    }

    Colmn ReadDestinationColmn(ParseInfo& info)
    {
        if (info.IsConsumed())
        {
            throw InvalidSANMoveException("Unexpected end of string in SAN move.");
        }

        if (info.last() < 'a' || info.last() > 'h')
        {
            throw InvalidSANMoveException("Unexpected character in SAN move.");
        }

        Colmn colmn =  NewColmn(info.last());
        --info.end;

        return colmn;
    }

    void ReadDestination(ParseInfo& info)
    {
        Row row = ReadDestinationRow(info);
        Colmn colmn = ReadDestinationColmn(info);

        info.to = NewSq(colmn, row);
    }

    void ReadMovedPiece(ParseInfo& info)
    {
        PieceType piece_type = kPawn;

        if (!info.IsConsumed())
        {
            piece_type = GetPieceTypeFromChar(info.first());
            if (IsPieceType(piece_type))
            {
                ++info.begin;
            }
            else
            {
                piece_type = kPawn;
            }
        }

        info.piece = NewPiece(piece_type, info.side_to_move);
    }

    Bb GetPawnMoveCandidates(ParseInfo& info, const Board& board)
    {
        Bb bb = GetSingleBitBb(info.to);
        bb = (info.side_to_move == kWhite ? bb >> 8 : bb << 8);
        Bb candidates = bb & board.bb_piece(info.piece);

        bb &= ~board.bb_occupied();
        bb &= kBbRow[GetColorWiseRow(info.side_to_move, kRow3)];
        bb = (info.side_to_move == kWhite ? bb >> 8 : bb << 8);
        candidates |= bb & board.bb_piece(info.piece);

        return candidates;
    }

    /// IF candidate are pinned_candidates we keep them only if the ray defined by
    /// the piece and the destination is the same as the ray defined by the piece
    /// and it's king. This way the piece still protects it's king.
    Bb RemovePinnedCandidates(Bb candidates, Color color, Sq to, const Board& board)
    {
        Bb pinned_candidates = candidates & GetPinnedPieces(board, color);
        if (pinned_candidates)
        {
            Sq king_sq = board.king_sq(color);
            
            do
            {
                Sq from = RemoveLsb(pinned_candidates);

                if (GetDirection(from, to) != GetDirection(from, king_sq))
                {
                    UnsetBit(candidates, from);
                }
            } while (pinned_candidates);
        }

        return candidates;
    }

    void GetOriginSq(ParseInfo& info, const Board& board)
    {
        Bb candidates;

        if (!IsPiece(info.piece_taken) && GetPieceType(info.piece) == kPawn)
        {
            candidates = GetPawnMoveCandidates(info, board);
        }
        else
        {
            candidates = GenerateAttacksTo(board, info.piece, info.to);
        }

        candidates &= info.from_filter;
        candidates = RemovePinnedCandidates(candidates, info.side_to_move, info.to, board);

        if (GetPopct(candidates) != 1)
        {
            throw InvalidSANMoveException();
        }

        info.from = GetLsb(candidates);
    }

    bool IsEnPassant(ParseInfo& info, const Board& board)
    {
        return GetPieceType(info.piece) == kPawn &&
               GetColmn(info.to) == board.colmn_enpas() &&
               GetRow(info.to) == GetColorWiseRow(info.side_to_move, kRow6);
    }

    bool IsCapture(ParseInfo& info, const Board& board)
    {
        return IsPiece(board[info.to]) || IsEnPassant(info, board);
    }

    void ReadCapture(ParseInfo& info, const Board& board)
    {
        if (IsCapture(info, board))
        {
            info.piece_taken = board[info.to];
            if (!IsPiece(info.piece_taken))
            {
                // Prise en passant
                info.piece_taken = NewPiece(kPawn, OpposColor(info.side_to_move));
            }
        }

        if (!info.IsConsumed() && info.last() == 'x')
        {
            --info.end;
        }
    }

    void ReadPromotion(ParseInfo& info)
    {
        // If the last character is a piece it's the promotion piece.
        PieceType promotion = GetPieceTypeFromChar(info.last());
        if (IsPieceType(promotion))
        {
            info.promote_to = NewPiece(promotion, info.side_to_move);
            --info.end;

            if (info.last() == '=')
            {
                --info.end;
            }
        }
    }

    void SkipMateAndCheckCharacters(ParseInfo& info)
    {
        while (info.last() == '+' || info.last() == '#')
        {
            --info.end;
        }
    }

    void ReadDisambiguation(ParseInfo& info)
    {
        // If the first character is a column, it's the origin column.
        if (!info.IsConsumed() && 'a' <= info.first() && info.first() <= 'h')
        {
            Colmn colmn = NewColmn(info.first());
            info.from_filter = kBbColmn[colmn];
            ++info.begin;
        }

        // If the first character is a row, it's the origin row.
        if (!info.IsConsumed() && '1' <= info.first() && info.first() <= '8')
        {
            Row row = NewRow(info.first());
            info.from_filter &= kBbRow[row];
            ++info.begin;
        }
    }
    
    Move ParseSAN(std::string input, const Board& board)
    {
        bool done = false;

        ParseInfo info(input, board.side_to_move());

        SkipMateAndCheckCharacters(info);

        done = ParseCastlingMove(info, board);

        if (!done)
        {
            ReadPromotion(info);
            ReadDestination(info);
            ReadMovedPiece(info);
            ReadCapture(info, board);
            ReadDisambiguation(info);
            GetOriginSq(info, board);
        }

        return NewMove(info.from, info.to, info.piece, info.piece_taken, info.promote_to, info.castling);
    }

    void OutputCharPieceForSan(std::ostream& out, Move move)
    {
        PieceType piece_type = GetPieceType(GetPiece(move));
        if (piece_type != kPawn)
        {
            out << GetCharFromPieceType(piece_type);
        }
    }

    void OutputDestination(std::ostream& out, Move move)
    {
        out << SqToString(GetTo(move));
    }

    void OutputCastling(std::ostream& out, CastleType type)
    {
        out << (type == kKingSideCastle ? "O-O" : "O-O-O");
    }

    void OutputCaptureCharacter(std::ostream& out, Move move)
    {
        Piece taken = GetPieceTaken(move);
        if (IsPiece(taken))
        {
            out << 'x';
        }
    }

    void OutputPawnCaptureOriginColumn(std::ostream& out, Move move)
    {
        if (GetPieceType(GetPiece(move)) == kPawn &&
            IsPiece(GetPieceTaken(move)))
        {
            out << GetColumnChar(GetColmn(GetFrom(move)));
        }
    }

    void OutputPromotion(std::ostream& out, Move move)
    {
        Piece promote_to = GetPromoteTo(move);
        if (IsPiece(GetPromoteTo(move)))
        {
            PieceType piece_type = GetPieceType(promote_to);
            out << '=' << GetCharFromPieceType(piece_type);
        }
    }

    void OutputCheckAndMateCharacter(std::ostream& out, Move move, const Board& board)
    {
        // We remove the constness of the board in order to modify it to check for Mat. 
        // This is not great, but we promise to put it back like it was when we are done 
        // with it.
        Board& modifiable_board = const_cast<Board&>(board);

        Color color_after_move = OpposColor(GetColor(GetPiece(move)));

        UnmakeInfo unmake_info = modifiable_board.Make(move);
        if (IsInCheck(color_after_move, board))
        {
            out << (IsMat(modifiable_board) ? '#' : '+');
        };
        modifiable_board.Unmake(move, unmake_info);
    }

    void OutputDisambiguationCharacters(std::ostream& out, Move move, const Board& board)
    {
        Piece piece = GetPiece(move);
        if (GetPieceType(piece) != kPawn)
        {
            Color color = GetColor(piece);
            Sq to = GetTo(move);
            Sq from = GetFrom(move);
            Colmn colmn = GetColmn(from);
            Row row = GetRow(from);

            Bb candidates = GenerateAttacksTo(board, piece, to);
            candidates = RemovePinnedCandidates(candidates, color, to, board);

            if (GetPopct(candidates) > 1)
            {
                if (GetPopct(candidates & GetColmnBb(colmn)) == 1)
                {
                    out << GetColumnChar(colmn);
                }
                else if (GetPopct(candidates & GetRowBb(row)) == 1)
                {
                    out << GetRowNumber(row);
                }
                else
                {
                    out << GetColumnChar(colmn) << GetRowNumber(row);
                }
            }
        }
    }

    std::string RenderSAN(Move move, const Board& board)
    {
        std::ostringstream out;

        CastleType castle_type = GetCastling(move);
        if (castle_type == kNoCastling)
        {
            OutputCharPieceForSan(out, move);
            OutputPawnCaptureOriginColumn(out, move);
            OutputDisambiguationCharacters(out, move, board);
            OutputCaptureCharacter(out, move);
            OutputDestination(out, move);
            OutputPromotion(out, move);
            OutputCheckAndMateCharacter(out, move, board);
        }
        else
        {
            OutputCastling(out, castle_type);
        }

        return out.str();
    }
}