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