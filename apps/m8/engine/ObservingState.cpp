/// @file   ObservingState.cpp
/// @author Mathieu Pagé
/// @date   Decembre 2019
/// @brief  Contains the WaitingState class. Controlling the engine behavior when it's 
///         waiting for commands.

#include <memory>

#include "m8common/options/Options.hpp"
#include "m8chess/CoordinateNotation.hpp"
#include "m8chess/time/ConventionalTimeControl.hpp"
#include "m8chess/time/IncrementalTimeControl.hpp"
#include "m8chess/time/TimePerMoveChessClock.hpp"
#include "m8chess/time/ConventionalChessClock.hpp"
#include "m8chess/time/IncrementalChessClock.hpp"

#include "ObservingState.hpp"
#include "WaitingState.hpp"
#include "PerftState.hpp"
#include "SearchState.hpp"
#include "InvalidMoveException.hpp"

namespace m8::engine {

    void ObservingState::Perft(int depth, IPerftObserver* observer)
    {
        auto perft_state = std::make_unique<PerftState>(this->engine_, depth, observer);
        engine_->ChangeState(std::move(perft_state));
    }

    void ObservingState::UserMove(std::string str_move)
    {
        Move move = ParseMove(str_move);

        engine_->board_.Make(move);
    }

    Move ObservingState::ParseMove(const std::string& str_move)
    {
        Move move = kNullMove;

        try
        {
            move = options::Options::get().use_san ? ParseSAN(str_move, engine_->board_) : ParseCoordinateNotation(str_move, engine_->board_);
        }
        catch (const InvalidMoveNotationException&)
        {
            throw InvalidMoveException("Invalid Move : " + str_move);
        }

        return move;
    }

    void ObservingState::New()
    {
        engine_->board_ = Board(kStartingPositionFEN, engine_->psqt_);
        engine_->engine_color_ = kBlack;
        engine_->clock_ = time::ChessClock::CreateChessClock(*engine_->time_control_);
        engine_->max_depth_ = engine_->kMaxMaxDepth;

        auto waiting_state = std::make_unique<WaitingState>(engine_);
        engine_->ChangeState(std::move(waiting_state));
    }

    void ObservingState::Go()
    {
        engine_->engine_color_ = engine_->board_.side_to_move();

        auto thinking_state = std::make_unique<SearchState>(engine_);
        engine_->ChangeState(std::move(thinking_state));
    }

    void ObservingState::SetTimeControl(time::ChessClock::Duration time_per_move)
    {
        engine_->time_control_ = std::make_unique<time::TimePerMoveTimeControl>(time_per_move);
        engine_->clock_ = std::make_unique<time::TimePerMoveChessClock>(static_cast<time::TimePerMoveTimeControl&>(*engine_->time_control_));
    }

    void ObservingState::SetTimeControl(std::uint32_t moves, time::ChessClock::Duration time)
    {
        engine_->time_control_ = std::make_unique<time::ConventionalTimeControl>(moves, time);
        engine_->clock_ = std::make_unique<time::ConventionalChessClock>(static_cast<time::ConventionalTimeControl&>(*engine_->time_control_));
    }

    void ObservingState::SetTimeControl(time::ChessClock::Duration base, time::ChessClock::Duration increment)
    {
        engine_->time_control_ = std::make_unique<time::IncrementalTimeControl>(base, increment);
        engine_->clock_ = std::make_unique<time::IncrementalChessClock>(static_cast<time::IncrementalTimeControl&>(*engine_->time_control_));
    }

    void ObservingState::SetDepth(DepthType depth)
    {
        engine_->max_depth_ = depth;
    }
}