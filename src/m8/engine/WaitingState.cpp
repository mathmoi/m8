/// @file	WaitingState.cpp
/// @author Mathieu Pag�
/// @date	January 2020
/// @brief	Contains the WaitingState class. Controlling the engine behavior when it's 
///         waiting for the user to play.

#include "../../m8chess/SAN.hpp"

#include "InvalidMoveException.hpp"
#include "ThinkingState.hpp"

#include "WaitingState.hpp"

namespace m8::engine
{
	WaitingState::WaitingState(EngineState* source)
		: EngineState("WaitingState", source)
	{}

	void WaitingState::UserMove(std::string str_move)
	{	
		Move move = ParseMove(str_move);

		this->board().Make(move);

		auto thinking_state = new ThinkingState(this);
		ChangeState(thinking_state);
	}

	Move WaitingState::ParseMove(const std::string& str_move)
	{
		Move move = kNullMove;

		try
		{
			move = ParseSAN(str_move, this->board());
		}
		catch (const InvalidSANMoveException&)
		{
			throw InvalidMoveException("Invalid Move : " + str_move);
		}

		return move;
	}
}

