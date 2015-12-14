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
#include <cassert>

// Determine if we can use the MSVC++ Intrinsics.
#ifdef _MSC_VER
#  ifdef _WIN64
#     include <intrin.h>
#     pragma intrinsic(_BitScanForward64)
#     pragma intrinsic(_BitScanReverse64)
#     define M8_USE_MSC_INTRINSICS
#  endif
#endif

// Determine if we can use the g++ asm instruction
#if defined(__GNUC__) && defined(__LP64__)
#  define M8_USE_GCC_ASSEMBLY
#endif

// Determine if we are doing a 64 bit compilation
#if defined(_WIN64) || defined(__LP64__)
#  define M8_64_BITS_COMPILE
#endif

namespace m8
{
   /// Type that represents a bitboard.
   typedef std::uint64_t Bb;

   /// Empty bitboard constant
   const Bb kEmptyBb = UINT64_C(0);

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
      // A : Bit is a valid bit index
      assert(bit < 64);

      bb |= (UINT64_C(1) << bit);
   }

   /// Set the specified bit to zero in a bitboard.
   ///
   /// @param bb  The bitboard to modify.
   /// @param bit The position of the bit to set.
   inline void UnsetBit(Bb& bb, std::uint32_t bit)
   {
      // A : Bit is a valid bit index
      assert(bit < 64);

      bb &= ~(UINT64_C(1) << bit);
   }

   /// Switch the value of the specified bit to zero in a bitboard.
   ///
   /// @param bb  The bitboard to modify.
   /// @param bit The position of the bit to set.
   inline void SwitchBit(Bb& bb, std::uint32_t bit)
   {
      // A : Bit is a valid bit index
      assert(bit < 64);

      bb ^= (UINT64_C(1) << bit);
   }

   /// Get the position of the least significant bit set to 1 in a bitboard.
   ///
   /// @param bb Bitboard to read from.
   /// @return Position of the least significant bit set to 1.
   inline std::uint32_t GetLsb(Bb bb)
   {
      // A : The bitboard is not empty
      assert(bb != 0);

#if defined(M8_USE_MSC_INTRINSICS)
      // This is the intrinsics version of GetLsb used with VC++
      unsigned long index;
      _BitScanForward64(&index, bb);
      return index;
#elif defined(M8_USE_GCC_ASSEMBLY)
      // This is the g++ assembly version
      asm ("bsfq %0, %0" : "=r" (bb) : "0" (bb));
      return static_cast<unsigned int>(bb);

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

   /// Get the position of the most significant bit set to 1 in a bitboard.
   ///
   /// @param bb Bitboard to read from.
   /// @return Position of the most significant bit set to 1.
   inline std::uint32_t GetMsb(Bb bb)
   {
      // A : The bitboard is not empty
      assert(bb != 0);

#if defined(M8_USE_MSC_INTRINSICS)
      // This is the intrinsics version of GetMsb used with VC++
      unsigned long index;
      _BitScanReverse64(&index, bb);
      return index;
#elif defined(M8_USE_GCC_ASSEMBLY)
      // This is the g++ assembly version
      asm("bsrl %0, %0" : "=r" (bb) : "0" (bb));
      return static_cast<unsigned int>(bb);

#else
      // This is a 64 bit friendly of GetMsb using debruijn multiplication
      const unsigned int index64[64] =
      {
         0, 47,  1, 56, 48, 27,  2, 60,
         57, 49, 41, 37, 28, 16,  3, 61,
         54, 58, 35, 52, 50, 42, 21, 44,
         38, 32, 29, 23, 17, 11,  4, 62,
         46, 55, 26, 59, 40, 36, 15, 53,
         34, 51, 20, 43, 31, 22, 10, 45,
         25, 39, 14, 33, 19, 30,  9, 24,
         13, 18,  8, 12,  7,  6,  5, 63
      };

      bb |= bb >> 1; 
      bb |= bb >> 2;
      bb |= bb >> 4;
      bb |= bb >> 8;
      bb |= bb >> 16;
      bb |= bb >> 32;
      return index64[(bb * debruijn64) >> 58];

#endif
   }

   /// Get the least significant bit set to 1 in a bitboard and set it to 0.
   ///
   /// @param bb Bitboard to read.
   /// @returns Position of the least significant bit set to 1 in bb.
   inline std::uint32_t RemoveLsb(Bb& bb)
   {
      // A : The bitboard is not empty
      assert(bb != 0);

      std::uint32_t lsb = GetLsb(bb);
      bb &= bb - 1;
      return lsb;
   }

   /// Count the number of bits set to 1 in a bitboard.
   ///
   /// @param bb Bitboard to count the bits from.
   /// @returns The number of bits set to 1 (0..64).
   inline std::uint64_t GetPopct(Bb bb)
   {
#if defined(M8_USE_MSC_INTRINSICS)
      return __popcnt64(bb);
#else
      unsigned int w = static_cast<unsigned int>(bb >> 32),
         v = static_cast<unsigned int>(bb);
      v = v - ((v >> 1) & 0x55555555);
      w = w - ((w >> 1) & 0x55555555);
      v = (v & 0x33333333) + ((v >> 2) & 0x33333333);
      w = (w & 0x33333333) + ((w >> 2) & 0x33333333);
      v = (v + (v >> 4)) & 0x0F0F0F0F;
      w = (w + (w >> 4)) & 0x0F0F0F0F;
      v = ((v + w) * 0x01010101) >> 24;
      return v;
#endif
   }
}

#endif // M8_BB_HPP_
