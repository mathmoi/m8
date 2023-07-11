/// @file   AlphaBeta.hpp
/// @author Mathieu Pagé
/// @date   Decembre 2018
/// @brief  Contains the AlphaBeta class. This class is responsible for the search
///         algorithm of the chess engine.

#include <chrono>

#include "../eval/Eval.hpp"
#include "../MoveGen.hpp"
#include "../Checkmate.hpp"

#include "AlphaBeta.hpp"

namespace m8::search {

	AlphaBeta::AlphaBeta(const Board& board,
						 const time::TimeManager& time_manager)
		: board_(board),
		  continue_(true),
		  best_move_(kNullMove),
		  nodes_counter_(0),
		  nodes_count_next_time_check_(kNodesBeforeFirstCheck),
		  time_manager_(time_manager)
	{}

	template<bool root, bool qsearch>
	EvalType AlphaBeta::Search(EvalType alpha, EvalType beta, DepthType depth, PV& pv)
	{
		PV local_pv;

		pv.Clear();
		++nodes_counter_;

		// We check if we need to abort the search because of time constraint
		if (!qsearch && nodes_count_next_time_check_ <= nodes_counter_)
		{
			continue_ = time_manager_.can_continue();
			if (!continue_)
			{
				return 0;
			}
			nodes_count_next_time_check_ = nodes_counter_ + time_manager_.CalculateNodesBeforeNextCheck(nodes_counter_);
		}

		// If we are in the qsearch we must evaluate the stand path option.
		if (qsearch)
		{
			EvalType stand_path = eval::Evaluate(board_);
			if (stand_path >= beta)
			{
				return beta;
			}
			if (alpha < stand_path)
			{
				alpha = stand_path;
			}
		}

		// Generate all the moves
		MoveList moves; // TODO : MoveList should be a class. Test for performance.
		Move* first = moves.data();
		Move* last = qsearch ? GenerateAllCaptures(board_, moves.data())
			                 : GenerateAllMoves(board_, moves.data());
				
		// Evaluate all moves
		for (Move* next = first; next < last && continue_; ++next)
		{
			EvalType value;

			UnmakeInfo unmake_info = board_.Make(*next);

			// If the position is invalid because the side to move is in check we must 
			// undo the current move and continue with the next one.
			if (IsInvalidCheckPosition(board_))
			{
				board_.Unmake(*next, unmake_info);
			}
			else
			{
				// If we are at depth > 1 we need to make a recursive call to the search
				// function. If we are at depth 1 or in the qsearch we need to make a
				// recursive call to the qsearch function.
				if (!qsearch && depth > 1)
				{
					// Recursive call to the search function
					value = -Search<false, false>(-beta, -alpha, depth - 1, local_pv);
				}
				else
				{
					// Call to the qsearch
					value = -Search<false, true>(-beta, -alpha, 0, local_pv);
				}
				value = eval::AddDepthToMate(value);
				board_.Unmake(*next, unmake_info);

				// If we are aborting the search we need to leave immediately.
				if (!continue_)
				{
					return 0;
				}

				// If the value of the current move is better or equal to beta we can 
				// abort the search at this node.
				if (value >= beta)
				{
					return beta;
				}

				// If value is better than alpha we possibly have a new best move at this
				// node.
				if (value > alpha)
				{
					alpha = value;
					if (!qsearch)
					{
						pv.Replace(*next, local_pv);
					}

					// If it is a new best move we notify the user.
					if (root && *next != best_move_)
					{
						best_move_ = *next;
						NotifyNewBestMove(pv, alpha, depth, 0, nodes_counter_);
					}
				}
			}
		}

		return alpha;
	}

	std::optional<SearchResult> AlphaBeta::Search(DepthType depth)
	{
		PV pv;

		NotifySearchStarted();

		auto value = Search<true, false>(eval::kMinEval, eval::kMaxEval, depth, pv);
		
		// If we are aborting the search we need to leave immediately.
		if (!continue_) // TODO : Find a way to return the nodes_count separately from the pv/value
		{
			// TODO : If we make sure that we always search the previous best move first, we could still use the result of a partial search.
			return std::nullopt;
		}

		auto result = SearchResult(pv, value, nodes_counter_);

		NotifySearchCompleted(pv, 0);

		return result;
	}

	void AlphaBeta::Stop()
	{
		continue_ = false;
	}

}