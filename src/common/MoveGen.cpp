/// @file   MoveGen.cpp
/// @author Mathieu Pagé
/// @date   December 2015
/// @brief  Contains the definition of types and utilities to generate moves

// TODO : Remove
#include <iostream>

#include "MoveGen.hpp"

namespace m8
{
    MoveGen::AttackArray MoveGen::knight_attack_bb_;
    MoveGen::AttackArray MoveGen::king_attack_bb_;

    void MoveGen::InitializeKnightAttackBb()
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

            knight_attack_bb_[sq] = mask;
        }
    }

    void MoveGen::InitializeKingAttackBb()
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
                SetBit(mask, sq -7);

            if (row > kRow1)
                SetBit(mask, sq - 8);

            if (col > kColmnA && row > kRow1)
                SetBit(mask, sq - 9);

            if (col > kColmnA)
                SetBit(mask, sq - 1);

            if (col > kColmnA && row < kRow8)
                SetBit(mask, sq + 7);

            king_attack_bb_[sq] = mask;
        }
    }

    void MoveGen::InitializePreCalc()
    {
        InitializeKnightAttackBb();
        InitializeKingAttackBb();
    }
}