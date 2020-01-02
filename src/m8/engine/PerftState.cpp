/// @file	PerftState.cpp
/// @author Mathieu Pagé
/// @date	Decembre 2019
/// @brief	Contains the PerftState class. Controlling the engine behavior when it's 
///         executing a perft test.

#include "PerftState.hpp"
#include "WaitingState.hpp"

namespace m8::engine {

	void PerftState::BeginState()
	{
		perft_.RunParallel();
	}

	void PerftState::HandleResult(std::uint64_t count, double seconds)
	{
		result_callback_(count, seconds);
		
		auto waiting_state = new WaitingState(this);
		ChangeState(waiting_state);
	}

	void PerftState::Stop()
	{
		perft_.Abort();

		auto waiting_state = new WaitingState(this);
		ChangeState(waiting_state);
	}

}