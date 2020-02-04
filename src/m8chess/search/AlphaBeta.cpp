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
		                 SearchObserver* observer)
		: board_(board),
		  continue_(true),
		  best_move_(kNullMove),
		  nodes_counter_(0),
		  observer_(observer)
	{}

	template<bool root, bool qsearch>
	EvalType AlphaBeta::Search(EvalType alpha, EvalType beta, DepthType depth, PV& pv)
	{
		PV local_pv;

		pv.Clear();
		++nodes_counter_;

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
		MoveList moves;
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
					pv.Replace(*next, local_pv);

					// If it is a new best move we notify the user.
					if (root && *next != best_move_)
					{
						best_move_ = *next;
						observer_->OnNewBestMove(pv, alpha, depth, 0, nodes_counter_);
					}
				}
			}
		}

		return alpha;
	}

	SearchResult AlphaBeta::Search(DepthType depth)
	{
		PV pv;

		observer_->OnBeginSearch();

		auto value = Search<true, false>(eval::kMinEval, eval::kMaxEval, depth, pv);
		auto result = SearchResult(pv, value, nodes_counter_);

		observer_->OnSearchCompleted(pv, 0);

		return result;
	}

	void AlphaBeta::Stop()
	{
		continue_ = false;
	}

}