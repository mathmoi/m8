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

namespace m8::engine
{
    /// Encapsulate all m8 functionalities
    class Engine
    {
        // Allows the EngineState class to change the current state of the engine
        friend class EngineState;

    public:
        /// Constructor
        Engine(EngineCallbacks callbacks);

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

        /// Stop the current operation
        void Stop() { state_->Stop(); };

        /// Terminate the current game and prepare the engine to play a new game.
        void New() { state_->New(); };

        /// Accept a move to play on the current board.
        void UserMove(std::string move) { state_->UserMove(move); };

    private:
        EngineState* state_;

        /// Change the state of the engine
        void ChangeState(EngineState* new_state);
    };


    /*
	/// Represents the possibles states of the engine class.
	enum class EngineStateEnum // TODO : Remove this type completely or rename it better.
	{
		Ready,
		Searching
	};

    /// Encapsulate all m8 functionalities
    class Engine
    {
    public:
		typedef std::function<void(const std::string&)> EngineMoveCallback;

        /// Constructor
        Engine(EngineMoveCallback engine_move_callback);

        /// Accessor for the board
        ///
        /// @returns The board
        inline const Board& board() const { return board_; };

        /// Set the board position using a fen string.
        ///
        /// @param fen XFen string representing the new position.
        inline void set_fen(std::string fen) { board_ = Board(fen); };

		/// Accessor for the state of the engine.
		inline EngineStateEnum state() const { return state_; };

        /// Run a perft tests.
        ///
        /// @param depth    Depth of the test to run.
        /// @param board    Position to run the test on.
        /// @param callback Method to call after each root move to give a subcount of the 
        ///                 nodes.
        PerftResult Perft(int depth, std::function<void(std::string, std::uint64_t)> callback);

		/// Set the engine to play the current side and start playing.
		void Go();

		/// Terminate the current game and prepare the engine to play a new game.
		void New();
        
    private:
		// class attributes
		EngineStateEnum state_;

        Board board_;
		Color engine_color_;

		search::Search search_;

		// callbacks
		EngineMoveCallback engine_move_callback_;

		// private methods
		void OnSearchCompleted(const search::SearchResult& result);
    };
    */
}

#endif