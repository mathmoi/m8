/// @file   Minimax.cpp
/// @author Mathieu Pagé
/// @date   November 2019
/// @brief  Contains the Minimax class. This class implements the naive minimax algorithm.

#include "Minimax.hpp"
#include "../MoveGen.hpp"
#include "../Checkmate.hpp"

namespace m8 { namespace search
{
	SearchResult Minimax::Search(std::uint32_t depth)
	{
		continue_ = true;

		MoveList moves;
		Move* last = GenerateAllMoves(board_, moves.data());

		eval::EvalType max_value = eval::kMinEval;
		Move best_move = kNullMove;

		for (Move* next = moves.data(); next < last && continue_; ++next)
		{
			UnmakeInfo unmake_info = board_.Make(*next);

			eval::EvalType value;

			if (!IsInvalidCheckPosition(board_))
			{
				if (depth > 1)
				{
					value = -SearchInternal(depth - 1);
					value = eval::AddDepthToMate(value);
				}
				else
				{
					value = -eval::Evaluate(board_);
				}

				if (value > max_value)
				{
					max_value = value;
					best_move = *next;
				}
			}

			board_.Unmake(*next, unmake_info);
		}	

		if (continue_)
		{
			return SearchResult(max_value, best_move);
		}
		else
		{
			return SearchResult(0, kNullMove);
		} 
	}

	eval::EvalType Minimax::SearchInternal(std::uint32_t depth)
	{
		MoveList moves;
		Move* last = GenerateAllMoves(board_, moves.data());

		eval::EvalType max_value = eval::kMinEval;

		for (Move* next = moves.data(); next < last && continue_; ++next)
		{
			UnmakeInfo unmake_info = board_.Make(*next);

			eval::EvalType value;

			if (!IsInvalidCheckPosition(board_))
			{
				if (depth > 1)
				{
					value = -SearchInternal(depth - 1);
					value = eval::AddDepthToMate(value);
				}
				else
				{
					value = -eval::Evaluate(board_);
				}

				if (value > max_value)
				{
					max_value = value;
				}
			}

			board_.Unmake(*next, unmake_info);
		}

		// detect mate and stalemate and returns appropriate evaluations
		if (max_value == eval::kMinEval)
		{
			max_value = IsInCheck(board_.side_to_move(), board_)
				? -eval::kEvalMat
				: eval::kEvalNull;
		}

		return max_value;
	}

	void Minimax::Stop()
	{
		continue_ = false;
	}
}}