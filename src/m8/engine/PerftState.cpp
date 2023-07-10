/// @file	PerftState.cpp
/// @author Mathieu Pag�
/// @date	Decembre 2019
/// @brief	Contains the PerftState class. Controlling the engine behavior when it's 
///         executing a perft test.

#include "PerftState.hpp"
#include "ObservingState.hpp"

namespace m8::engine {

	void PerftState::BeginState()
	{
		perft_.RunParallel();
	}

	void PerftState::HandleResult(std::uint64_t count, double seconds)
	{
		engine_->callbacks_.perft_result_callback(count, seconds);
		
		auto waiting_state = std::make_unique<ObservingState>(this->engine_);
		engine_->ChangeState(std::move(waiting_state));
	}

	void PerftState::Stop()
	{
		perft_.Abort();

		auto waiting_state = std::make_unique<ObservingState>(this->engine_);
		engine_->ChangeState(std::move(waiting_state));
	}

}