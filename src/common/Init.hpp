/// @file   Init.hpp
/// @author Mathieu Pag�
/// @date   December 2015
/// @brief  Contains initialization functionalities.

#include "MoveGen.hpp"
#include "Bb.hpp"

namespace m8
{
    /// Initialize all the precalculation for all class of m8.
    inline void InitializePreCalc()
    {
        MoveGen::InitializePreCalc();
        BbInitializePreCalc();
    }
}