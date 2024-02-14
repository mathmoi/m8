/// @file   PieceSqTable.cpp
/// @author Mathieu Pagé
/// @date   Feburary 2018
/// @brief  Contains functionalities about the piece-square table evaluation features.

#include "m8chess/eval/PieceSqTable.hpp"
#include "m8chess/Board.hpp"
#include "m8common/options/Options.hpp"

namespace m8::eval {

    PieceSqTablePtr GenerateEmptyPieceSqTable()
    {
        auto psqt = std::make_shared<PieceSqTable>();
        for (size_t index_piece = 0; index_piece < psqt->size(); ++index_piece)
        {
            for (size_t sq = 0; sq < kNumSqOnBoard; ++sq)
            {
                (*psqt)[index_piece][sq] = 0;
            }
        }

        return psqt;
    }

    /// Apply a value to a piece-square table.
    ///
    /// @param zone  Bitboard representing the squares to which to apply the value.
    /// @param value Value to add to the squares of the zone.
    /// @param color Color of the player for which we apply the values. If the color is
    ///              black the row of the squares will be reversed.
    /// @param psqt  Piece-Square table to which we apply the zone.
    void ApplyZoneToPsqt(Bb zone, int value, Color color, SinglePieceSqTable& psqt)
    {
        while (zone)
        {
            Sq sq = RemoveLsb(zone);
            sq = GetColorWiseSq(color, sq);
            psqt[sq] += value;
        }
    }

    /// Apply all the zones to the piece-square table of a piece.
    void ApplyZonesToPsqt(PieceType piece_type, Color color, std::vector<options::PsqtZone> zones, PieceSqTablePtr psqt)
    {
        auto piece = NewPiece(piece_type, color);

        SinglePieceSqTable& single_piece_psqt = psqt->at(piece);
        int multiplier = (color == kWhite ? 1 : -1);
        single_piece_psqt.fill(multiplier * options::Options::get().eval.get_value_piece_type(piece_type));

        for (auto zone : zones)
        {
            ApplyZoneToPsqt(zone.zone, zone.value * multiplier, color, single_piece_psqt);
        }
    }

    PieceSqTablePtr GeneratePieceSqTable()
    {
        auto psqt = std::make_shared<PieceSqTable>();

        for (Color color = kMinColor; IsColor(color); ++color)
        {   
            for (auto piece_zones : options::Options::get().eval.psqt_zones)
            {
                ApplyZonesToPsqt(piece_zones.first, color, piece_zones.second, psqt);
            }
        }

        return psqt;
    }
}