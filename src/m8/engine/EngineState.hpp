/// @file	EngineState.hpp
/// @author Mathieu Pagé
/// @date	Decembre 2019
/// @brief	Contains the EngineState class. This is an abstract class that represent the
///         state of the engine at a given time.

#ifndef M8_ENGINE_ENGINE_STATE_HPP_
#define M8_ENGINE_ENGINE_STATE_HPP_

#include <memory>
#include <stdexcept>

#include "../../m8chess/Perft.hpp"
#include "../../m8chess/Color.hpp"	
#include "../../m8chess/Board.hpp"

#include "InvalidEngineCommandException.hpp"

namespace m8::engine {

    // Forward declaration of the engine class.
    class Engine;

    /// Abstract base class for all engine state classes.
    class EngineState
    {
    public:
        typedef std::function<void(std::string, std::uint64_t)> PartialPerftResultCallback;
        typedef std::function<void(std::uint64_t, double)> PerftResultCallback;

        /// Default constructor
        EngineState(Engine* engine)
            : engine_(engine),
              board_(kStartingPositionFEN),
              engine_color_(kBlack)
        {}

        /// Constructor from a previous state
        EngineState(EngineState* source)
            : engine_(source->engine_),
              board_(source->board_),
              engine_color_(source->engine_color_)
        {}

        /// Accessor for the board
        ///
        /// @returns The board
        inline const Board& board() const { return board_; };

        /// Set the board position using a fen string.
        ///
        /// @param fen XFen string representing the new position.
        virtual inline void set_fen(std::string fen) { board_ = Board(fen); };

        /// Method that is run before a state is replaced by a new state
        virtual inline void BeginState() {};

        /// Method that is run when a state becomes the new current state.
        virtual inline void EndState() {};

        /// Run a perft tests.
        ///
        /// @param depth                   Depth of the test to run.
        /// @param partial_result_callback Method to call after each root move to give a 
        ///                                subcount of the nodes.
        /// @param result_callback         Method to call when the tests is finished to 
        ///                                return the nodes count and the time used to 
        ///                                perform the test.
        virtual inline void Perft(int depth,
                                  PartialPerftResultCallback partial_result_callback,
                                  PerftResultCallback result_callback)
        {
            throw InvalidEngineCommandException();
        };

        /// Set the engine to play the current side and start playing.
        virtual inline void Go()
        {
            throw InvalidEngineCommandException();
        };

        /// Stops the current operation.
        virtual inline void Stop()
        {
            throw InvalidEngineCommandException();
        }

        /// Terminate the current game and prepare the engine to play a new game.
        virtual inline void New()
        {
            throw InvalidEngineCommandException();
        };

    protected:

        /// Change the state of the engine
        void ChangeState(EngineState* new_state);

    private:
        Engine* engine_;

        Board board_;
        Color engine_color_;
    };
}

#endif // M8_ENGINE_ENGINE_STATE_HPP_