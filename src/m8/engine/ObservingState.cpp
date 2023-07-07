/// @file	ObservingState.cpp
/// @author Mathieu Pagé
/// @date	Decembre 2019
/// @brief	Contains the WaitingState class. Controlling the engine behavior when it's 
///         waiting for commands.

#include <memory>

#include "../options/Options.hpp"
#include "../../m8chess/CoordinateNotation.hpp"

#include "ObservingState.hpp"
#include "WaitingState.hpp"
#include "PerftState.hpp"
#include "ThinkingState.hpp"
#include "InvalidMoveException.hpp"

namespace m8::engine {

	void ObservingState::Perft(int depth)
	{
		auto perft_state = new PerftState(this->engine_, depth);
		engine_->ChangeState(perft_state);
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
		engine_->set_fen(kStartingPositionFEN);
		engine_->engine_color_ = kBlack;

		auto waiting_state = new WaitingState(engine_);
		engine_->ChangeState(waiting_state);
	}

	void ObservingState::Go()
	{
		engine_->engine_color_ = engine_->board_.side_to_move();

		auto thinking_state = new ThinkingState(engine_);
		engine_->ChangeState(thinking_state);
	}

	void ObservingState::SetTimeControl(float seconds_per_move)
	{
		std::chrono::duration<float> fseconds(seconds_per_move);
		auto duration = std::chrono::duration_cast<time::TimePerMoveTimeControl::Duration>(fseconds);

		engine_->time_control_ = std::make_unique<time::TimePerMoveTimeControl>(duration);
	}
}