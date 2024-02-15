/// @file   Engine.cpp
/// @author Mathieu Pagé
/// @date   November 2017
/// @brief  Contains the Engine class that encapsulate all m8 functionalities.

#include <iostream>

#include "m8chess/SAN.hpp"

#include "m8common/logging.hpp"

#include "m8common/options/Options.hpp"

#include "Engine.hpp"
#include "EngineState.hpp"
#include "ObservingState.hpp"

namespace m8::engine
{
    Engine::Engine(eval::PieceSqTablePtr psqt)
    : psqt_(psqt),
      board_(kStartingPositionFEN, psqt),
      engine_color_(kBlack),
      max_depth_(kMaxMaxDepth),
      time_control_(std::make_unique<time::TimePerMoveTimeControl>(std::chrono::seconds(1))),
      clock_(time::ChessClock::CreateChessClock(*time_control_)),
      transposition_table_(options::Options::get().tt_size*1024*1024),
      searcher_(transposition_table_)
    {
        state_ = std::make_unique<ObservingState>(this);
    }

    Engine::~Engine() = default;

    void Engine::ChangeState(std::unique_ptr<EngineState> new_state)
    {
        if (new_state != state_)
        {
            M8_DEBUG << "ChangeState from " << state_->state_name() << " to " << new_state->state_name();

            state_->EndState();

            state_ = std::move(new_state);

            state_->BeginState();
        }
        else
        {
            M8_DEBUG << "Same state (" << state_->state_name() <<"), no need to change.";
        }
    }

    EvalType Engine::current_evaluation() const
    {
        return eval::Evaluate(board_);
    }

    void Engine::SetBoard(std::string fen) { state_->SetBoard(fen); }

    void Engine::SetTimeControl(time::ChessClock::Duration time_per_move) { state_->SetTimeControl(time_per_move); }

    void Engine::SetTimeControl(std::uint32_t moves, time::ChessClock::Duration time) { state_->SetTimeControl(moves, time); }

    void Engine::SetTimeControl(time::ChessClock::Duration base, time::ChessClock::Duration increment) { state_->SetTimeControl(base, increment); }

    void Engine::SetDepth(DepthType depth) { state_->SetDepth(depth); }

    void Engine::Perft(int depth, IPerftObserver* observer) { return state_->Perft(depth, observer); }

    void Engine::Go() { state_->Go(); }

    void Engine::Force() { state_->Force(); }

    void Engine::Stop() { state_->Stop(); }

    void Engine::New() { state_->New(); }

    void Engine::UserMove(std::string move) { state_->UserMove(move); }
}