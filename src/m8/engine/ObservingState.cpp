/// @file	ObservingState.cpp
/// @author Mathieu Pagé
/// @date	Decembre 2019
/// @brief	Contains the WaitingState class. Controlling the engine behavior when it's 
///         waiting for commands.

#include <memory>

#include "ObservingState.hpp"
#include "WaitingState.hpp"
#include "PerftState.hpp"

namespace m8::engine {

	void ObservingState::Perft(int depth)
	{
		auto perft_state = new PerftState(this, depth);
		ChangeState(perft_state);
	}

	void ObservingState::New()
	{
		set_fen(kStartingPositionFEN);
		set_engine_color(kBlack);

		auto waiting_state = new WaitingState(this);
		ChangeState(waiting_state);
	}
}