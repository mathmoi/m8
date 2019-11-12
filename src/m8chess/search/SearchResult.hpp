/// @file   SearchResult.hpp
/// @author Mathieu Pagé
/// @date   Novembre 2019
/// @brief  Contains the SearchResult class.

#ifndef M8_SEARCH_RESULT_HPP_
#define M8_SEARCH_RESULT_HPP_

#include "../eval/Eval.hpp"
#include "../Move.hpp"

namespace m8 { namespace search {

	/// Implements the minimax search algorithm
	struct SearchResult
	{
	public:

		/// Constructor
		inline SearchResult(eval::EvalType value, Move best_move)
			: value_(value),
			  best_move_(best_move)
		{};

		/// Returns the value.
		eval::EvalType GetValue() const { return value_; }

		/// Returns the best move
		Move GetBestMove() const { return best_move_; }

	private:
		eval::EvalType value_;
		Move           best_move_;

	};

}}

#endif // M8_SEARCH_RESULT_HPP_