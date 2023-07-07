/// @file   IterativeDeepening.hpp
/// @author Mathieu Pagé
/// @date   Janvier 2020
/// @brief  Contains the Iterative Deepening algorithm.

#ifndef M8_CHESS_SEARCH_ITERATIVE_DEEPENING_HPP_
#define M8_CHESS_SEARCH_ITERATIVE_DEEPENING_HPP_

#include "../Board.hpp"
#include "../Types.hpp"
#include "../time/TimeManager.hpp"
#include "SearchResult.hpp"
#include "SearchSubject.hpp"
#include "AlphaBeta.hpp"

namespace m8::search {

	class IterativeDeepening : public SearchSubject<PV>, public ISearchObserver<PV>
	{
	public:
		/// Constructor.
		IterativeDeepening(const Board& board,
		                   std::shared_ptr<time::TimeManager> time_manager);

		// TODO : Remove depth from the Search method and replace by proper time management
		/// Start a search on a given position.
		SearchResult Search(DepthType depth);

		/// Method called when a new best move is found at the root.
		void OnNewBestMove(const PV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes);

		/// Stop the search
		void Stop();

	private:
		const DepthType kMinimumSearchDepth = 3;

		AlphaBeta                          alpha_beta_;
		std::shared_ptr<time::TimeManager> time_manager_;
	};
}

#endif // M8_CHESS_SEARCH_ITERATIVE_DEEPENING_HPP_
