/// @file   Engine.hpp
/// @author Mathieu Pagé
/// @date   November 2017
/// @brief  Contains the Engine class that encapsulate all m8 functionalities.

#ifndef M8_ENGINE_ENGINE_HPP_
#define M8_ENGINE_ENGINE_HPP_

#include <memory>
#include <string>

#include "m8chess/eval/Eval.hpp"

#include "m8chess/search/Searcher.hpp"
#include "m8chess/search/SearchSubject.hpp"

#include "m8chess/time/ChessClock.hpp"
#include "m8chess/time/TimeControl.hpp"

#include "m8chess/transposition/TranspositionTable.hpp"

#include "m8chess/Types.hpp"

#include "m8chess/IPerftObserver.hpp"

namespace m8::engine
{
    class EngineState;
    class ObservingState;
    class PerftState;
    class SearchState;
    class WaitingState;

    /// Encapsulate all m8 functionalities
    class Engine : public search::SearchSubject<std::vector<std::string>, std::string>
    {
        // Allows the EngineState class to change the current state of the engine
        friend class EngineState;

    public:
        /// Constructor
        Engine(eval::PieceSqTablePtr psqt);

        /// Destructor
        ~Engine();

        /// Accessor for the board
        ///
        /// @returns The board
        inline const Board& board() const { return board_; };

        /// Set the board position using a fen string.
        ///
        /// @param fen XFen string representing the new position.
        void SetBoard(std::string fen);

        /// Return engine's chess clock
        inline const time::ChessClock& clock() const { return *clock_; }

        /// Return the current time control
        inline const time::TimeControl& time_control() const { return *time_control_; }

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

        /// Run a perft tests.
        ///
        /// @param depth  epth of the test to run.
        void Perft(int depth, IPerftObserver* observer);

        /// Set the engine to play the current side and start playing.
        void Go();

        /// Set the engine to play neither color.
        void Force();

        /// Stop the current operation
        void Stop();

        /// Terminate the current game and prepare the engine to play a new game.
        void New();

        /// Accept a move to play on the current board.
        void UserMove(std::string move);

        /// Returns the current evaluation;
        EvalType current_evaluation() const;

    private:
        friend EngineState;
        friend ObservingState;
        friend PerftState;
        friend SearchState;
        friend WaitingState;

        const DepthType kMaxMaxDepth = 256;

        std::unique_ptr<EngineState> state_;

        eval::PieceSqTablePtr psqt_;
        Board board_;
        Color engine_color_;
        DepthType max_depth_;
        std::unique_ptr<time::TimeControl> time_control_;
        std::unique_ptr<time::ChessClock> clock_;

        //////////////////////////////////////////////////////////////////////////////////
        // Searching variables
        ////////////////////////////////////////////////////////////////////////////////// 
        transposition::TranspositionTable transposition_table_;
        search::Searcher searcher_;
        bool searching_;
        std::mutex search_mutex_;

        /// Change the state of the engine
        void ChangeState(std::unique_ptr<EngineState> new_state);
    };
}

#endif