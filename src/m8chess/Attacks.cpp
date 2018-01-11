/// @file   Attacks.cpp
/// @author Mathieu Pagé
/// @date   Janurary 2018
/// @brief  Contains methods and array used to generate attacks bitboards.

#include "Attacks.hpp"

namespace m8
{
    AttackArray knight_attack_bb;
    AttackArray king_attack_bb;
    std::array<Bb, 102400> rook_attack_bb;
    std::array<Bb, 5248> bishop_attack_bb;
    MagicArray rook_magic;
    MagicArray bishop_magic;

/*    const std::array<const Bb, 64> kRookMagics =
    {
        BB_C(0x0080004000802410),
        BB_C(0x004000a000401001),
        BB_C(0x0080100008200080),
        BB_C(0x0080041000808800),
        BB_C(0x2080020400800800),
        BB_C(0x0100010002040048),
        BB_C(0x0080060001000080),
        BB_C(0x0080002041000080),
        BB_C(0x0200800140008020),
        BB_C(0x0000401000402002),
        BB_C(0x0011001100402000),
        BB_C(0x0001000810010420),
        BB_C(0x0800800800140080),
        BB_C(0x0000800401800600),
        BB_C(0x0020800100220080),
        BB_C(0x0100800280004100),
        BB_C(0x0100208000804000),
        BB_C(0x4001010040008020),
        BB_C(0x0090002000280400),
        BB_C(0x8010008008001080),
        BB_C(0x1000808008010400),
        BB_C(0x0000808002000401),
        BB_C(0x0004810100020004),
        BB_C(0x0001004000800040),
        BB_C(0x0000420200210080),
        BB_C(0x008020004000d000),
        BB_C(0x0000120200402080),
        BB_C(0x1800080080100080),
        BB_C(0x4000080080800400),
        BB_C(0x4002000200100804),
        BB_C(0x0040080400011002),
        BB_C(0x0000808200004401),
        BB_C(0x0001008003002040),
        BB_C(0x0000201000c00240),
        BB_C(0x4000100880802000),
        BB_C(0x0000800800801005),
        BB_C(0x0041080080800400),
        BB_C(0x0000040080801200),
        BB_C(0x0000020104000810),
        BB_C(0x0000800062800100),
        BB_C(0x0040008020408000),
        BB_C(0x0000600050004000),
        BB_C(0x2000100020008080),
        BB_C(0x0010880010008080),
        BB_C(0x0008008400808008),
        BB_C(0x0000840002008080),
        BB_C(0x0000040200010100),
        BB_C(0x0000110040820004),
        BB_C(0x0040400820800080),
        BB_C(0x0080400080200080),
        BB_C(0x0020801000200180),
        BB_C(0x0000100080080080),
        BB_C(0x1080040008008080),
        BB_C(0x1000240080020080),
        BB_C(0x0010800100020080),
        BB_C(0x8000800100084080),
        BB_C(0x0001002080004011),
        BB_C(0x0000400100801021),
        BB_C(0x0000084100200011),
        BB_C(0x0000201000040901),
        BB_C(0x0002001020080402),
        BB_C(0x0001000400020801),
        BB_C(0x0000010200900804),
        BB_C(0x0000040040210082)
    };

    const std::array<const std::uint32_t, 64> kRookMagicShifts =
    {
        52, 53, 53, 53, 53, 53, 53, 52,
        53, 54, 54, 54, 54, 54, 54, 53,
        53, 54, 54, 54, 54, 54, 54, 53,
        53, 54, 54, 54, 54, 54, 54, 53,
        53, 54, 54, 54, 54, 54, 54, 53,
        53, 54, 54, 54, 54, 54, 54, 53,
        53, 54, 54, 54, 54, 54, 54, 53,
        52, 53, 53, 53, 53, 53, 53, 52
    };

    const std::array<const Bb, 64> kBishopMagics =
    {
        BB_C(0x0004100200410000),
        BB_C(0x0020010101000000),
        BB_C(0x0004041400400080),
        BB_C(0x0144042080000000),
        BB_C(0x00040c2000000820),
        BB_C(0x4000900420000800),
        BB_C(0x0000440405000000),
        BB_C(0x000010840c200000),
        BB_C(0x0001200810010000),
        BB_C(0x0020040108020008),
        BB_C(0x0000100420404000),
        BB_C(0x0000840400800010),
        BB_C(0x0080840420000002),
        BB_C(0x0000020804040000),
        BB_C(0x0000040205060000),
        BB_C(0x0000010046100002),
        BB_C(0x0020080404040800),
        BB_C(0x000400a008008100),
        BB_C(0x0202000404040008),
        BB_C(0x0048000182004000),
        BB_C(0x0011000090400100),
        BB_C(0x0000400208200408),
        BB_C(0x0801000080901000),
        BB_C(0x0000200202840400),
        BB_C(0x0020040010042800),
        BB_C(0x0001040008480800),
        BB_C(0x0004010010010020),
        BB_C(0x0002020808008008),
        BB_C(0x0000840014802000),
        BB_C(0x0010010100208800),
        BB_C(0x0001020004020900),
        BB_C(0x0002002802008200),
        BB_C(0x0011022000080800),
        BB_C(0x0004020240200400),
        BB_C(0x0000108880100400),
        BB_C(0x0000200800028820),
        BB_C(0x0004010010040240),
        BB_C(0x0000908200010100),
        BB_C(0x0001020080020800),
        BB_C(0x4000820200008080),
        BB_C(0x0001012010102000),
        BB_C(0x0022008208002000),
        BB_C(0x0000084048001020),
        BB_C(0x0000044204800800),
        BB_C(0x0040400092000100),
        BB_C(0x0040080081000020),
        BB_C(0x0004080a04000040),
        BB_C(0x0001010202000080),
        BB_C(0x8000020110090000),
        BB_C(0x8000010101100100),
        BB_C(0x0000042205100000),
        BB_C(0x0480000042020000),
        BB_C(0x0000441002020400),
        BB_C(0x0040400204010080),
        BB_C(0x6000040404004000),
        BB_C(0x2000040800812000),
        BB_C(0x0000008044025000),
        BB_C(0x00004004040a0200),
        BB_C(0x0000020220841000),
        BB_C(0x0008000008208820),
        BB_C(0x0800400010020220),
        BB_C(0x0000002104050200),
        BB_C(0x0080800401040400),
        BB_C(0x0100100101010200)
    };

    const std::array<const std::uint32_t, 64> kBishopMagicShifts =
    {
        58, 59, 59, 59, 59, 59, 59, 58,
        59, 59, 59, 59, 59, 59, 59, 59,
        59, 59, 57, 57, 57, 57, 59, 59,
        59, 59, 57, 55, 55, 57, 59, 59,
        59, 59, 57, 55, 55, 57, 59, 59,
        59, 59, 57, 57, 57, 57, 59, 59,
        59, 59, 59, 59, 59, 59, 59, 59,
        58, 59, 59, 59, 59, 59, 59, 58
    };*/

    const std::array<const Bb, 64> kRookMagics =
    {
        BB_C(0x0080008220400010),
        BB_C(0x1140002000100040),
        BB_C(0x0080200010010880),
        BB_C(0x0480080010000480),
        BB_C(0x0080220800040080),
        BB_C(0x4100010004000208),
        BB_C(0x0280020001000080),
        BB_C(0x0880008000204100),
        BB_C(0x0000802140008008),
        BB_C(0x0100401000402000),
        BB_C(0x0001001040600100),
        BB_C(0x8010800800801000),
        BB_C(0x10008008000c0080),
        BB_C(0x0801000804010002),
        BB_C(0x0001000100020084),
        BB_C(0x0001800180004100),
        BB_C(0x008000c02000c000),
        BB_C(0x0060004000500020),
        BB_C(0x0000430011002000),
        BB_C(0x0002808008001000),
        BB_C(0x0004808008010400),
        BB_C(0x0084008004020080),
        BB_C(0x0000240001081002),
        BB_C(0x0000020000448401),
        BB_C(0x0080400080008022),
        BB_C(0x0000400080200088),
        BB_C(0x0220200080801000),
        BB_C(0x0001002100100028),
        BB_C(0x0020080080040081),
        BB_C(0x0100120080040080),
        BB_C(0x0021004100020004),
        BB_C(0x0000010200009044),
        BB_C(0x0080004000402001),
        BB_C(0x0021201000404000),
        BB_C(0x0100200080801001),
        BB_C(0x0000820800801000),
        BB_C(0x0000100501000800),
        BB_C(0x0400020080800400),
        BB_C(0x0820800100800200),
        BB_C(0x00008000c0800100),
        BB_C(0x0080004020014010),
        BB_C(0x001000c020004004),
        BB_C(0x0010008020008010),
        BB_C(0x0000480010008080),
        BB_C(0x0004001800808004),
        BB_C(0x0004060004008080),
        BB_C(0x0002000c08020001),
        BB_C(0x0400040080420001),
        BB_C(0x0000224000801080),
        BB_C(0x1001004000802100),
        BB_C(0x0020002081100080),
        BB_C(0x0000821800100080),
        BB_C(0x0000210080401002),
        BB_C(0x0008020080040080),
        BB_C(0x0100800200210080),
        BB_C(0x0000012080440200),
        BB_C(0x0000410010208001),
        BB_C(0x0000802040010011),
        BB_C(0x0000200100084011),
        BB_C(0x0000200900041001),
        BB_C(0x0001000800100403),
        BB_C(0x0001000802040041),
        BB_C(0x0000011008020084),
        BB_C(0x0000040040208102)
    };

    const std::array<const std::uint32_t, 64> kRookMagicShifts =
    {
        52, 53, 53, 53, 53, 53, 53, 52,
        53, 54, 54, 54, 54, 54, 54, 53,
        53, 54, 54, 54, 54, 54, 54, 53,
        53, 54, 54, 54, 54, 54, 54, 53,
        53, 54, 54, 54, 54, 54, 54, 53,
        53, 54, 54, 54, 54, 54, 54, 53,
        53, 54, 54, 54, 54, 54, 54, 53,
        52, 53, 53, 53, 53, 53, 53, 52
    };

    const std::array<const Bb, 64> kBishopMagics =
    {
        BB_C(0x0002200404008020),
        BB_C(0x2020080080808000),
        BB_C(0x0004040082000800),
        BB_C(0x0004040080000000),
        BB_C(0x0022021000200040),
        BB_C(0x0011100804400000),
        BB_C(0x0004020202200200),
        BB_C(0x0000140104100400),
        BB_C(0x0000200204010408),
        BB_C(0x0000021001011100),
        BB_C(0x1000220204002000),
        BB_C(0x0808040408800000),
        BB_C(0x0800041044010000),
        BB_C(0x0000020803080100),
        BB_C(0x0000020202024000),
        BB_C(0x0000002084042000),
        BB_C(0x0010000820018400),
        BB_C(0x0084000204040400),
        BB_C(0x0040400800802080),
        BB_C(0x0002002020801000),
        BB_C(0x0001040820080000),
        BB_C(0x0040800410008800),
        BB_C(0x0004000104020200),
        BB_C(0x0002000482008200),
        BB_C(0x0018400008100100),
        BB_C(0x0004100020010900),
        BB_C(0x0004010030004080),
        BB_C(0x0808080000220020),
        BB_C(0x0001001001004000),
        BB_C(0x2002008004100080),
        BB_C(0x0001020000480404),
        BB_C(0x0080808002004400),
        BB_C(0x0002100404400800),
        BB_C(0x0001011000181000),
        BB_C(0x0000140201040800),
        BB_C(0x0800020080880080),
        BB_C(0x0008060400001100),
        BB_C(0x0010100040012400),
        BB_C(0x0004082040020100),
        BB_C(0x0000840100008080),
        BB_C(0x0082021040000400),
        BB_C(0x0000410420081000),
        BB_C(0x0000420050000300),
        BB_C(0x0000004208000880),
        BB_C(0x0800880104000040),
        BB_C(0x0110101000400020),
        BB_C(0x0004080200400408),
        BB_C(0x0002042400800020),
        BB_C(0x0000840c02400000),
        BB_C(0x0004220804040000),
        BB_C(0x8000008068080000),
        BB_C(0x1000000142021000),
        BB_C(0x0000009002020000),
        BB_C(0x0000040408420001),
        BB_C(0x0020040400840040),
        BB_C(0x0010110801004000),
        BB_C(0x0001002202024000),
        BB_C(0x0000008201012000),
        BB_C(0x0000000202840401),
        BB_C(0x2200000000218800),
        BB_C(0x0100084010020200),
        BB_C(0x0000402002021200),
        BB_C(0x0000401001020090),
        BB_C(0x0010200200820010)
    };

    const std::array<const std::uint32_t, 64> kBishopMagicShifts =
    {
        58, 59, 59, 59, 59, 59, 59, 58,
        59, 59, 59, 59, 59, 59, 59, 59,
        59, 59, 57, 57, 57, 57, 59, 59,
        59, 59, 57, 55, 55, 57, 59, 59,
        59, 59, 57, 55, 55, 57, 59, 59,
        59, 59, 57, 57, 57, 57, 59, 59,
        59, 59, 59, 59, 59, 59, 59, 59,
        58, 59, 59, 59, 59, 59, 59, 58
    };

    void InitializeKnightAttackBb()
    {
        for (Sq sq = kA1; IsSqOnBoard(sq); ++sq)
        {
            Bb mask = BB_C(0);
            Colmn col = GetColmn(sq);
            Row row = GetRow(sq);

            if (col <= kColmnG && row <= kRow6)
                SetBit(mask, sq + 17);

            if (col <= kColmnF && row <= kRow7)
                SetBit(mask, sq + 10);

            if (col <= kColmnF && row >= kRow2)
                SetBit(mask, sq - 6);

            if (col <= kColmnG && row >= kRow3)
                SetBit(mask, sq - 15);

            if (col >= kColmnB && row >= kRow3)
                SetBit(mask, sq - 17);

            if (col >= kColmnC && row >= kRow2)
                SetBit(mask, sq - 10);

            if (col >= kColmnC && row <= kRow7)
                SetBit(mask, sq + 6);

            if (col >= kColmnB && row <= kRow6)
                SetBit(mask, sq + 15);

            knight_attack_bb[sq] = mask;
        }
    }

    void InitializeKingAttackBb()
    {
        for (Sq sq = kA1; IsSqOnBoard(sq); ++sq)
        {
            Bb mask = BB_C(0);
            Colmn col = GetColmn(sq);
            Row row = GetRow(sq);

            if (row < kRow8)
                SetBit(mask, sq + 8);

            if (col < kColmnH && row < kRow8)
                SetBit(mask, sq + 9);

            if (col < kColmnH)
                SetBit(mask, sq + 1);

            if (col < kColmnH && row > kRow1)
                SetBit(mask, sq - 7);

            if (row > kRow1)
                SetBit(mask, sq - 8);

            if (col > kColmnA && row > kRow1)
                SetBit(mask, sq - 9);

            if (col > kColmnA)
                SetBit(mask, sq - 1);

            if (col > kColmnA && row < kRow8)
                SetBit(mask, sq + 7);

            king_attack_bb[sq] = mask;
        }
    }

    Bb GenerateRookAttackForOccupancy(Sq from, Bb occupation)
    {
        Bb bb_attack = kEmptyBb;
        Sq sq;

        /* go north */
        sq = from;
        while (GetRow(sq) < kRow8 && !GetBit(occupation, sq))
        {
            SetBit(bb_attack, sq + 8);
            sq += 8;
        }

        /* go south */
        sq = from;
        while (GetRow(sq) > kRow1 && !GetBit(occupation, sq))
        {
            SetBit(bb_attack, sq - 8);
            sq -= 8;
        }

        /* go west */
        sq = from;
        while (GetColmn(sq) > kColmnA && !GetBit(occupation, sq))
        {
            SetBit(bb_attack, sq - 1);
            sq -= 1;
        }

        /* go est */
        sq = from;
        while (GetColmn(sq) < kColmnH && !GetBit(occupation, sq))
        {
            SetBit(bb_attack, sq + 1);
            sq += 1;
        }

        return bb_attack;
    }

    /// Generate the rook attacks precalculation for a given square.
    ///
    /// @param sq Position of the rook.
    /// @param magic Magic structure for which the methods generate the attack tables.
    ///              This parameter is a reference and will be modified by the method.
    void InitializeRookAttack(Sq sq, Magic& magic)
    {
        /* For each variation of occupation of the mask */
        std::uint64_t nbr_bits = GetPopct(magic.mask);
        for (Bb occ_index = kEmptyBb; occ_index < (BB_C(1) << nbr_bits); ++occ_index)
        {
            Bb mask = DistributeBits(occ_index, magic.mask);
            std::ptrdiff_t index = (std::ptrdiff_t)((mask * magic.magic) >> magic.shift);
            magic.attack[index] = GenerateRookAttackForOccupancy(sq, mask);
        }
    }

    void InitializeRookMagics()
    {
        Bb* ptr_attack = rook_attack_bb.data();

        for (Sq sq = kA1; IsSqOnBoard(sq); ++sq)
        {
            Row row = GetRow(sq);
            Colmn col = GetColmn(sq);
            Magic& magic = rook_magic[sq];

            magic.attack = ptr_attack;
            magic.mask = ((kBbRow[row] & ~(kBbColmn[kColmnA] | kBbColmn[kColmnH])) |
                (kBbColmn[col] & ~(kBbRow[kRow1] | kBbRow[kRow8]))) &
                ~GetSingleBitBb(sq);
            magic.magic = kRookMagics[sq];
            magic.shift = kRookMagicShifts[sq];

            InitializeRookAttack(sq, magic);

            ptr_attack += BB_C(1) << (64 - magic.shift);
        }
    }

    Bb GenerateBishopAttackForOccupancy(Sq from, Bb occupation)
    {
        Bb bb_attack = kEmptyBb;
        Sq sq;

        /* go northest */
        sq = from;
        while (GetRow(sq) < kRow8 &&
            GetColmn(sq) < kColmnH &&
            !GetBit(occupation, sq))
        {
            SetBit(bb_attack, sq + 9);
            sq += 9;
        }

        /* go southest */
        sq = from;
        while (GetRow(sq) > kRow1 &&
            GetColmn(sq) < kColmnH &&
            !GetBit(occupation, sq))
        {
            SetBit(bb_attack, sq - 7);
            sq -= 7;
        }

        /* go southwest */
        sq = from;
        while (GetRow(sq) > kRow1 &&
            GetColmn(sq) > kColmnA &&
            !GetBit(occupation, sq))
        {
            SetBit(bb_attack, sq - 9);
            sq -= 9;
        }

        /* go northwest */
        sq = from;
        while (GetRow(sq) < kRow8 &&
            GetColmn(sq) > kColmnA &&
            !GetBit(occupation, sq))
        {
            SetBit(bb_attack, sq + 7);
            sq += 7;
        }

        return bb_attack;
    }

    /// Generate the bishop attacks precalculation for a given square.
    ///
    /// @param sq Position of the rook.
    /// @param magic Magic structure for which the methods generate the attack tables.
    ///              This parameter is a reference and will be modified by the method.
    void InitializeBishopAttack(Sq sq, Magic& magic)
    {
        /* For each variation of occupation of the mask */
        uint64_t nbr_bits = GetPopct(magic.mask);
        for (Bb occ_index = kEmptyBb; occ_index < (BB_C(1) << nbr_bits); ++occ_index)
        {
            Bb mask = DistributeBits(occ_index, magic.mask);
            std::ptrdiff_t index = (std::ptrdiff_t)((mask * magic.magic) >> magic.shift);
            magic.attack[index] = GenerateBishopAttackForOccupancy(sq, mask);
        }
    }

    void InitializeBishopMagics()
    {
        Bb border = kBbRow[kRow8] |
            kBbColmn[kColmnA] | kBbColmn[kColmnH] |
            kBbRow[kRow1];

        Bb* ptr_attack = bishop_attack_bb.data();

        for (Sq sq = kA1; IsSqOnBoard(sq); ++sq)
        {
            Diagonal diag = GetDiag(sq);
            Diagonal anti_diag = GetAntiDiag(sq);
            Magic& magic = bishop_magic[sq];

            magic.attack = ptr_attack;
            magic.mask = (kBbDiag[diag] ^ kBbAntiDiag[anti_diag]) & ~border;
            magic.magic = kBishopMagics[sq];
            magic.shift = kBishopMagicShifts[sq];

            InitializeBishopAttack(sq, magic);

            ptr_attack += BB_C(1) << (64 - magic.shift);
        }
    }

    void InitializeAttacks()
    {
        InitializeKnightAttackBb();
        InitializeKingAttackBb();
        InitializeRookMagics();
        InitializeBishopMagics();
    }
}