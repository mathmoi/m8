/// @file	ThinkingState.cpp
/// @author Mathieu Pagé
/// @date	January 2020
/// @brief	Contains the ThinkingState class. Controlling the engine behavior when it's 
///         searching for a move to play

#include <stack>

#include "../../m8common/logging.hpp"
#include "../options/Options.hpp"
#include "../../m8chess/SAN.hpp"
#include "../../m8chess/CoordinateNotation.hpp"
#include "../../m8chess/time/TimeManager.hpp"

#include "WaitingState.hpp"
#include "ThinkingState.hpp"
#include "ObservingState.hpp"

namespace m8::engine
{
	ThinkingState::ThinkingState(EngineState* source)
		: EngineState("ThinkingState", source),
		search_(source->board(), time::TimeManager::CreateTimeManager(this->time_control(), this->clock()), this),
		searching_(true)
	{}

	void ThinkingState::BeginState()
	{
		M8_DEBUG << this->board().fen();

		clock().Start();

		observer()->OnBeginSearch();

		search_.Start();
	}

	void ThinkingState::OnSearchCompleted(const search::PV& pv, double time)
	{
		bool was_searching = false;

		{
			auto lock = std::lock_guard(state_mutex_);
			if (searching_)
			{
				was_searching = true;

				auto pv_str = RenderPVMoves(pv);

				this->board().Make(pv.first());

				observer()->OnSearchCompleted(pv_str, time);

				clock().Stop();

				searching_ = false;
			}
		}

		if (was_searching)
		{
			SwitchToWaitingState();
		}
	}

	void ThinkingState::OnNewBestMove(const search::PV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes)
	{
		auto pv_str = RenderPVMoves(pv);
		observer()->OnNewBestMove(pv_str, eval, depth, time, nodes);
	}

	void ThinkingState::OnIterationCompleted(const search::PV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes)
	{
		auto pv_str = RenderPVMoves(pv);
		observer()->OnIterationCompleted(pv_str, eval, depth, time, nodes);
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

	std::vector<std::string> ThinkingState::RenderPVMoves(const search::PV& pv)
	{
		std::vector<std::string> moves;
		std::stack<UnmakeInfo> unmake_info_stack;

		Board& boardr = board();

		for (size_t x = 0; x < pv.count(); ++x)
		{
			std::string str_move = options::Options::get().use_san ? RenderSAN(pv[x], boardr) : RenderCoordinateNotation(pv[x]);
			moves.push_back(str_move);
			unmake_info_stack.push(boardr.Make(pv[x]));
		}

		while (unmake_info_stack.size() > 0)
		{
			boardr.Unmake(pv[unmake_info_stack.size() - 1], unmake_info_stack.top());
			unmake_info_stack.pop();
		}
		
		return moves;
	}
}