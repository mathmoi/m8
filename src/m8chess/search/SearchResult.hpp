/// @file   SearchResult.hpp
/// @author Mathieu Pagé
/// @date   Novembre 2019
/// @brief  Contains the SearchResult class.

#ifndef M8_SEARCH_RESULT_HPP_
#define M8_SEARCH_RESULT_HPP_

#include "../eval/Eval.hpp"
#include "../Move.hpp"
#include "../Types.hpp"

namespace m8::search {

	/// Implements the minimax search algorithm
	struct SearchResult
	{
	public:

		// Constructor.
		SearchResult(EvalType value = 0,
			         Move best_move = kNullMove,
					 NodeCounterType nodes = 0)
			: value_(value),
			  best_move_(best_move),
			  nodes_(nodes)
		{}

		EvalType        value_;
		Move            best_move_;
		NodeCounterType nodes_;

	};
}

#endif // M8_SEARCH_RESULT_HPP_