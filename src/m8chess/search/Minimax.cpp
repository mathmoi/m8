/// @file   Minimax.cpp
/// @author Mathieu Pagé
/// @date   November 2019
/// @brief  Contains the Minimax class. This class implements the naive minimax algorithm.

#include "Minimax.hpp"
#include "../MoveGen.hpp"
#include "../Checkmate.hpp"

namespace m8 { namespace search
{
	// TODO : Detect Mate and nulls
	SearchResult Minimax::Search(std::uint32_t depth)
	{
		MoveGen move_gen(board_);
		MoveList moves;
		Move* last = move_gen.GenerateAllMoves(moves.data());

		eval::EvalType max_value = eval::kMinEval;
		Move best_move = kNullMove;

		for (Move* next = moves.data(); next < last; ++next)
		{
			UnmakeInfo unmake_info = board_.Make(*next);

			eval::EvalType value;

			if (!IsInvalidCheckPosition(board_, move_gen))
			{
				if (depth > 1)
				{
					value = -SearchInternal(depth - 1);
					value = eval::AddDepthToMate(value);
				}
				else
				{
					value = eval::Evaluate(board_);
				}

				if (value > max_value)
				{
					max_value = value;
					best_move = *next;
				}
			}

			board_.Unmake(*next, unmake_info);
		}

		return SearchResult(max_value, best_move);
	}

	eval::EvalType Minimax::SearchInternal(std::uint32_t depth)
	{
		MoveGen move_gen(board_);
		MoveList moves;
		Move* last = move_gen.GenerateAllMoves(moves.data());

		eval::EvalType max_value = eval::kMinEval;

		for (Move* next = moves.data(); next < last; ++next)
		{
			UnmakeInfo unmake_info = board_.Make(*next);

			eval::EvalType value;

			if (!IsInvalidCheckPosition(board_, move_gen))
			{
				if (depth > 1)
				{
					value = -SearchInternal(depth - 1);
					value = eval::AddDepthToMate(value);
				}
				else
				{
					value = eval::Evaluate(board_);
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
			max_value = IsInCheck(board_.side_to_move(), board_, move_gen)
				? -eval::kEvalMat
				: eval::kEvalNull;
		}

		return max_value;
	}
}}