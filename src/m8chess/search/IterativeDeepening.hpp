/// @file   IterativeDeepening.hpp
/// @author Mathieu Pagé
/// @date   Janvier 2020
/// @brief  Contains the Iterative Deepening algorithm.

#ifndef M8_CHESS_SEARCH_ITERATIVE_DEEPENING_HPP_
#define M8_CHESS_SEARCH_ITERATIVE_DEEPENING_HPP_

#include "../time/TimeManager.hpp"

#include "../Board.hpp"
#include "../Types.hpp"

#include "AlphaBeta.hpp"
#include "Search.hpp"
#include "SearchResult.hpp"
#include "SearchSubject.hpp"

namespace m8::search {

	class IterativeDeepening : public SearchSubject<PV>, public ISearchObserver<PV>
	{
	public:
		/// Constructor.
		IterativeDeepening();

		/// Start a search on a given position.
		SearchResult Start(std::shared_ptr<Search> search);

		/// Method called when a new best move is found at the root.
		void OnNewBestMove(const PV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes);

	private:
		const DepthType kMinimumSearchDepth = 3;
	};
}

#endif // M8_CHESS_SEARCH_ITERATIVE_DEEPENING_HPP_
