/// @file	ObservingState.cpp
/// @author Mathieu Pagé
/// @date	Decembre 2019
/// @brief	Contains the WaitingState class. Controlling the engine behavior when it's 
///         waiting for commands.

#include <memory>

#include "ObservingState.hpp"
#include "WaitingState.hpp"
#include "PerftState.hpp"
#include "ThinkingState.hpp"
#include "InvalidMoveException.hpp"

namespace m8::engine {

	void ObservingState::Perft(int depth)
	{
		auto perft_state = new PerftState(this, depth);
		ChangeState(perft_state);
	}

	void ObservingState::UserMove(std::string str_move)
	{
		Move move = ParseMove(str_move);

		this->board().Make(move);
	}

	Move ObservingState::ParseMove(const std::string& str_move)
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

	void ObservingState::New()
	{
		set_fen(kStartingPositionFEN);
		set_engine_color(kBlack);

		auto waiting_state = new WaitingState(this);
		ChangeState(waiting_state);
	}

	void ObservingState::Go()
	{
		set_engine_color(board().side_to_move());

		auto thinking_state = new ThinkingState(this);
		ChangeState(thinking_state);
	}
}