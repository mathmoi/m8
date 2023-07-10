/// @file	EngineState.hpp
/// @author Mathieu Pagé
/// @date	Decembre 2019
/// @brief	Contains the EngineState class. This is an abstract class that represent the
///         state of the engine at a given time.

#ifndef M8_ENGINE_ENGINE_STATE_HPP_
#define M8_ENGINE_ENGINE_STATE_HPP_

#include <memory>
#include <stdexcept>
#include <string>

#include "../../m8/engine/Engine.hpp"

#include "../../m8chess/Perft.hpp"
#include "../../m8chess/Color.hpp"	
#include "../../m8chess/Board.hpp"
#include "../../m8chess/search/SearchSubject.hpp"

#include "../../m8chess/time/ChessClock.hpp"
#include "../../m8chess/time/TimeControl.hpp"
#include "../../m8chess/time/TimePerMoveTimeControl.hpp"

#include "EngineCallbacks.hpp"
#include "InvalidEngineCommandException.hpp"

namespace m8::engine
{

    /// Abstract base class for all engine state classes.
    class EngineState
    {
    public:

        /// Default constructor
        EngineState(Engine* engine)
            : engine_(engine)
        {}

        /// Destructor.
        virtual inline ~EngineState() {};

        /// Return the name of the state
        inline virtual const std::string state_name() const = 0;

        /// Method that is run when a state becomes the new current state.
        virtual inline void BeginState() {};
        
        /// Method that is run before a state is replaced by a new state
        virtual inline void EndState() {};

        /// Set the board position using a fen string.
        ///
        /// @param fen XFen string representing the new position.
        virtual inline void set_fen(std::string fen) { throw InvalidEngineCommandException("set_fen"); }

        /// Run a perft tests.
        ///
        /// @param depth Depth of the test to run.
        virtual inline void Perft(int depth) { throw InvalidEngineCommandException("Perft"); }

        /// Set the engine to play the current side and start playing.
        virtual inline void Go() { throw InvalidEngineCommandException("Go"); }

        /// Set the engine to play neither color.
        virtual inline void Force() { throw InvalidEngineCommandException("Force"); }

        /// Stops the current operation.
        virtual inline void Stop() { throw InvalidEngineCommandException("Stop"); }

        /// Terminate the current game and prepare the engine to play a new game.
        virtual inline void New() { throw InvalidEngineCommandException("New"); }

        /// Accept a move to play on the current board.
        virtual inline void UserMove(std::string move) { throw InvalidEngineCommandException("UserMove"); }

        /// Set the time control to a fixed number of seconds per move
        /// 
        /// @param seconds_per_move Number of seconds to use per move
        virtual inline void SetTimeControl(time::ChessClock::Duration time_per_move) { throw InvalidEngineCommandException("SetTimeControl"); }

        /// Set the time control to a conventional one.
        /// 
        /// @param moves Number of moves for each control
        /// @param time  Time added for each control
        virtual inline void SetTimeControl(std::uint32_t moves, time::ChessClock::Duration time) { throw InvalidEngineCommandException("SetTimeControl"); }

        /// Set the time control to an incremental one.
        /// 
        /// @param base      Base time
        /// @param increment Incremental time added after each move
        virtual inline void SetTimeControl(time::ChessClock::Duration base, time::ChessClock::Duration increment) { throw InvalidEngineCommandException("SetTimeControl"); }

    protected:

        // TODO : Remove if not used
        /*
        /// Change the state of the engine
        void ChangeState(EngineState* new_state);

        /// Get the color played by the engine.
        inline Color engine_color() const { return engine_->engine_color_; };

        /// Set the color played by the engine
        inline void set_engine_color(Color new_engine_color) { engine_->engine_color_ = new_engine_color; };

        /// Accessor for the callbacks
        inline const EngineCallbacks callbacks() const { return engine_->callbacks_; }
        */

        Engine* engine_;
    };
}

#endif // M8_ENGINE_ENGINE_STATE_HPP_