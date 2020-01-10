/// @file	Engine.cpp
/// @author Mathieu Pagé
/// @date	November 2017
/// @brief	Contains the Engine class that encapsulate all m8 functionalities.

#include <iostream>

#include "../../m8common/logging.hpp"

#include "Engine.hpp"
#include "ObservingState.hpp"

#include "../../m8chess/SAN.hpp"

namespace m8::engine
{
	Engine::Engine(EngineCallbacks callbacks)
	{
		state_ = new ObservingState(this, callbacks);
	}

	Engine::~Engine()
	{
		delete state_;
	}

	void Engine::ChangeState(EngineState* new_state)
	{
		if (new_state != state_)
		{
			M8_DEBUG << "ChangeState from " << state_->state_name() << " to " << new_state->state_name();

			state_->EndState();

			delete state_;
			state_ = new_state;

			state_->BeginState();
		}
	};

    /*Engine::Engine(EngineMoveCallback engine_move_callback)
        : state_(EngineStateEnum::Ready),
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
		assert(state_ == EngineStateEnum::Ready);

        m8::Perft perft(depth, board_, [callback, this](Move move, std::uint64_t count) { callback(RenderSAN(move, board_), count); });
        return perft.RunParallel();
    }

	void Engine::OnSearchCompleted(const search::SearchResult& result)
	{
		assert(state_ == EngineStateEnum::Searching);

		auto move = result.GetBestMove();
		
		if (move != kNullMove)
		{
			engine_move_callback_(RenderSAN(move, board_));
			board_.Make(move);
		}

		state_ = EngineStateEnum::Ready;
	}

	void Engine::Go()
	{
		assert(state_ == EngineStateEnum::Ready);

		state_ = EngineStateEnum::Searching;

		engine_color_ = board_.side_to_move();

		search_.set_board(board_);
		search_.Start();
	}

	void Engine::New()
	{
		if (search_.state() != search::SearchState::Ready)
		{
			search_.Stop();
		}

		board_ = Board(kStartingPositionFEN);
		engine_color_ = kBlack;
	}*/
}