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

#include "InvalidMoveException.hpp"
#include "ThinkingState.hpp"
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

		EngineState* next_state = IsMat(engine_->board_)
		                        ? static_cast<EngineState*>(new ObservingState(engine_))
								: static_cast<EngineState*>(new ThinkingState(engine_));
		engine_->ChangeState(next_state);
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

		auto thinking_state = new ThinkingState(engine_);
		engine_->ChangeState(thinking_state);
	}

	void WaitingState::Force()
	{
		auto observing_state = new ObservingState(engine_);
		engine_->ChangeState(observing_state);
	}

	void WaitingState::SetTimeControl(float seconds_per_move)
	{
		std::chrono::duration<float> fseconds(seconds_per_move);
		auto duration = std::chrono::duration_cast<time::TimePerMoveTimeControl::Duration>(fseconds);

		engine_->time_control_ = std::make_unique<time::TimePerMoveTimeControl>(duration);
	}
}

