/// @file	Engine.cpp
/// @author Mathieu Pagé
/// @date	November 2017
/// @brief	Contains the Engine class that encapsulate all m8 functionalities.

#include <iostream>

#include "../../m8common/logging.hpp"

#include "Engine.hpp"
#include "EngineState.hpp"
#include "ObservingState.hpp"

#include "../../m8chess/SAN.hpp"

namespace m8::engine
{
    Engine::Engine(eval::PieceSqTablePtr psqt,
                   EngineCallbacks callbacks)
    : callbacks_(callbacks),
      psqt_(psqt),
      board_(kStartingPositionFEN, psqt),
      engine_color_(kBlack),
      time_control_(std::make_unique<time::TimePerMoveTimeControl>(std::chrono::seconds(1))),
      clock_(time::ChessClock::CreateChessClock(*time_control_))
    {
        state_ = new ObservingState(this);
    }

    Engine::~Engine()
    {
        delete state_;
    }

    void Engine::ChangeState(EngineState* new_state)
    {
        if (new_state != state_)
        {
            M8_DEBUG << "ChangeState from " << state_->state_name() << " to " << new_state->state_name();

            state_->EndState();

            delete state_; // TODO : Smart Pointer
            state_ = new_state;

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

    void Engine::set_fen(std::string fen) { state_->set_fen(fen); }

    void Engine::SetTimeControl(float seconds_per_move) { state_->SetTimeControl(seconds_per_move); }

    void Engine::Perft(int depth) { return state_->Perft(depth); }

    void Engine::Go() { state_->Go(); }

    void Engine::Force() { state_->Force(); }

    void Engine::Stop() { state_->Stop(); }

    void Engine::New() { state_->New(); }

    void Engine::UserMove(std::string move) { state_->UserMove(move); }
}