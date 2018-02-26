/// @file	Engine.cpp
/// @author Mathieu Pagé
/// @date	November 2017
/// @brief	Contains the Engine class that encapsulate all m8 functionalities.

#include "Engine.hpp"

#include "../m8chess/SAN.hpp"

namespace m8
{
    Engine::Engine()
        : board_(kStartingPositionFEN)
    {
        board_.set_psqt(eval::GeneratePieceSqTable());
    }

    PerftResult Engine::Perft(int depth, std::function<void(std::string, std::uint64_t)> callback)
    {
        m8::Perft perft(depth, board_, [callback, this
        ](Move move, std::uint64_t count) { callback(RenderSAN(move, board_), count); });
        return perft.RunParallel();
    }
}