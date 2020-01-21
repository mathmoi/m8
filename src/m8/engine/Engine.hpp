/// @file	Engine.hpp
/// @author Mathieu Pagé
/// @date	November 2017
/// @brief	Contains the Engine class that encapsulate all m8 functionalities.

#ifndef M8_ENGINE_ENGINE_HPP_
#define M8_ENGINE_ENGINE_HPP_

#include <memory>
#include <string>

#include "EngineState.hpp"
#include "EngineCallbacks.hpp"
#include "../../m8chess/eval/Eval.hpp"

namespace m8::engine
{
    /// Encapsulate all m8 functionalities
    class Engine
    {
        // Allows the EngineState class to change the current state of the engine
        friend class EngineState;

    public:
        /// Constructor
        Engine(eval::PieceSqTablePtr psqt,
               EngineCallbacks callbacks);

        /// Destructor
        ~Engine();

        /// Accessor for the board
        ///
        /// @returns The board
        inline const Board& board() const { return state_->board(); };

        /// Set the board position using a fen string.
        ///
        /// @param fen XFen string representing the new position.
        inline void set_fen(std::string fen) { state_->set_fen(fen); };

        /// Run a perft tests.
        ///
        /// @param depth                   Depth of the test to run.
        inline void Perft(int depth) { return state_->Perft(depth); };

        /// Set the engine to play the current side and start playing.
        void Go() { state_->Go(); };

        /// Set the engine to play neither color.
        void Force() { state_->Force(); }

        /// Stop the current operation
        void Stop() { state_->Stop(); };

        /// Terminate the current game and prepare the engine to play a new game.
        void New() { state_->New(); };

        /// Accept a move to play on the current board.
        void UserMove(std::string move) { state_->UserMove(move); };

        /// Returns the current evaluation;
        eval::EvalType current_evaluation() const;

    private:
        EngineState* state_;

        /// Change the state of the engine
        void ChangeState(EngineState* new_state);
    };
}

#endif