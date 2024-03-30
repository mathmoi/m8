/// @file   Init.hpp
/// @author Mathieu Pagé
/// @date   December 2015
/// @brief  Contains initialization functionalities.

#include <iostream>

#include "m8common/Bb.hpp"

#include "eval/Eval.hpp"
#include "eval/PieceSqTable.hpp"

#include "movegen/Attacks.hpp"
#include "movegen/MvvLva.hpp"

#include "transposition/Zobrist.hpp"

#include "Direction.hpp"

namespace m8
{
    /// Initialize all the precalculation for all class of m8.
    inline void InitializePreCalc()
    {
        eval::InitializeEval();
        eval::InitializePieceSqTable();
        movegen::InitializeAttacks();
        movegen::InitializeMvvLva();
        BbInitializePreCalc();
        InitializeDirection();
        transposition::InitializeZobrist();
    }
}