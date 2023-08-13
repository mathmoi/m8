/// @file   Init.hpp
/// @author Mathieu Pagé
/// @date   December 2015
/// @brief  Contains initialization functionalities.

#include <iostream>

#include "../m8common/Bb.hpp"

#include "movegen/Attacks.hpp"

#include "transposition/Zobrist.hpp"

#include "Direction.hpp"

namespace m8
{
    /// Initialize all the precalculation for all class of m8.
    inline void InitializePreCalc()
    {
        movegen::InitializeAttacks();
        BbInitializePreCalc();
        InitializeDirection();
        transposition::InitializeZobrist();
    }
}