/// @file	ThinkingState.cpp
/// @author Mathieu Pagé
/// @date	January 2020
/// @brief	Contains the ThinkingState class. Controlling the engine behavior when it's 
///         searching for a move to play

// TODO : Remove this
#include <iostream>
#include <functional>

#include "../../m8chess/SAN.hpp"

#include "WaitingState.hpp"

#include "ThinkingState.hpp"

namespace m8::engine
{
	ThinkingState::ThinkingState(EngineState* source)
		: EngineState("ThinkingState", source),
		search_(source->board(),
			    std::bind(&ThinkingState::HandleSearchResult, this, std::placeholders::_1))
	{}

	void ThinkingState::BeginState()
	{
		search_.Start();
	}

	void ThinkingState::HandleSearchResult(search::SearchResult result)
	{
		std::string move = RenderSAN(result.GetBestMove(), this->board());

		MakeEngineMove(result);
		SendResultToUser(move, result);
		SwitchToWaitingState();
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
}