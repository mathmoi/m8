/// @file   Attacks.hpp
/// @author Mathieu Pag�
/// @date   Janurary 2018
/// @brief  Contains methods and array used to generate attacks bitboards.

#include <cstdint>

#include "../m8common/m8common.hpp"
#include "Sq.hpp"

#ifndef M8_ATTACKS_HPP_
#define M8_ATTACKS_HPP_


namespace m8
{
    /// Structure that hold all the parameters required to do the move generation of a 
    /// rook or bishop on a given square.
    struct Magic
    {
        Bb* attack;
        Bb mask;
        Bb magic;
        std::uint32_t shift;
    };

    /// Type for an attack array for simples moves (knight and kings).
    typedef std::array<Bb, 64> AttackArray;

    /// Type for an array of Magic structures.
    typedef std::array<Magic, 64> MagicArray;

    extern const std::array<const Bb, 64> kRookMagics;
    extern const std::array<const std::uint32_t, 64> kRookMagicShifts;
    extern const std::array<const Bb, 64> kBishopMagics;
    extern const std::array<const std::uint32_t, 64> kBishopMagicShifts;

    extern AttackArray knight_attack_bb;
    extern AttackArray king_attack_bb;
    extern std::array<Bb, 102400> rook_attack_bb;
    extern std::array<Bb, 5248> bishop_attack_bb;
    extern MagicArray rook_magic;
    extern MagicArray bishop_magic;

    /// Initialize the precalculated data associated with the MoveGen class. This 
    /// method should be called once before the class can be used.
    void InitializeAttacks();

    /// Given a board occupancy and the magic precalculation array, returns the 
    /// squares attacked by a slider (rook or bishop) on a given square.
    ///
    /// @param magics Precalculation for the magic bitboard move generation for the 
    ///               slider type. If the magics precalculation for a rook are passed
    ///               this method will generate attacks for a rook.
    /// @param occ    Bitboard representing the occupancy of the board.
    /// @param sq     Postion of the slider.
    /// @return       A bitboard representing all squares attacked by the slider.
    inline Bb GenerateSliderAttacks(const MagicArray& magics, Bb occ, Sq sq)
    {
        const Magic&  magic = magics[sq];
        occ &= magic.mask;
        occ *= magic.magic;
        occ >>= magic.shift;
        return  magic.attack[occ];
    }

    /// Given a board occupancy, returns the squares attacked by a rook on a given 
    /// square.
    ///
    /// @param occ Bitboard representing the occupancy of the board.
    /// @param sq  Postion of the rook.
    /// @return    A bitboard representing all squares attacked by the rook.
    inline Bb GenerateRookAttacks(Bb occ, Sq sq)
    {
        return GenerateSliderAttacks(rook_magic, occ, sq);
    }

    /// Given a board occupancy, returns the squares attacked by a bishop on a given 
    /// square.
    ///
    /// @param occ Bitboard representing the occupancy of the board.
    /// @param sq  Postion of the bishop.
    /// @return    A bitboard representing all squares attacked by the bishop.
    inline Bb GenerateBishopAttacks(Bb occ, Sq sq)
    {
        return GenerateSliderAttacks(bishop_magic, occ, sq);
    }    
}

#endif // M8_ATTACKS_HPP_