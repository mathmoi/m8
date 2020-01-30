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
			           eval::PieceSqTablePtr psqt,
			           EngineCallbacks callbacks,
			           search::SearchObserver* observer)
			: EngineState("ObservingState", engine, psqt, callbacks, observer)
		{};

		/// Constructor from a previous state
		ObservingState(EngineState* source)
			: EngineState("ObservingState", source)
		{}

		/// Run a perft tests.
		///
		/// @param depth                   Depth of the test to run.
		virtual void Perft(int depth);

		/// Accept a move to play on the current board.
		virtual void UserMove(std::string move);

		/// Terminate the current game and prepare the engine to play a new game.
		virtual void New();

		/// Set the engine to play the current side and start playing.
		virtual void Go();

		/// Set the engine to play neither color.
		virtual inline void Force() { /* do nothing */ }

	private:
		Move ParseMove(const std::string& str_move);
	};
}

#endif // M8_ENGINE_OBSERVING_STATE_HPP_