/// @file   XRay.hpp
/// @author Mathieu Pag�
/// @date   Janurary 2018
/// @brief  Contains methods to compute XRay attacks. XRay attacks are attacks by sliders
///         "through" a defending play. For exemple, a rook on a1 would xray-attack a king
///         on h1 if there is one and only one piece protecting the king somewhere on the
///         first rank.

#include <cstdint>

#include "m8common/Bb.hpp"

#include "movegen/Attacks.hpp"

#include "Sq.hpp"

#ifndef M8_X_RAY_HPP_
#define M8_X_RAY_HPP_

namespace m8
{
    /// Given a board occupancy, returns the x-ray attacks of a piece on a given 
    /// squares. X-ray attacks are squares "attacked" by a piece through another
    /// piece. Theses are not real attacks because the blocking piece protext the 
    /// so-called attacked square.
    ///
    /// @param occ       Bitboard representing the occupancy of the board.
    /// @param blockers  Positions of the piece to considered blockers.
    /// @param sq        Postion of the bishop.
    template<Bb(*GenerateAttacks)(Bb, Sq)>
    inline static Bb GeneratePieceXRay(Bb occ, Bb blockers, Sq sq)
    {
        Bb attacks = GenerateAttacks(occ, sq);
        blockers &= attacks;
        occ ^= blockers;
        Bb attaksWhithoutBlockers = GenerateAttacks(occ, sq);

        return attacks ^ attaksWhithoutBlockers;
    }

    /// Given a board occupancy, returns the x-ray attacks of a rook on a given 
    /// squares. X-ray attacks are squares "attacked" by a piece through another
    /// piece. Theses are not real attacks because the blocking piece protext the 
    /// so-called attacked square.
    ///
    /// @param occ       Bitboard representing the occupancy of the board.
    /// @param blockers  Positions of the piece to considered blockers.
    /// @param sq        Postion of the bishop.
    inline Bb GenerateRookXRay(Bb occ, Bb blockers, Sq sq)
    {
        return GeneratePieceXRay<movegen::GenerateRookAttacks>(occ, blockers, sq);
    }

    /// Given a board occupancy, returns the x-ray attacks of a bishop on a given 
    /// squares. X-ray attacks are squares "attacked" by a piece through another
    /// piece. Theses are not real attacks because the blocking piece protext the 
    /// so-called attacked square.
    ///
    /// @param occ       Bitboard representing the occupancy of the board.
    /// @param blockers  Positions of the piece to considered blockers.
    /// @param sq        Postion of the bishop.
    inline Bb GenerateBishopXRay(Bb occ, Bb blockers, Sq sq)
    {
        return GeneratePieceXRay<movegen::GenerateBishopAttacks>(occ, blockers, sq);
    }    
}

#endif // M8_X_RAY_HPP_