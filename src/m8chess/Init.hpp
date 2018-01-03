/// @file   Init.hpp
/// @author Mathieu Pagé
/// @date   December 2015
/// @brief  Contains initialization functionalities.

#include "../m8common/m8common.hpp"

#include "MoveGen.hpp"
#include "Direction.hpp"

#include <iostream>

namespace m8
{
    /// Initialize all the precalculation for all class of m8.
    inline void InitializePreCalc()
    {
        MoveGen::InitializePreCalc();
        BbInitializePreCalc();
        InitializeDirection();
    }
}