/// @file	ThinkingState.cpp
/// @author Mathieu Pagé
/// @date	January 2020
/// @brief	Contains the ThinkingState class. Controlling the engine behavior when it's 
///         searching for a move to play

#include "../../m8common/logging.hpp"

#include "../../m8chess/SAN.hpp"

#include "WaitingState.hpp"
#include "ThinkingState.hpp"
#include "ObservingState.hpp"

namespace m8::engine
{
	ThinkingState::ThinkingState(EngineState* source)
		: EngineState("ThinkingState", source),
		search_(source->board(), this),
		searching_(true)
	{}

	void ThinkingState::BeginState()
	{
		M8_DEBUG << this->board().fen();

		observer()->OnBeginSearch();

		search_.Start();
	}

	void ThinkingState::OnSearchCompleted(Move move, double time)
	{
		bool was_searching = false;

		{
			auto lock = std::lock_guard(state_mutex_);
			if (searching_)
			{
				was_searching = true;

				auto move_text = RenderSAN(move, board());
				this->board().Make(move);
				observer()->OnSearchCompleted(move_text, time);
				searching_ = false;
			}
		}

		if (was_searching)
		{
			SwitchToWaitingState();
		}
	}

	void ThinkingState::OnNewBestMove(Move move, EvalType eval, DepthType depth, double time, NodeCounterType nodes)
	{
		auto move_text = RenderSAN(move, board());
		observer()->OnNewBestMove(move_text, eval, depth, time, nodes);
	}

	void ThinkingState::OnIterationCompleted(Move move, EvalType eval, DepthType depth, double time, NodeCounterType nodes)
	{
		auto move_text = RenderSAN(move, board());
		observer()->OnIterationCompleted(move_text, eval, depth, time, nodes);
	}

	void ThinkingState::SwitchToWaitingState()
	{
		auto waiting_state = new WaitingState(this);
		ChangeState(waiting_state);
	}

	void ThinkingState::Force()
	{
		auto was_searching = StopSearch();
		if (was_searching)
		{
			SwitchToObservingState();
		}
	}

	void ThinkingState::SwitchToObservingState()
	{
		auto observing_state = new ObservingState(this);
		ChangeState(observing_state);
	}

	bool ThinkingState::StopSearch()
	{
		auto lock = std::lock_guard(state_mutex_);
		
		bool was_searching = searching_;

		if (searching_)
		{
			search_.Stop();
			searching_ = false;
		}

		return was_searching;
	}
}