/// @file	Engine.cpp
/// @author Mathieu Pagé
/// @date	November 2017
/// @brief	Contains the Engine class that encapsulate all m8 functionalities.

#include <iostream>

#include "Engine.hpp"

#include "../m8chess/SAN.hpp"

namespace m8
{
    Engine::Engine(EngineMoveCallback engine_move_callback)
        : state_(EngineState::Ready),
		  board_(kStartingPositionFEN),
		  engine_color_(kBlack),
		  search_(board_, std::bind(&Engine::OnSearchCompleted, this, std::placeholders::_1)),
		  engine_move_callback_(engine_move_callback)
    {
        board_.set_psqt(eval::GeneratePieceSqTable());
    }

    PerftResult Engine::Perft(int depth, std::function<void(std::string, std::uint64_t)> callback)
    {
		// TODO : Make sure this is handled by the interface.
		assert(state_ == EngineState::Ready);

        m8::Perft perft(depth, board_, [callback, this](Move move, std::uint64_t count) { callback(RenderSAN(move, board_), count); });
        return perft.RunParallel();
    }

	void Engine::OnSearchCompleted(const search::SearchResult& result)
	{
		assert(state_ == EngineState::Searching);

		auto move = result.GetBestMove();
		
		if (move != kNullMove)
		{
			engine_move_callback_(RenderSAN(move, board_));
			board_.Make(move);
		}

		state_ = EngineState::Ready;
	}

	void Engine::Go()
	{
		assert(state_ == EngineState::Ready);

		state_ = EngineState::Searching;

		engine_color_ = board_.side_to_move();

		search_.set_board(board_);
		search_.Start();
	}
}