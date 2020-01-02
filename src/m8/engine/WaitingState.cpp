/// @file	WaitingState.cpp
/// @author Mathieu Pagé
/// @date	Decembre 2019
/// @brief	Contains the WaitingState class. Controlling the engine behavior when it's 
///         waiting for commands.

#include <memory>

#include "WaitingState.hpp"
#include "PerftState.hpp"

namespace m8::engine {

	void WaitingState::Perft(int depth,
		                     EngineState::PartialPerftResultCallback partial_result_callback,
		                     EngineState::PerftResultCallback result_callback)
	{
		auto perft_state = new PerftState(this, depth, partial_result_callback, result_callback);
		ChangeState(perft_state);
	}
}