/// @file   MoveGen.cpp
/// @author Mathieu Pag�
/// @date   December 2015
/// @brief  Contains the definition of types and utilities to generate moves

#include "MoveGen.hpp"

namespace m8
{
    MoveGen::AttackArray MoveGen::knight_attack_bb_;
    MoveGen::AttackArray MoveGen::king_attack_bb_;
    std::array<Bb, 102400> MoveGen::rook_attack_bb_;
    std::array<Bb, 5248> MoveGen::bishop_attack_bb_;
    MoveGen::MagicArray MoveGen::rook_magic_;
    MoveGen::MagicArray MoveGen::bishop_magic_;

    const std::array<const Bb, 64> MoveGen::kRookMagics =
    {
        UINT64_C(0x0080004000802410),
        UINT64_C(0x004000a000401001),
        UINT64_C(0x0080100008200080),
        UINT64_C(0x0080041000808800),
        UINT64_C(0x2080020400800800),
        UINT64_C(0x0100010002040048),
        UINT64_C(0x0080060001000080),
        UINT64_C(0x0080002041000080),
        UINT64_C(0x0200800140008020),
        UINT64_C(0x0000401000402002),
        UINT64_C(0x0011001100402000),
        UINT64_C(0x0001000810010420),
        UINT64_C(0x0800800800140080),
        UINT64_C(0x0000800401800600),
        UINT64_C(0x0020800100220080),
        UINT64_C(0x0100800280004100),
        UINT64_C(0x0100208000804000),
        UINT64_C(0x4001010040008020),
        UINT64_C(0x0090002000280400),
        UINT64_C(0x8010008008001080),
        UINT64_C(0x1000808008010400),
        UINT64_C(0x0000808002000401),
        UINT64_C(0x0004810100020004),
        UINT64_C(0x0001004000800040),
        UINT64_C(0x0000420200210080),
        UINT64_C(0x008020004000d000),
        UINT64_C(0x0000120200402080),
        UINT64_C(0x1800080080100080),
        UINT64_C(0x4000080080800400),
        UINT64_C(0x4002000200100804),
        UINT64_C(0x0040080400011002),
        UINT64_C(0x0000808200004401),
        UINT64_C(0x0001008003002040),
        UINT64_C(0x0000201000c00240),
        UINT64_C(0x4000100880802000),
        UINT64_C(0x0000800800801005),
        UINT64_C(0x0041080080800400),
        UINT64_C(0x0000040080801200),
        UINT64_C(0x0000020104000810),
        UINT64_C(0x0000800062800100),
        UINT64_C(0x0040008020408000),
        UINT64_C(0x0000600050004000),
        UINT64_C(0x2000100020008080),
        UINT64_C(0x0010880010008080),
        UINT64_C(0x0008008400808008),
        UINT64_C(0x0000840002008080),
        UINT64_C(0x0000040200010100),
        UINT64_C(0x0000110040820004),
        UINT64_C(0x0040400820800080),
        UINT64_C(0x0080400080200080),
        UINT64_C(0x0020801000200180),
        UINT64_C(0x0000100080080080),
        UINT64_C(0x1080040008008080),
        UINT64_C(0x1000240080020080),
        UINT64_C(0x0010800100020080),
        UINT64_C(0x8000800100084080),
        UINT64_C(0x0001002080004011),
        UINT64_C(0x0000400100801021),
        UINT64_C(0x0000084100200011),
        UINT64_C(0x0000201000040901),
        UINT64_C(0x0002001020080402),
        UINT64_C(0x0001000400020801),
        UINT64_C(0x0000010200900804),
        UINT64_C(0x0000040040210082)
    };

    const std::array<const std::uint32_t, 64> MoveGen::kRookMagicShifts =
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

    const std::array<const Bb, 64> MoveGen::kBishopMagics =
    {
        UINT64_C(0x0004100200410000),
        UINT64_C(0x0020010101000000),
        UINT64_C(0x0004041400400080),
        UINT64_C(0x0144042080000000),
        UINT64_C(0x00040c2000000820),
        UINT64_C(0x4000900420000800),
        UINT64_C(0x0000440405000000),
        UINT64_C(0x000010840c200000),
        UINT64_C(0x0001200810010000),
        UINT64_C(0x0020040108020008),
        UINT64_C(0x0000100420404000),
        UINT64_C(0x0000840400800010),
        UINT64_C(0x0080840420000002),
        UINT64_C(0x0000020804040000),
        UINT64_C(0x0000040205060000),
        UINT64_C(0x0000010046100002),
        UINT64_C(0x0020080404040800),
        UINT64_C(0x000400a008008100),
        UINT64_C(0x0202000404040008),
        UINT64_C(0x0048000182004000),
        UINT64_C(0x0011000090400100),
        UINT64_C(0x0000400208200408),
        UINT64_C(0x0801000080901000),
        UINT64_C(0x0000200202840400),
        UINT64_C(0x0020040010042800),
        UINT64_C(0x0001040008480800),
        UINT64_C(0x0004010010010020),
        UINT64_C(0x0002020808008008),
        UINT64_C(0x0000840014802000),
        UINT64_C(0x0010010100208800),
        UINT64_C(0x0001020004020900),
        UINT64_C(0x0002002802008200),
        UINT64_C(0x0011022000080800),
        UINT64_C(0x0004020240200400),
        UINT64_C(0x0000108880100400),
        UINT64_C(0x0000200800028820),
        UINT64_C(0x0004010010040240),
        UINT64_C(0x0000908200010100),
        UINT64_C(0x0001020080020800),
        UINT64_C(0x4000820200008080),
        UINT64_C(0x0001012010102000),
        UINT64_C(0x0022008208002000),
        UINT64_C(0x0000084048001020),
        UINT64_C(0x0000044204800800),
        UINT64_C(0x0040400092000100),
        UINT64_C(0x0040080081000020),
        UINT64_C(0x0004080a04000040),
        UINT64_C(0x0001010202000080),
        UINT64_C(0x8000020110090000),
        UINT64_C(0x8000010101100100),
        UINT64_C(0x0000042205100000),
        UINT64_C(0x0480000042020000),
        UINT64_C(0x0000441002020400),
        UINT64_C(0x0040400204010080),
        UINT64_C(0x6000040404004000),
        UINT64_C(0x2000040800812000),
        UINT64_C(0x0000008044025000),
        UINT64_C(0x00004004040a0200),
        UINT64_C(0x0000020220841000),
        UINT64_C(0x0008000008208820),
        UINT64_C(0x0800400010020220),
        UINT64_C(0x0000002104050200),
        UINT64_C(0x0080800401040400),
        UINT64_C(0x0100100101010200)
    };

    const std::array<const std::uint32_t, 64> MoveGen::kBishopMagicShifts =
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

    void MoveGen::InitializeKnightAttackBb()
    {
        for (Sq sq = kA1; IsSqOnBoard(sq); ++sq)
        {
            Bb mask = UINT64_C(0);
            Colmn col = GetColmn(sq);
            Row row = GetRow(sq);

            if (col <= kColmnG && row <= kRow6)
                mask.Set(sq + 17);

            if (col <= kColmnF && row <= kRow7)
                mask.Set(sq + 10);

            if (col <= kColmnF && row >= kRow2)
                mask.Set(sq - 6);

            if (col <= kColmnG && row >= kRow3)
                mask.Set(sq - 15);

            if (col >= kColmnB && row >= kRow3)
                mask.Set(sq - 17);

            if (col >= kColmnC && row >= kRow2)
                mask.Set(sq - 10);

            if (col >= kColmnC && row <= kRow7)
                mask.Set(sq + 6);

            if (col >= kColmnB && row <= kRow6)
                mask.Set(sq + 15);

            knight_attack_bb_[sq] = mask;
        }
    }

    void MoveGen::InitializeKingAttackBb()
    {
        for (Sq sq = kA1; IsSqOnBoard(sq); ++sq)
        {
            Bb mask = UINT64_C(0);
            Colmn col = GetColmn(sq);
            Row row = GetRow(sq);

            if (row < kRow8)
                mask.Set(sq + 8);

            if (col < kColmnH && row < kRow8)
                mask.Set(sq + 9);

            if (col < kColmnH)
                mask.Set(sq + 1);

            if (col < kColmnH && row > kRow1)
                mask.Set(sq -7);

            if (row > kRow1)
                mask.Set(sq - 8);

            if (col > kColmnA && row > kRow1)
                mask.Set(sq - 9);

            if (col > kColmnA)
                mask.Set(sq - 1);

            if (col > kColmnA && row < kRow8)
                mask.Set(sq + 7);

            king_attack_bb_[sq] = mask;
        }
    }

    Bb MoveGen::GenerateRookAttackForOccupancy(Sq from, Bb occupation)
    {
        Bb bb_attack = Bb::Empty();
        Sq sq;

        /* go north */
        sq = from;
        while (GetRow(sq) < kRow8 && !occupation[sq])
        {
            bb_attack.Set(sq + 8);
            sq += 8;
        }

        /* go south */
        sq = from;
        while (GetRow(sq) > kRow1 && !occupation[sq])
        {
            bb_attack.Set(sq - 8);
            sq -= 8;
        }

        /* go west */
        sq = from;
        while (GetColmn(sq) > kColmnA && !occupation[sq])
        {
            bb_attack.Set(sq - 1);
            sq -= 1;
        }

        /* go est */
        sq = from;
        while (GetColmn(sq) < kColmnH && !occupation[sq])
        {
            bb_attack.Set(sq + 1);
            sq += 1;
        }

        return bb_attack;
    }

    void MoveGen::InitializeRookAttack(Sq sq, MoveGen::Magic& magic)
    {
        /* For each variation of occupation of the mask */
        std::uint64_t nbr_bits = magic.mask.GetPopct();
        for (Bb occ_index = Bb::Empty(); occ_index < (UINT64_C(1) << nbr_bits); ++occ_index)
        {
            Bb mask = Bb::DistributeBits(occ_index, magic.mask);
            std::ptrdiff_t index = (std::ptrdiff_t)((mask * magic.magic) >> magic.shift);
            magic.attack[index] = GenerateRookAttackForOccupancy(sq, mask);
        }
    }

    void MoveGen::InitializeRookMagics()
    {
        Bb* ptr_attack = rook_attack_bb_.data();

        for (Sq sq = kA1; IsSqOnBoard(sq); ++sq)
        {
            Row row = GetRow(sq);
            Colmn col = GetColmn(sq);
            Magic& magic = rook_magic_[sq];

            magic.attack = ptr_attack;
            magic.mask = ((kBbRow[row] & ~(kBbColmn[kColmnA] | kBbColmn[kColmnH])) |
                          (kBbColmn[col] & ~(kBbRow[kRow1] | kBbRow[kRow8]))) &
                          ~ Bb::GetSingleBitBb(sq);
            magic.magic = kRookMagics[sq];
            magic.shift = kRookMagicShifts[sq];

            InitializeRookAttack(sq, magic);

            ptr_attack += UINT64_C(1) << (64 - magic.shift);
        }
    }

    Bb MoveGen::GenerateBishopAttackForOccupancy(Sq from, Bb occupation)
    {
        Bb bb_attack = Bb::Empty();
        Sq sq;

        /* go northest */
        sq = from;
        while (GetRow(sq) < kRow8 &&
               GetColmn(sq) < kColmnH &&
               !occupation[sq])
        {
            bb_attack.Set(sq + 9);
            sq += 9;
        }

        /* go southest */
        sq = from;
        while (GetRow(sq) > kRow1 &&
               GetColmn(sq) < kColmnH &&
               !occupation[sq])
        {
            bb_attack.Set(sq - 7);
            sq -= 7;
        }

        /* go southwest */
        sq = from;
        while (GetRow(sq) > kRow1 &&
               GetColmn(sq) > kColmnA &&
               !occupation[sq])
        {
            bb_attack.Set(sq - 9);
            sq -= 9;
        }

        /* go northwest */
        sq = from;
        while (GetRow(sq) < kRow8 &&
               GetColmn(sq) > kColmnA &&
               !occupation[sq])
        {
            bb_attack.Set(sq + 7);
            sq += 7;
        }

        return bb_attack;
    }

    void MoveGen::InitializeBishopAttack(Sq sq, MoveGen::Magic& magic)
    {
        /* For each variation of occupation of the mask */
        uint64_t nbr_bits = magic.mask.GetPopct();
        for (Bb occ_index = Bb::Empty(); occ_index < (UINT64_C(1) << nbr_bits); ++occ_index)
        {
            Bb mask = Bb::DistributeBits(occ_index, magic.mask);
            std::ptrdiff_t index = (std::ptrdiff_t)((mask * magic.magic) >> magic.shift);
            magic.attack[index] = GenerateBishopAttackForOccupancy(sq, mask);
        }
    }

    void MoveGen::InitializeBishopMagics()
    {
        Bb border =              kBbRow[kRow8] |
                    kBbColmn[kColmnA] | kBbColmn[kColmnH] |
                                 kBbRow[kRow1];

        Bb* ptr_attack = bishop_attack_bb_.data();

        for (Sq sq = kA1; IsSqOnBoard(sq); ++sq)
        {
            Diagonal diag = GetDiag(sq);
            Diagonal anti_diag = GetAntiDiag(sq);
            Magic& magic = bishop_magic_[sq];

            magic.attack = ptr_attack;
            magic.mask = (kBbDiag[diag] ^ kBbAntiDiag[anti_diag]) & ~border;
            magic.magic =  kBishopMagics[sq];
            magic.shift = kBishopMagicShifts[sq];

            InitializeBishopAttack(sq, magic);

            ptr_attack += UINT64_C(1) << (64 - magic.shift);
        }
    }

    void MoveGen::InitializePreCalc()
    {
        InitializeKnightAttackBb();
        InitializeKingAttackBb();
        InitializeRookMagics();
        InitializeBishopMagics();
    }
}