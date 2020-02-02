/// @file   IterativeDeepening.cpp
/// @author Mathieu Pagé
/// @date   Janvier 2020
/// @brief  Contains the Iterative Deepening algorithm.

#include "IterativeDeepening.hpp"

namespace m8::search {

	IterativeDeepening::IterativeDeepening(const Board& board,
		                                   SearchObserver* observer)
		: alpha_beta_(board,
			          this),
		  observer_(observer)
	{}

	SearchResult IterativeDeepening::Search(DepthType depth)
	{
		SearchResult result;

		observer_->OnBeginSearch();

		for (DepthType current_depth = 1; current_depth <= depth; ++current_depth)
		{
			result = alpha_beta_.Search(current_depth);
			observer_->OnIterationCompleted(result.pv_, result.value_, current_depth, 0, result.nodes_);
		}

		observer_->OnSearchCompleted(result.pv_, 0);

		return result;
	}

	void IterativeDeepening::Stop()
	{
		alpha_beta_.Stop();
	}

	void IterativeDeepening::OnNewBestMove(const PV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes)
	{
		observer_->OnNewBestMove(pv, eval, depth, time, nodes);
	}
}