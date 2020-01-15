/// @file	EngineState.hpp
/// @author Mathieu Pag�
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
                    const EngineCallbacks callbacks)
            : state_name_(state_name),
              engine_(engine),
              callbacks_(callbacks),
              board_(kStartingPositionFEN),
              engine_color_(kBlack)
        {}

        /// Constructor from a previous state
        EngineState(std::string state_name, 
                    EngineState* source)
            : state_name_(state_name),
              engine_(source->engine_),
              callbacks_(source->callbacks_),
              board_(source->board_),
              engine_color_(source->engine_color_)
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
        virtual inline void set_fen(std::string fen) { board_ = Board(fen); };

        /// Return the name of the state
        inline const std::string& state_name() const { return state_name_; }

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

    protected:

        /// Change the state of the engine
        void ChangeState(EngineState* new_state);

        /// Get the color played by the engine.
        inline Color engine_color() const { return engine_color_; };

        /// Set the color played by the engine
        inline void set_engine_color(Color new_engine_color) { engine_color_ = new_engine_color; };

        /// Accessor for the callbacks
        inline const EngineCallbacks callbacks() const { return callbacks_; }

    private:
        std::string state_name_;

        Engine* engine_;

        EngineCallbacks callbacks_;

        Board board_;
        Color engine_color_;
    };
}

#endif // M8_ENGINE_ENGINE_STATE_HPP_