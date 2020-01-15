/// @file	Engine.cpp
/// @author Mathieu Pag�
/// @date	November 2017
/// @brief	Contains the Engine class that encapsulate all m8 functionalities.

#include <iostream>

#include "../../m8common/logging.hpp"

#include "Engine.hpp"
#include "ObservingState.hpp"

#include "../../m8chess/SAN.hpp"

namespace m8::engine
{
	Engine::Engine(EngineCallbacks callbacks)
	{
		state_ = new ObservingState(this, callbacks);
	}

	Engine::~Engine()
	{
		delete state_;
	}

	void Engine::ChangeState(EngineState* new_state)
	{
		if (new_state != state_)
		{
			M8_DEBUG << "ChangeState from " << state_->state_name() << " to " << new_state->state_name();

			state_->EndState();

			delete state_;
			state_ = new_state;

			state_->BeginState();
		}
	}
}