/// @file   PerftState.cpp
/// @author Mathieu Pagé
/// @date   Decembre 2019
/// @brief  Contains the PerftState class. Controlling the engine behavior when it's 
///         executing a perft test.

#include "PerftState.hpp"
#include "ObservingState.hpp"

namespace m8::engine {

    void PerftState::BeginState()
    {
        perft_.Run();
    }

    void PerftState::OnPartialPerftResult(const std::string& move, std::uint64_t count)
    {
        observer_->OnPartialPerftResult(move, count);
    }

    void PerftState::OnPerftCompleted(std::uint64_t count, double time)
    {
        observer_->OnPerftCompleted(count, time);
        
        auto waiting_state = std::make_unique<ObservingState>(this->engine_);
        engine_->ChangeState(std::move(waiting_state));
    }

    void PerftState::Stop()
    {
        auto waiting_state = std::make_unique<ObservingState>(this->engine_);
        engine_->ChangeState(std::move(waiting_state));
    }

}