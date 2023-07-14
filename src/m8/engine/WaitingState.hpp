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
		WaitingState(Engine* engine);

		/// Return the name of the state
        inline const std::string state_name() const { return "WaitingState"; }

		/// Accept a move to play on the current board.
		void UserMove(std::string move);

		/// Terminate the current game and prepare the engine to play a new game.
		void New();

		/// Set the engine to play the current side and start playing.
		void Go();

		/// Set the engine to play neither color.
		void Force();

		/// Stops the current operation.
        inline void Stop() { Force(); }

		/// Set the time control to a fixed number of seconds per move
        /// 
        /// @param seconds_per_move Number of seconds to use per move
        void SetTimeControl(time::ChessClock::Duration time_per_move);

		/// Set the time control to a conventional one.
        /// 
        /// @param moves Number of moves for each control
        /// @param time  Time added for each control
        void SetTimeControl(std::uint32_t moves, time::ChessClock::Duration time);

        /// Set the time control to an incremental one.
        /// 
        /// @param base      Base time
        /// @param increment Incremental time added after each move
        void SetTimeControl(time::ChessClock::Duration base, time::ChessClock::Duration increment);

	private:
		Move ParseMove(const std::string& str_move);
	};
}

#endif // M8_ENGINE_WAITING_STATE_HPP_