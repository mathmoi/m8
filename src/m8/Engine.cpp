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
    {}

    PerftResult Engine::Perft(int depth, std::function<void(std::string, std::uint64_t)> callback)
    {
        return m8::Perft(depth, board_, [callback, this](Move move, std::uint64_t count) { callback(RenderSAN(move, board_), count); });
    }
}