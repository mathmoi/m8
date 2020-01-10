/// @file	EngineState.cpp
/// @author Mathieu Pag�
/// @date	Decembre 2019
/// @brief	Contains the EngineState class. This is an abstract class that represent the
///         state of the engine at a given time.

#include "Engine.hpp"

namespace m8::engine {

	void EngineState::ChangeState(EngineState* new_state)
	{
		engine_->ChangeState(new_state);
	}
}