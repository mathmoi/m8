/// @file   Bb.cpp
/// @author Mathieu Pagé
/// @date   December 2015
/// @brief  Contains the definition of types and utilities used to manipulates 
///         bitboards. 
///         Bitboards are 64 bits integers that represents a state of the board 
///         one bit per square. 

#include "Bb.hpp"
#include "Sq.hpp"

namespace m8
{
    std::array<Bb, 240> kBbBetweenArray;
    Bb* kBbBetween;

    Bb DistributeBits(Bb bits, Bb mask)
    {
        Bb result = kEmptyBb;
        uint64_t bits_count = GetPopct(mask);

        for (uint32_t x = 0; x < bits_count; ++x)
        {
            uint32_t pos = RemoveLsb(mask);
            if (GetBit(bits, x))
            {
                SetBit(result, pos);
            }
        }

        return result;
    }

    void InitializeBbBetween()
    {
        /* Initialize the pointer */
        kBbBetween = kBbBetweenArray.data() + 120;

        /* first initalize all entry to zero */
        for (ptrdiff_t x = 0; x < 240; ++x)
        {
            kBbBetweenArray[x] = kEmptyBb;
        }

        // We generate all the difference variation with a1 as the from square. For the
        // destination square we go north, northeast and east. For each pair generated 
        // this way can also compute it's reverse by swaping the origin and destination 
        // square.

        Bb between = kEmptyBb;
        for (Sq to = kA3; to <= kA8; to += 8)
        {
            SetBit(between, to - 8);
            kBbBetween[CalculateOx88diff(kA1, to)] = between;
            kBbBetween[CalculateOx88diff(to, kA1)] = RotateRight(between, to);
        }

        between = kEmptyBb;
        for (Sq to = kC3; to <= kH8; to += 9)
        {
            SetBit(between, to - 9);
            kBbBetween[CalculateOx88diff(kA1, to)] = between;
            kBbBetween[CalculateOx88diff(to, kA1)] = RotateRight(between, to);
        }

        between = kEmptyBb;
        for (Sq to = kC1; to <= kH1; to += 1)
        {
            SetBit(between, to - 1);
            kBbBetween[CalculateOx88diff(kA1, to)] = between;
            kBbBetween[CalculateOx88diff(to, kA1)] = RotateRight(between, to);
        }
    }

    void BbInitializePreCalc()
    {
        InitializeBbBetween();
    }

	void DisplayBb(std::ostream& out, Bb bb)
	{
		for (Row row = kRow8; IsRowOnBoard(row); --row)
		{
			out << '\t';
			for (Colmn colmn = kColmnA; IsColmnOnBoard(colmn); ++colmn)
			{
				Sq sq = NewSq(colmn, row);
				out << (GetBit(bb, sq) ? 'X' : '-');

				if (colmn < kColmnH)
					out << ' ';
			}

			out << std::endl;
		}
	}
}