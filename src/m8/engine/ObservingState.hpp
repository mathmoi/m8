/// @file	ObservingState.hpp 
/// @author Mathieu Pag�
/// @date	Decembre 2019
/// @brief	Contains the ObservingState class. When he is not involved in the game (not 
///         searching, pondering or waiting for an opponent to move.)

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
		ObservingState(Engine* engine)
			: EngineState(engine)
		{};

		/// Return the name of the state
        inline virtual const std::string state_name() const { return "ObservingState"; }

		/// Set the board position using a fen string.
        ///
        /// @param fen XFen string representing the new position.
        inline void set_fen(std::string fen) { engine_->board_ = Board(fen, engine_->psqt_); };

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

		/// Set the time control to a fixed number of seconds per move
        /// 
        /// @param seconds_per_move Number of seconds to use per move
        virtual void SetTimeControl(float seconds_per_move);

	private:
		Move ParseMove(const std::string& str_move);
	};
}

#endif // M8_ENGINE_OBSERVING_STATE_HPP_