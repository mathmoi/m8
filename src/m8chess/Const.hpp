/// @file   Const.hpp
/// @author Mathieu Pagé
/// @date   Feburary 2015
/// @brief  Contains constants variables, globales variables that acts like 
///         constants after their initialisations and functions that compute and
//          returns constants like values.

#ifndef M8_CONST_HPP_
#define M8_CONST_HPP_

#include <cassert>

#include "../m8common/m8common.hpp"

#include "Sq.hpp"

namespace m8
{
   /// Returns a bitboard with all the bits of a specific row set to 1.
   ///
   /// @param row The row.
   /// @return A bitboard
   inline Bb GetRowBb(Row row)
   {
      // A: The row in valid
      assert(row.IsOnBoard());

      return UINT64_C(0x00000000000000ff) << (row.value() * 8);
   }

   /// Returns a bitboard with all the bits of a specific column set to 1.
   ///
   /// @param colmn The column
   /// @return A bitboard
   inline Bb GetColmnBb(Column colmn) 
   {
      // A : The column is valid
      assert(colmn.IsOnBoard());

      return UINT64_C(0x101010101010101) << colmn.value();
   }
}



#endif // M8_CONST_HPP_
