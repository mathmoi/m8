/// @file	WaitingState.hpp
/// @author Mathieu Pagé
/// @date	Decembre 2019
/// @brief	Contains the WaitingState class. Controlling the engine behavior when it's 
///         waiting for commands.

#ifndef M8_ENGINE_WAITING_STATE_HPP_
#define M8_ENGINE_WAITING_STATE_HPP_

#include "EngineState.hpp"

namespace m8::engine {

	/// Class controlling the engine behavior in the waiting state.
	class WaitingState : public EngineState
	{
	public:
		/// Default constructor
		WaitingState(Engine* engine)
			: EngineState(engine)
		{};

		/// Constructor from a previous state
		WaitingState(EngineState* source)
			: EngineState(source)
		{}

		/// Run a perft tests.
		///
		/// @param depth                   Depth of the test to run.
		/// @param partial_result_callback Method to call after each root move to give a 
		///                                subcount of the nodes.
		/// @param result_callback         Method to call when the tests is finished to 
		///                                return the nodes count and the time used to 
		///                                perform the test.
		virtual void Perft(int depth,
			               EngineState::PartialPerftResultCallback partial_result_callback,
			               EngineState::PerftResultCallback        result_callback);
	};
}

#endif // M8_ENGINE_WAITING_STATE_HPP_