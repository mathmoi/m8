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
#include <iostream>
#include <array>

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
    /// <summary>
    ///  Represent a bitboard
    /// </summary>
    class Bb
    {
    public:
        /// <summary>
        ///  Default constructor.
        /// </summary>
        inline Bb()
        {};

        /// <summary>
        ///  Constructor. Allows implicit conversion from uint64_t.
        /// </summary>
        /// <param name="value"></param>
        inline Bb(std::uint64_t value) { value_ = value; };

        /// <summary>
        ///  Operator for implicit conversion to uint64_t
        /// </summary>
        inline operator std::uint64_t() const { return value_; };

        /// <summary>
        ///  Index operator allows to get a single bit value.
        /// </summary>
        /// <param name="index">Index of the bit</param>
        /// <returns>True if the bit is 1, false if it is 0.</returns>
        inline bool operator[](std::size_t index) const;

        /// <summary>
        ///  Get the position of the least significatn bit set to 1.
        /// </summary>
        /// <remarks>
        ///  The bitboard have at least one bit set to one before calling this method. 
        ///  Oterwise the result are undefined.
        /// </remarks>
        /// <returns>An index between 0 and 63</returns>
        inline std::uint32_t GetLSB() const;

        /// <summary>
        ///  Get the position of the most significatn bit set to 1.
        /// </summary>
        /// <remarks>
        ///  The bitboard have at least one bit set to one before calling this method. 
        ///  Oterwise the result are undefined.
        /// </remarks>
        /// <returns>An index between 0 and 63</returns>
        inline std::uint32_t GetMSB() const;

        /// <summary>
        ///  Count the number of bits set to 1 in the bitboard.
        /// </summary>
        /// <returns></returns>
        inline std::uint64_t GetPopct() const;

        /// <summary>
        ///  Set the specified bit to one in the current bitboard.
        /// </summary>
        /// <param name="index">Index of the bit to set</param>
        inline void Set(std::size_t index);

        /// <summary>
        ///  Unset the specified bit in the current bitboard.
        /// </summary>
        /// <param name="index">Index of the bit to unset</param>
        inline void Unset(std::size_t index);

        /// <summary>
        ///  Switch the specified bit in the current bitboard.
        /// </summary>
        /// <param name="index">Index of the bit to switch</param>
        inline void Switch(std::size_t index);

        /// <summary>
        ///  Get the least significant bit set to 1 in a bitboard and sit it to 0.
        /// </summary>
        /// <returns>Index of the bit removed</returns>
        inline std::uint32_t RemoveLSB();

        /// <summary>
        ///  Shift the current bitboard left or right depending on the sign of the length
        ///  parameter.
        /// </summary>
        /// <param name="length">
        ///  Length to shift. If the value is positive a left shift is applied. If the
        ///  value is negative a right shift is applied.
        /// </param>
        inline void Shift(int length);

        /// <summary>
        ///  Rotate the bitboard to the left. A rotation is like a shift, but the bits 
        ///  that overflow (or underflow) swaps arround.
        /// </summary>
        /// <param name="length">Number of bits to rotate</param>
        inline void RotateLeft(std::uint64_t length);

        /// <summary>
        ///  Rotate the bitboard to the right. A rotation is like a shift, but the bits 
        ///  that overflow (or underflow) swaps arround.
        /// </summary>
        /// <param name="length">Number of bits to rotate</param>
        inline void RotateRight(std::uint64_t length);

        /// <summary>
        ///  Create a bitboard with a single bit set to one.
        /// </summary>
        static inline Bb Bb::GetSingleBitBb(std::uint64_t bit) { return Bb(UINT64_C(1) << bit); };

        /// <summary>
        ///  Distribute the bits of bits into a new bitboard using only the positions of 
        ///  the bits set to one in mask.
        /// </summary>
        /// <param name="bits">Bits to distribute</param>
        /// <param name="mask">
        ///  Mask representing the positions to which the bits should be distributed.
        /// </param>
        /// <returns>A bitboard</returns>
        static inline Bb DistributeBits(Bb bits, Bb mask);

        inline Bb& operator ^= (Bb rhs) { value_ ^= rhs.value_; return *this; };
        inline Bb& operator |= (Bb rhs) { value_ |= rhs.value_; return *this; };
        inline Bb& operator &= (Bb rhs) { value_ &= rhs.value_; return *this; };
        inline Bb& operator *= (Bb rhs) { value_ *= rhs.value_;return *this; };
        inline Bb& operator >>= (std::int32_t rhs) { value_ >>= rhs; return *this; };
        inline Bb& operator <<= (std::int32_t rhs) { value_ <<= rhs; return *this; };
        inline Bb& operator -= (std::int32_t rhs) { value_ -= rhs; return *this; };
        inline Bb& operator ++ () { ++value_; return *this; };
        
        inline Bb operator ~ () { return ~value_; };
        
        friend std::ostream& operator <<(std::ostream& out, Bb bb);

        inline static Bb Empty() { return Bb(UINT64_C(0)); };

    private:

        /// <summary>
        ///  Value of the bitboard.
        /// </summary>
        std::uint64_t value_;
    };
    
    inline Bb operator ^ (Bb lhs, Bb rhs) { lhs ^= rhs; return lhs; }
    inline Bb operator | (Bb lhs, Bb rhs) { lhs |= rhs; return lhs; }
    inline Bb operator & (Bb lhs, Bb rhs) { lhs &= rhs; return lhs; }
    inline Bb operator * (Bb lhs, Bb rhs) { lhs *= rhs; return lhs; }
    inline Bb operator - (Bb lhs, std::int32_t rhs) { lhs -= rhs; return lhs; }
    inline Bb operator >> (Bb lhs, std::int32_t rhs) { lhs >>= rhs; return lhs; }
    inline Bb operator <<(Bb lhs, std::int32_t rhs) { lhs <<= rhs; return lhs; }

    /// <summary>
    ///  Operator allowinf to visually output a bitboard on a std::ostream.
    /// </summary>
    /// <remarks>
    ///   example output:
    ///   - - X - - - - -
    ///   X X - X - X - X
    ///    -X X X - X X X
    ///   X X X - X - X X
    ///   X X - - X - - X
    ///   - X - X X - - X
    ///   - X - X - - X -
    ///   X - - - X - - X
    /// </remarks>
    std::ostream& operator <<(std::ostream& out, Bb bb);

    inline bool Bb::operator[](std::size_t index) const
    {
        assert(0 <= index && index < 64);

        return (value_ & Bb::GetSingleBitBb(index).value_) != 0;
    }

    inline void Bb::Set(std::size_t index)
    {
        assert(0 <= index && index < 64);

        value_ |= Bb::GetSingleBitBb(index);
    }

    inline void Bb::Unset(std::size_t index)
    {
        assert(0 <= index && index < 64);

        value_ &= ~Bb::GetSingleBitBb(index);
    }

    inline void Bb::Switch(std::size_t index)
    {
        assert(0 <= index && index < 64);

        value_ ^= Bb::GetSingleBitBb(index);
    }

    inline std::uint32_t Bb::GetLSB() const
    {
        // A : The bitboard is not empty
        assert(value_ != 0);

#if defined(M8_USE_MSC_INTRINSICS)
        // This is the intrinsics version of GetLsb used with VC++
        unsigned long index;
        _BitScanForward64(&index, value_);
        return index;
#elif defined(M8_USE_GCC_ASSEMBLY)

        // This is the g++ assembly version
        std::uint64_t bb = value_;
        asm("bsfq %0, %0" : "=r" (bb) : "0" (bb));
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
        return index64[((value_ & -static_cast<std::int64_t>(value_)) * debruijn64) >> 58];
#else
        // This is the 32 bits friendly version
        std::uint64_t bb = value_;
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

    inline std::uint32_t Bb::GetMSB() const
    {
        assert(value_ != 0);

#if defined(M8_USE_MSC_INTRINSICS)
        // This is the intrinsics version of GetMsb used with VC++
        unsigned long index;
        _BitScanReverse64(&index, value_);
        return index;
#elif defined(M8_USE_GCC_ASSEMBLY)
        // This is the g++ assembly version
        asm("bsrq %0, %0" : "=r" (value_) : "0" (value_));
        return static_cast<unsigned int>(value_);

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

        std::uint64_t bb  = value_ | value_ >> 1;
        bb |= bb >> 2;
        bb |= bb >> 4;
        bb |= bb >> 8;
        bb |= bb >> 16;
        bb |= bb >> 32;
        return index64[(bb * debruijn64) >> 58];
#endif
    }

    inline std::uint64_t Bb::GetPopct() const
    {
#if defined(M8_USE_MSC_INTRINSICS)
        return __popcnt64(value_);
#else
        unsigned int w = static_cast<unsigned int>(value_ >> 32),
                     v = static_cast<unsigned int>(value_);
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

    inline std::uint32_t Bb::RemoveLSB()
    {
        // A : The bitboard is not empty
        assert(value_ != 0);

        std::uint32_t lsb = GetLSB();
        value_ &= value_ - 1;
        return lsb;
    }

    inline void Bb::Shift(int length)
    {
        if (length > 0)
            value_ <<= length;
        else
            value_ >>= -length;
    }

    inline void Bb::RotateLeft(std::uint64_t length)
    {
        value_ = (value_ << length) | (value_ >> (64 - length));
    }

    inline void Bb::RotateRight(std::uint64_t length)
    {
        RotateLeft(64 - length);
    }

    inline Bb Bb::DistributeBits(Bb bits, Bb mask)
    {
        Bb result = Bb(UINT64_C(0));
        uint64_t bits_count = mask.GetPopct();

        for (uint32_t x = 0; x < bits_count; ++x)
        {
            uint32_t pos = mask.RemoveLSB();
            if (bits[x])
            {
                result.Set(pos);
            }
        }

        return result;
    }

    /// Bitboards with bits sets for each column
    const std::array<const Bb, 8> kBbColmn = { UINT64_C(0x0101010101010101),
                                               UINT64_C(0x0202020202020202),
                                               UINT64_C(0x0404040404040404),
                                               UINT64_C(0x0808080808080808),
                                               UINT64_C(0x1010101010101010),
                                               UINT64_C(0x2020202020202020),
                                               UINT64_C(0x4040404040404040),
                                               UINT64_C(0x8080808080808080) };

    /// Bitboards with bits sets for each row
    const std::array<const Bb, 8> kBbRow = { UINT64_C(0x00000000000000FF),
                                             UINT64_C(0x000000000000FF00),
                                             UINT64_C(0x0000000000FF0000),
                                             UINT64_C(0x00000000FF000000),
                                             UINT64_C(0x000000FF00000000),
                                             UINT64_C(0x0000FF0000000000),
                                             UINT64_C(0x00FF000000000000),
                                             UINT64_C(0xFF00000000000000) };

    /// Bitboards with bits sets for each diagonal.
    ///
    /// Diagonals are in the a8-h1 orientation and the first one at index zero contains 
    /// only the a1 square. The second one contains the a2 and b1 squares.
    const std::array<const Bb, 15> kBbDiag = { UINT64_C(0x0000000000000080),
                                               UINT64_C(0x0000000000008040),
                                               UINT64_C(0x0000000000804020),
                                               UINT64_C(0x0000000080402010),
                                               UINT64_C(0x0000008040201008),
                                               UINT64_C(0x0000804020100804),
                                               UINT64_C(0x0080402010080402),
                                               UINT64_C(0x8040201008040201),
                                               UINT64_C(0x4020100804020100),
                                               UINT64_C(0x2010080402010000),
                                               UINT64_C(0x1008040201000000),
                                               UINT64_C(0x0804020100000000),
                                               UINT64_C(0x0402010000000000),
                                               UINT64_C(0x0201000000000000),
                                               UINT64_C(0x0100000000000000) };

    /// Bitboards with bits sets for each anti-diagonal.
    ///
    /// Anti-diagonals are in the a1-h8 orientation and the first one at index zero contains 
    /// only the h1 square. The second one contains the g1 and h2 squares.
    const std::array<const Bb, 15> kBbAntiDiag = { UINT64_C(0x0000000000000001),
                                                   UINT64_C(0x0000000000000102),
                                                   UINT64_C(0x0000000000010204),
                                                   UINT64_C(0x0000000001020408),
                                                   UINT64_C(0x0000000102040810),
                                                   UINT64_C(0x0000010204081020),
                                                   UINT64_C(0x0001020408102040),
                                                   UINT64_C(0x0102040810204080),
                                                   UINT64_C(0x0204081020408000),
                                                   UINT64_C(0x0408102040800000),
                                                   UINT64_C(0x0810204080000000),
                                                   UINT64_C(0x1020408000000000),
                                                   UINT64_C(0x2040800000000000),
                                                   UINT64_C(0x4080000000000000),
                                                   UINT64_C(0x8000000000000000) };

    /// Array containing bitboards with bits set for the squares between two squared 
    /// indexed by 0x88 difference.
    extern std::array<Bb, 240> kBbBetweenArray;

    /// Pointer used to access m8_g_bb_between_array with indices in the range [-119, 199].
    extern Bb* kBbBetween;

    /// Initialize the pre calculated array pertaining to the Bb type.
    void BbInitializePreCalc();



    /// Return the 0x88 index of a Mailbox square.
    ///
    /// @param sq Index of the square in a mailbox board representation.
    /// @return Index of the square in a 0x88 board representation.
    inline std::uint8_t Get0x88Index(std::uint8_t index) // TODO : Remplace
    {
        return index + (index & ~7);
    }

    /// Return the 0x88 difference between two square. This diffrence has the property of
    /// being unique for each combination of squares in regards to direction and distance.
    ///
    /// @param from First square.
    /// @param to Second square.
    /// @param The difference between the squares. Should be in the range [-119, 119];
    inline std::int8_t CalculateOx88diff(std::uint8_t from, std::uint8_t to) // TODO : Remplace
    {
        return Get0x88Index(to) - Get0x88Index(from);
    }

    /// Return a bitboard with the bits between two squares set to one.
    ///
    /// @param from Index of the origin square.
    /// @param to   Index of the destination square.
    /// @return A bitboard with the bits between from an to set to one.
    inline Bb BbBetween(std::uint8_t from, std::uint8_t to) // TODO : Remplace
    {
        Bb bb = kBbBetween[CalculateOx88diff(from, to)];
        bb.RotateLeft(from);
        return bb;
    }
}

#endif // M8_BB_HPP_
