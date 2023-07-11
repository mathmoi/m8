/// @file	WaitingState.cpp
/// @author Mathieu Pagé
/// @date	January 2020
/// @brief	Contains the WaitingState class. Controlling the engine behavior when it's 
///         waiting for the user to play.

#include "../../m8common/logging.hpp"

#include "../options/Options.hpp"
#include "../../m8chess/CoordinateNotation.hpp"
#include "../../m8chess/SAN.hpp"
#include "../../m8chess/Checkmate.hpp"
#include "../../m8chess/time/ConventionalTimeControl.hpp"
#include "../../m8chess/time/IncrementalTimeControl.hpp"
#include "../../m8chess/time/TimePerMoveChessClock.hpp"
#include "../../m8chess/time/ConventionalChessClock.hpp"
#include "../../m8chess/time/IncrementalChessClock.hpp"

#include "InvalidMoveException.hpp"
#include "SearchState.hpp"
#include "ObservingState.hpp"

#include "WaitingState.hpp"

namespace m8::engine
{
    WaitingState::WaitingState(Engine* engine)
        : EngineState(engine)
    {}

    void WaitingState::UserMove(std::string str_move)
    {	
        M8_TRACE <<"WaitingState::UserMove(\"" <<str_move <<"\")";
        
        Move move = ParseMove(str_move);

        engine_->board_.Make(move);

        std::unique_ptr<EngineState> next_state = IsMat(engine_->board_)
                                                ? static_cast<std::unique_ptr<EngineState>>(std::make_unique<ObservingState>(engine_))
                                                : static_cast<std::unique_ptr<EngineState>>(std::make_unique<SearchState>(engine_));
        engine_->ChangeState(std::move(next_state));
    }

    Move WaitingState::ParseMove(const std::string& str_move)
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

    void WaitingState::New()
    {
        set_fen(kStartingPositionFEN);
        engine_->engine_color_ = kBlack;
    }

    void WaitingState::Go()
    {
        engine_->engine_color_ = engine_->board_.side_to_move();

        auto thinking_state = std::make_unique<SearchState>(engine_);
        engine_->ChangeState(std::move(thinking_state));
    }

    void WaitingState::Force()
    {
        auto observing_state = std::make_unique<ObservingState>(engine_);
        engine_->ChangeState(std::move(observing_state));
    }

    void WaitingState::SetTimeControl(time::ChessClock::Duration time_per_move)
    {
        engine_->time_control_ = std::make_unique<time::TimePerMoveTimeControl>(time_per_move);
        engine_->clock_ = std::make_unique<time::TimePerMoveChessClock>(static_cast<time::TimePerMoveTimeControl&>(*engine_->time_control_));
    }

    void WaitingState::SetTimeControl(std::uint32_t moves, time::ChessClock::Duration time)
    {
        engine_->time_control_ = std::make_unique<time::ConventionalTimeControl>(moves, time);
        engine_->clock_ = std::make_unique<time::ConventionalChessClock>(static_cast<time::ConventionalTimeControl&>(*engine_->time_control_));
    }

    void WaitingState::SetTimeControl(time::ChessClock::Duration base, time::ChessClock::Duration increment)
    {
        engine_->time_control_ = std::make_unique<time::IncrementalTimeControl>(base, increment);
        engine_->clock_ = std::make_unique<time::IncrementalChessClock>(static_cast<time::IncrementalTimeControl&>(*engine_->time_control_));
    }
}

