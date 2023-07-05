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

#include "../../m8chess/Perft.hpp"
#include "../../m8chess/Color.hpp"	
#include "../../m8chess/Board.hpp"
#include "../../m8chess/search/SearchObserver.hpp"

#include "../../m8chess/time/ChessClock.hpp"
#include "../../m8chess/time/TimeControl.hpp"
#include "../../m8chess/time/TimePerMoveTimeControl.hpp"

#include "EngineCallbacks.hpp"
#include "InvalidEngineCommandException.hpp"

namespace m8::engine {

    // Forward declaration of the engine class.
    class Engine;

    /// Abstract base class for all engine state classes.
    class EngineState
    {
    public:

        /// Default constructor
        EngineState(std::string state_name,
                    Engine* engine,
                    eval::PieceSqTablePtr psqt,
                    const EngineCallbacks callbacks,
                    search::SearchObserver* observer)
            : state_name_(state_name),
              engine_(engine),
              callbacks_(callbacks),
              psqt_(psqt),
              board_(kStartingPositionFEN, psqt),
              engine_color_(kBlack),
              time_control_(std::make_unique<time::TimePerMoveTimeControl>(std::chrono::seconds(1))),
              clock_(time::ChessClock::CreateChessClock(*time_control_)),
              observer_(observer)
        {}

        /// Constructor from a previous state
        EngineState(std::string state_name, 
                    EngineState* source)
            : state_name_(state_name),
              engine_(source->engine_),
              callbacks_(source->callbacks_),
              psqt_(source->psqt_),
              board_(source->board_),
              engine_color_(source->engine_color_),
              time_control_(std::move(source->time_control_)),
              clock_(std::move(source->clock_)),
              observer_(source->observer_)
        {}

        /// Destructor.
        virtual inline ~EngineState() {};

        /// Accessor for the board
        ///
        /// @returns The board
        inline Board& board() { return board_; };

        /// Set the board position using a fen string.
        ///
        /// @param fen XFen string representing the new position.
        inline void set_fen(std::string fen) { board_ = Board(fen, psqt_); };

        /// Return the name of the state
        inline const std::string& state_name() const { return state_name_; }

        /// Return the current time control
        inline const time::TimeControl& time_control() const { return *time_control_; }

        /// Return engine's chess clock
        inline time::ChessClock& clock() const { return *clock_; }

        /// Set the current time control
        inline void set_time_control(std::unique_ptr<time::TimeControl> value)
        {
            time_control_ = std::move(value);
            clock_ = time::ChessClock::CreateChessClock(*time_control_);
        }

        /// Method that is run before a state is replaced by a new state
        virtual inline void BeginState() {};

        /// Method that is run when a state becomes the new current state.
        virtual inline void EndState() {};

        /// Run a perft tests.
        ///
        /// @param depth                   Depth of the test to run.
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
        virtual inline void SetTimeControl(float seconds_per_move) { throw InvalidEngineCommandException("SetTimeControl"); };

    protected:

        /// Change the state of the engine
        void ChangeState(EngineState* new_state);

        /// Get the color played by the engine.
        inline Color engine_color() const { return engine_color_; };

        /// Set the color played by the engine
        inline void set_engine_color(Color new_engine_color) { engine_color_ = new_engine_color; };

        /// Accessor for the callbacks
        inline const EngineCallbacks callbacks() const { return callbacks_; }

        /// Accessor for the observer
        inline search::SearchObserver* observer() const { return observer_; }

    private:
        std::string state_name_;

        Engine* engine_;

        EngineCallbacks callbacks_;

        eval::PieceSqTablePtr psqt_;
        Board board_;
        Color engine_color_;
        std::unique_ptr<time::TimeControl> time_control_;
        std::unique_ptr<time::ChessClock> clock_;

        search::SearchObserver* observer_;
    };
}

#endif // M8_ENGINE_ENGINE_STATE_HPP_