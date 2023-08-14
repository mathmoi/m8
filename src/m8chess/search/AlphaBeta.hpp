/// @file   AlphaBeta.hpp
/// @author Mathieu Pagé
/// @date   Decembre 2018
/// @brief  Contains the AlphaBeta class. This class is responsible for the search
///         algorithm of the chess engine.

#ifndef M8_ALPHA_BETA_HPP_
#define M8_ALPHA_BETA_HPP_

#include <vector>
#include <chrono>

#include "../../m8chess/time/TimeManager.hpp"

#include "../transposition/TranspositionTable.hpp"

#include "../Board.hpp"
#include "../MoveList.hpp"
#include "../Types.hpp"

#include "PV.hpp"
#include "Search.hpp"
#include "SearchResult.hpp"
#include "SearchStats.hpp"
#include "SearchSubject.hpp"

namespace m8 {
    namespace search
    {
        /// Manage the seach for the engine.
        class AlphaBeta : public SearchSubject<PV, Move>
        {
        public:
            /// Constructor.
            AlphaBeta(std::shared_ptr<Search> search,
                      transposition::TranspositionTable& transposition_table,
                      const MoveList& root_moves);

            /// Start a search on a given position.
            SearchResult Start(DepthType depth);

        private:
            const NodeCounterType kNodesBeforeFirstCheck = 100000;

            Board board_;
            const MoveList& root_moves_;
            bool continue_;
            SearchStats stats_;
            NodeCounterType nodes_count_next_time_check_;
            std::shared_ptr<Search> search_;
            transposition::TranspositionTable& transposition_table_;

            template<bool root, bool qsearch>
            EvalType AlphaBetaSearch(EvalType alpha, EvalType beta, DepthType depth, DepthType distance, PV& pv);
        };

    }
}

#endif // M8_ALPHA_BETA_HPP_