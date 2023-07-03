/// @file	Engine.cpp
/// @author Mathieu Pagé
/// @date	November 2017
/// @brief	Contains the Engine class that encapsulate all m8 functionalities.

#include <iostream>

#include "../../m8common/logging.hpp"

#include "Engine.hpp"
#include "ObservingState.hpp"

#include "../../m8chess/SAN.hpp"

namespace m8::engine
{
	Engine::Engine(eval::PieceSqTablePtr psqt,
		           EngineCallbacks callbacks,
		           search::SearchObserver* observer)
	{
		state_ = new ObservingState(this, psqt, callbacks, observer);
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
		else
		{
			M8_DEBUG << "Same state (" << state_->state_name() <<"), no need to change.";
		}
	}

	EvalType Engine::current_evaluation() const
	{
		return eval::Evaluate(state_->board());
	}
}