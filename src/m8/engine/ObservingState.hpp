/// @file   ObservingState.hpp 
/// @author Mathieu Pagé
/// @date   Decembre 2019
/// @brief  Contains the ObservingState class. When he is not involved in the game (not 
///         searching, pondering or waiting for an opponent to move.)

#ifndef M8_ENGINE_OBSERVING_STATE_HPP_
#define M8_ENGINE_OBSERVING_STATE_HPP_

#include "EngineState.hpp"

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
        inline const std::string state_name() const { return "ObservingState"; }

        /// Set the board position using a fen string.
        ///
        /// @param fen XFen string representing the new position.
        inline void SetBoard(std::string fen) { engine_->board_ = Board(fen, engine_->psqt_); };

        /// Run a perft tests.
        ///
        /// @param depth Depth of the test to run.
        void Perft(int depth, IPerftObserver* observer);

        /// Accept a move to play on the current board.
        void UserMove(std::string move);

        /// Terminate the current game and prepare the engine to play a new game.
        void New();

        /// Set the engine to play the current side and start playing.
        void Go();

        /// Set the engine to play neither color.
        inline void Force() { /* do nothing */ }

        /// Stops the current operation.
        inline void Stop() { /* do nothing */ }

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

        /// Set the maximum depth the engine should search.
        void SetDepth(DepthType depth);

    private:
        Move ParseMove(const std::string& str_move);
    };
}

#endif // M8_ENGINE_OBSERVING_STATE_HPP_