/// @file   Bb.hpp
/// @author Mathieu Pagé
/// @date   Feburary 2015
/// @brief  Contains the definition of types and utilities used to manipulates 
///         bitboards. 
///         Bitboards are 64 bits integers that represents a state of the board 
///         one bit per square. 

#ifndef M8_BB_HPP_
#define M8_BB_HPP_

#include <cstdint>

// Determine if we can use the MSVC++ Intrinsics.
#ifdef _MSC_VER
#  ifdef _WIN64
#     include <intrin.h>
#     pragma intrinsic(_BitScanForward64)
#     pragma intrinsic(_BitScanReverse64)
#     define M8_USE_MSC_INTRINSICS____
#  endif
#endif

// Determine if we are doing a 64 bit compilation
#if defined(_WIN64) || defined(__LP64__)
#  define M8_64_BITS_COMPILE____
#endif

namespace m8
{
   /// Type that represents a bitboard.
   typedef std::uint64_t Bb;

   /// Returns the state of a bit in a bitboard.
   ///
   /// @param bb  The bitboard read the bit from.
   /// @param bit The position of the bit to read.
   /// @returns True if the bit is set to 1.
   inline bool GetBit(Bb bb, std::uint32_t bit)
   {
      return (bb & (UINT64_C(1) << bit)) != 0;
   }

   /// Set the specified bit to one in a bitboard.
   ///
   /// @param bb  The bitboard to modify.
   /// @param bit The position of the bit to set.
   inline void SetBit(Bb& bb, std::uint32_t bit)
   {
      bb |= (UINT64_C(1) << bit);
   }

   /// Set the specified bit to zero in a bitboard.
   ///
   /// @param bb  The bitboard to modify.
   /// @param bit The position of the bit to set.
   inline void UnsetBit(Bb& bb, std::uint32_t bit)
   {
      bb &= ~(UINT64_C(1) << bit);
   }

   /// Switch the value of the specified bit to zero in a bitboard.
   ///
   /// @param bb  The bitboard to modify.
   /// @param bit The position of the bit to set.
   inline void SwitchBit(Bb& bb, std::uint32_t bit)
   {
      bb ^= (UINT64_C(1) << bit);
   }

   /// Get the position of the least significant bit set to 1 in a bitboard.
   ///
   /// @param bb Bitboard to read from.
   /// @return Position of the least significatn bit set to 1.
   inline std::uint32_t GetLsb(Bb bb)
   {
#if defined(M8_USE_MSC_INTRINSICS)
      // This is the intrinsics version of GetLsb used with VC++
      unsigned long index;
      _BitScanForward64(&index, bb);
      return index;
#elif defined(M8_64_BITS_COMPILE)
      // This is a 64 bit friendly of GetLsb using debruijn multiplication
      const unsigned int index64[64] =
      {
         63, 0, 58, 1, 59, 47, 53, 2,
         60, 39, 48, 27, 54, 33, 42, 3,
         61, 51, 37, 40, 49, 18, 28, 20,
         55, 30, 34, 11, 43, 14, 22, 4,
         62, 57, 46, 52, 38, 26, 32, 41,
         50, 36, 17, 19, 29, 10, 13, 21,
         56, 45, 25, 31, 35, 16, 9, 12,
         44, 24, 15, 8, 23, 7, 6, 5
      };

      const std::uint64_t debruijn64 = UINT64_C(0x07EDD5E59A4E28C2);
      return index64[((bb & -static_cast<std::int64_t>(bb)) * debruijn64) >> 58];
#else
      // This is the 32 bits friendly version
      const unsigned int lsz64_tbl[64] =
      {
         63,30, 3,32,59,14,11,33,
         60,24,50, 9,55,19,21,34,
         61,29, 2,53,51,23,41,18,
         56,28, 1,43,46,27, 0,35,
         62,31,58, 4, 5,49,54, 6,
         15,52,12,40, 7,42,45,16,
         25,57,48,13,10,39, 8,44,
         20,47,38,22,17,37,36,26,
      };

      unsigned int folded;
      bb ^= (bb - 1);
      folded = (int)bb ^ (bb >> 32);
      return lsz64_tbl[folded * 0x78291ACF >> 26];
#endif
   }
}

#endif // M8_BB_HPP_