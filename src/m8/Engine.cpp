/// @file	Engine.cpp
/// @author Mathieu Pagé
/// @date	November 2017
/// @brief	Contains the Engine class that encapsulate all m8 functionalities.

#include "Engine.hpp"

namespace m8
{
    Engine::Engine()
        //: board_(kStartingPositionFEN)
        : board_("rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c4 0 1")
    {}
}