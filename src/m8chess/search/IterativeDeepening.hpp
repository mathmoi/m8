/// @file   IterativeDeepening.hpp
/// @author Mathieu Pagé
/// @date   Janvier 2020
/// @brief  Contains the Iterative Deepening algorithm.

#ifndef M8_SEARCH_ITERATIVE_DEEPENING_HPP_
#define M8_SEARCH_ITERATIVE_DEEPENING_HPP_

#include "../Board.hpp"
#include "../Types.hpp"
#include "SearchResult.hpp"
#include "SearchObserver.hpp"
#include "AlphaBeta.hpp"

namespace m8::search {

	class IterativeDeepening : public SearchObserver
	{
	public:
		/// Constructor.
		IterativeDeepening(const Board& board,
			               SearchObserver* observer);

		// TODO : Remove depth from the Search method and replace by proper time management
		/// Start a search on a given position.
		SearchResult Search(DepthType depth);

		/// Method called when a new best move is found at the root.
		void OnNewBestMove(const PV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes);

		/// Stop the search
		void Stop();

	private:
		AlphaBeta alpha_beta_;

		SearchObserver* observer_;
	};
}

#endif // M8_SEARCH_ITERATIVE_DEEPENING_HPP_
