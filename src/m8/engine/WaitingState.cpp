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
	WaitingState::WaitingState(EngineState* source)
		: EngineState("WaitingState", source)
	{}

	void WaitingState::UserMove(std::string str_move)
	{	
		M8_TRACE <<"WaitingState::UserMove(\"" <<str_move <<"\")";
		
		Move move = ParseMove(str_move);

		board().Make(move);

		EngineState* next_state = IsMat(board())
		                        ? static_cast<EngineState*>(new ObservingState(this))
								: static_cast<EngineState*>(new ThinkingState(this));
		ChangeState(next_state);
	}

	Move WaitingState::ParseMove(const std::string& str_move)
	{
		Move move = kNullMove;

		try
		{
			move = options::Options::get().use_san ? ParseSAN(str_move, this->board()) : ParseCoordinateNotation(str_move, this->board());
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
		set_engine_color(kBlack);
	}

	void WaitingState::Go()
	{
		set_engine_color(board().side_to_move());

		auto thinking_state = new ThinkingState(this);
		ChangeState(thinking_state);
	}

	void WaitingState::Force()
	{
		auto observing_state = new ObservingState(this);
		ChangeState(observing_state);
	}
}

