 /// @file	Engine.hpp
/// @author Mathieu Pagé
/// @date	November 2017
/// @brief	Contains the Engine class that encapsulate all m8 functionalities.

#ifndef M8_ENGINE_HPP_
#define M8_ENGINE_HPP_

#include "../m8chess/Board.hpp"
#include "../m8chess/Perft.hpp"
#include "../m8chess/search/Search.hpp"

namespace m8
{
	/// Represents the possibles states of the engine class.
	enum class EngineState 
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
		inline EngineState state() const { return state_; };

        /// Run a perft tests.
        ///
        /// @param depth    Depth of the test to run.
        /// @param board    Position to run the test on.
        /// @param callback Method to call after each root move to give a subcount of the 
        ///                 nodes.
        PerftResult Perft(int depth, std::function<void(std::string, std::uint64_t)> callback);

		/// Set the engine to play the current side and start playing.
		void Go();
        
    private:
		// class attributes
		EngineState state_;

        Board board_;
		Color engine_color_;

		search::Search search_;

		// callbacks
		EngineMoveCallback engine_move_callback_;

		// private methods
		void OnSearchCompleted(const search::SearchResult& result);
    };
}

#endif