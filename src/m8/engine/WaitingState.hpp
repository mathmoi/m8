/// @file	WaitingState.hpp
/// @author Mathieu Pagé
/// @date	January 2020
/// @brief	Contains the WaitingState class. Controlling the engine behavior when it's 
///         waiting for the user to play.

#ifndef M8_ENGINE_WAITING_STATE_HPP_
#define M8_ENGINE_WAITING_STATE_HPP_

#include "EngineState.hpp"

namespace m8::engine {

	/// Class controlling the engine behavior in the waiting state.
	class WaitingState : public EngineState
	{
	public:

		/// Constructor from a previous state
		WaitingState(EngineState* source);

		/// Accept a move to play on the current board.
		virtual void UserMove(std::string move);

	private:
		Move ParseMove(const std::string& str_move);
	};
}

#endif // M8_ENGINE_WAITING_STATE_HPP_