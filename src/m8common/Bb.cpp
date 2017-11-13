/// @file   Bb.cpp
/// @author Mathieu Pagé
/// @date   December 2015
/// @brief  Contains the definition of types and utilities used to manipulates 
///         bitboards. 
///         Bitboards are 64 bits integers that represents a state of the board 
///         one bit per square. 

#include "Bb.hpp"

namespace m8
{
    std::array<Bb, 240> kBbBetweenArray;
    Bb* kBbBetween;

    void InitializeBbBetween()
    {
        const std::uint8_t kA1 = 0;
        const std::uint8_t kA3 = 16;
        const std::uint8_t kA8 = 56;
        const std::uint8_t kC1 = 2;
        const std::uint8_t kC3 = 18;
        const std::uint8_t kH1 = 7;
        const std::uint8_t kH8 = 63;

        /* Initialize the pointer */
        kBbBetween = kBbBetweenArray.data() + 120;

        /* first initalize all entry to zero */
        for (std::ptrdiff_t x = 0; x < 240; ++x)
        {
            kBbBetweenArray[x] = EmptyBb;
        }

        // We generate all the difference variation with a1 as the from square. For the
        // destination square we go north, northeast and east. For each pair generated 
        // this way can also compute it's reverse by swaping the origin and destination 
        // square.

        Bb between = EmptyBb;
        for (std::uint8_t to = kA3; to <= kA8; to += 8)
        {
            between.Set(to - 8);
            kBbBetween[CalculateOx88diff(kA1, to)] = between;            
            kBbBetween[CalculateOx88diff(to, kA1)] = between;
            kBbBetween[CalculateOx88diff(to, kA1)].RotateRight(to);
        }

        between = EmptyBb;
        for (std::uint8_t to = kC3; to <= kH8; to += 9)
        {
            between.Set(to - 9);
            kBbBetween[CalculateOx88diff(kA1, to)] = between;
            kBbBetween[CalculateOx88diff(to, kA1)] = between;
            kBbBetween[CalculateOx88diff(to, kA1)].RotateRight(to);
        }

        between = EmptyBb;
        for (std::uint8_t to = kC1; to <= kH1; to += 1)
        {
            between.Set(to - 1);
            kBbBetween[CalculateOx88diff(kA1, to)] = between;
            kBbBetween[CalculateOx88diff(to, kA1)] = between;
            kBbBetween[CalculateOx88diff(to, kA1)].RotateRight(to);
        }
    }

    void BbInitializePreCalc()
    {
        InitializeBbBetween();
    }

    std::ostream& operator <<(std::ostream& out, Bb bb)
    {
        const std::uint8_t kRow8 = 7;
        const std::uint8_t kColmnA = 0;
        const std::uint8_t kColmnH = 8;

        for (std::uint8_t row = kRow8; row < 8; --row)
        {
            out << '\t';
            for (std::uint8_t colmn = kColmnA; colmn < 8; ++colmn)
            {
                std::uint8_t sq = (row << 3) + colmn;
                out << (bb[sq] ? 'X' : '-');

                if (colmn < kColmnH)
                    out << ' ';
            }

            out << std::endl;
        }

        return out;
    }
}