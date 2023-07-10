/// @file	Engine.hpp
/// @author Mathieu Pagé
/// @date	November 2017
/// @brief	Contains the Engine class that encapsulate all m8 functionalities.

#ifndef M8_ENGINE_ENGINE_HPP_
#define M8_ENGINE_ENGINE_HPP_

#include <memory>
#include <string>

#include "EngineCallbacks.hpp"
#include "../../m8chess/search/Search.hpp"
#include "../../m8chess/search/SearchSubject.hpp"
#include "../../m8chess/eval/Eval.hpp"
#include "../../m8chess/time/ChessClock.hpp"
#include "../../m8chess/time/TimeControl.hpp"

namespace m8::engine
{
    class EngineState;
    class ObservingState;
    class PerftState;
    class ThinkingState;
    class WaitingState;

    /// Encapsulate all m8 functionalities
    class Engine : public search::SearchSubject<std::vector<std::string>>
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
        inline const Board& board() const { return board_; };

        /// Set the board position using a fen string.
        ///
        /// @param fen XFen string representing the new position.
        void set_fen(std::string fen);

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

        /// Run a perft tests.
        ///
        /// @param depth  epth of the test to run.
        void Perft(int depth);

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

        // TODO : Separate all other methods into sections

    private:
        friend EngineState;
        friend ObservingState;
        friend PerftState;
        friend ThinkingState;
        friend WaitingState;

        EngineState* state_;

        EngineCallbacks callbacks_; // TODO : Is this still needed now that we use the ObserverPattern?

        eval::PieceSqTablePtr psqt_;
        Board board_;
        Color engine_color_;
        std::unique_ptr<time::TimeControl> time_control_;
        std::unique_ptr<time::ChessClock> clock_;

        //////////////////////////////////////////////////////////////////////////////////
        // Searching variables
        ////////////////////////////////////////////////////////////////////////////////// 
        // TODO : The search should become a permanent/reusable object
        std::unique_ptr<search::Search> search_;
		bool searching_;
		std::mutex search_mutex_; // TODO : This mutex should probable become more global to make the Engine class completely thread safe.

        /// Change the state of the engine
        void ChangeState(EngineState* new_state); // TODO : Is this used?
    };
}

#endif