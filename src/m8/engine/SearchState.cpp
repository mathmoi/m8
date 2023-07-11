/// @file	SearchState.cpp
/// @author Mathieu Pagé
/// @date	January 2020
/// @brief	Contains the SearchState class. Controlling the engine behavior when it's 
///         searching for a move to play

#include <stack>

#include "../../m8common/logging.hpp"
#include "../options/Options.hpp"
#include "../../m8chess/SAN.hpp"
#include "../../m8chess/CoordinateNotation.hpp"
#include "../../m8chess/time/TimeManager.hpp"

#include "WaitingState.hpp"
#include "SearchState.hpp"
#include "ObservingState.hpp"

namespace m8::engine
{
	SearchState::SearchState(Engine* engine)
		: EngineState(engine)
	{
		engine->search_ = std::make_unique<search::Search>(engine->board_,
		                                                   time::TimeManager::CreateTimeManager(*(engine->time_control_),
														                                        *(engine->clock_)));
		engine->searching_ = true;
		engine->search_->Attach(this);
	}

	void SearchState::BeginState()
	{
		M8_DEBUG << engine_->board_.fen();

		engine_->clock_->Start();

		engine_->NotifySearchStarted();

		engine_->search_->Start();
	}

	void SearchState::OnSearchCompleted(const search::PV& pv, double time)
	{
		bool was_searching = false;

		{
			auto lock = std::lock_guard(engine_->search_mutex_);
			if (engine_->searching_)
			{
				was_searching = true;

				auto pv_str = RenderPVMoves(pv);

				engine_->board_.Make(pv.first());

				engine_->NotifySearchCompleted(pv_str, time);

				engine_->clock_->Stop();

				engine_->searching_ = false;
			}
		}

		if (was_searching)
		{
			SwitchToWaitingState();
		}
	}

	void SearchState::OnNewBestMove(const search::PV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes)
	{
		auto pv_str = RenderPVMoves(pv);
		engine_->NotifyNewBestMove(pv_str, eval, depth, time, nodes);
	}

	void SearchState::OnIterationCompleted(const search::PV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes)
	{
		auto pv_str = RenderPVMoves(pv);
		engine_->NotifyIterationCompleted(pv_str, eval, depth, time, nodes);
	}

	void SearchState::SwitchToWaitingState()
	{
		auto waiting_state = std::make_unique<WaitingState>(engine_);
		engine_->ChangeState(std::move(waiting_state));
	}

	void SearchState::Force()
	{
		auto was_searching = StopSearch();
		if (was_searching)
		{
			SwitchToObservingState();
		}
	}

	void SearchState::Stop()
	{
		Force();
	}

	void SearchState::SwitchToObservingState()
	{
		auto observing_state = std::make_unique<ObservingState>(engine_);
		engine_->ChangeState(std::move(observing_state));
	}

	bool SearchState::StopSearch()
	{
		auto lock = std::lock_guard(engine_->search_mutex_);
		
		bool was_searching = engine_->searching_;

		if (engine_->searching_)
		{
			engine_->search_->Stop();
			engine_->searching_ = false;
		}

		return was_searching;
	}

	std::vector<std::string> SearchState::RenderPVMoves(const search::PV& pv)
	{
		std::vector<std::string> moves;
		std::stack<UnmakeInfo> unmake_info_stack;

		for (size_t x = 0; x < pv.count(); ++x)
		{
			std::string str_move = options::Options::get().use_san ? RenderSAN(pv[x], engine_->board_) : RenderCoordinateNotation(pv[x]);
			moves.push_back(str_move);
			unmake_info_stack.push(engine_->board_.Make(pv[x]));
		}

		while (unmake_info_stack.size() > 0)
		{
			engine_->board_.Unmake(pv[unmake_info_stack.size() - 1], unmake_info_stack.top());
			unmake_info_stack.pop();
		}
		
		return moves;
	}
}