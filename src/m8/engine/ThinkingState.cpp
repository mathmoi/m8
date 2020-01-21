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
		search_(source->board(),
			    std::bind(&ThinkingState::HandleSearchResult, this, std::placeholders::_1)),
		searching_(true)
	{}

	void ThinkingState::BeginState()
	{
		M8_DEBUG << this->board().fen();
		search_.Start();
	}

	void ThinkingState::HandleSearchResult(search::SearchResult result)
	{
		bool was_searching = false;

		{
			auto lock = std::lock_guard(state_mutex_);
			if (searching_)
			{
				was_searching = true;
				std::string move = RenderSAN(result.GetBestMove(), this->board());

				MakeEngineMove(result);
				SendResultToUser(move, result);
				searching_ = false;
			}
		}

		if (was_searching)
		{
			SwitchToWaitingState();
		}
	}
	void ThinkingState::SwitchToWaitingState()
	{
		auto waiting_state = new WaitingState(this);
		ChangeState(waiting_state);
	}

	void ThinkingState::SendResultToUser(const std::string& move, m8::search::SearchResult& result)
	{
		double value = eval::GetEvaluationInPawns(result.GetValue());
		this->callbacks().search_result_callback(move, value);
	}

	void ThinkingState::MakeEngineMove(m8::search::SearchResult& result)
	{
		this->board().Make(result.GetBestMove());
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