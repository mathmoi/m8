/// @file   SearchResult.hpp
/// @author Mathieu Pagé
/// @date   Novembre 2019
/// @brief  Contains the SearchResult class.

#ifndef M8_SEARCH_RESULT_HPP_
#define M8_SEARCH_RESULT_HPP_

#include "../eval/Eval.hpp"
#include "../Types.hpp"

#include "PV.hpp"

namespace m8::search {

	/// Implements the minimax search algorithm
	struct SearchResult
	{
	public:
		/// Default constuctor
		SearchResult()
			: value_(0),
			  nodes_(0)
		{}

		// Constructor.
		SearchResult(const PV& pv,
			         EvalType value,
			         NodeCounterType nodes)
			: value_(value),
			  pv_(pv),
			  nodes_(nodes)
		{}

		EvalType        value_;
		PV				pv_;
		NodeCounterType nodes_;

	};
}

#endif // M8_SEARCH_RESULT_HPP_