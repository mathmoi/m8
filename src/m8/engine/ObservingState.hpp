/// @file	ObservingState.hpp
/// @author Mathieu Pagé
/// @date	Decembre 2019
/// @brief	Contains the WaitingState class. Controlling the engine behavior when it's 
///         waiting for commands.

#ifndef M8_ENGINE_OBSERVING_STATE_HPP_
#define M8_ENGINE_OBSERVING_STATE_HPP_

#include "EngineState.hpp"
#include "EngineCallbacks.hpp"

namespace m8::engine {

	/// Class controlling the engine behavior in the observing state.
	class ObservingState : public EngineState
	{
	public:
		/// Default constructor
		ObservingState(Engine* engine,
			           EngineCallbacks callbacks)
			: EngineState("ObservingState", engine, callbacks)
		{};

		/// Constructor from a previous state
		ObservingState(EngineState* source)
			: EngineState("ObservingState", source)
		{}

		/// Run a perft tests.
		///
		/// @param depth                   Depth of the test to run.
		virtual void Perft(int depth);

		/// Terminate the current game and prepare the engine to play a new game.
		virtual void New();
	};
}

#endif // M8_ENGINE_OBSERVING_STATE_HPP_