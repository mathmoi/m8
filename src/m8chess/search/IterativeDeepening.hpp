/// @file   IterativeDeepening.hpp
/// @author Mathieu Pagé
/// @date   Janvier 2020
/// @brief  Contains the Iterative Deepening algorithm.

#ifndef M8_CHESS_SEARCH_ITERATIVE_DEEPENING_HPP_
#define M8_CHESS_SEARCH_ITERATIVE_DEEPENING_HPP_

#include "../time/TimeManager.hpp"

#include "../transposition/TranspositionTable.hpp"

#include "../Board.hpp"
#include "../Types.hpp"

#include "AlphaBeta.hpp"
#include "Search.hpp"
#include "SearchResult.hpp"
#include "SearchSubject.hpp"

namespace m8::search {

    class IterativeDeepening : public SearchSubject<PV, Move>, public ISearchObserver<PV, Move>
    {
    public:
        /// Constructor.
        IterativeDeepening(transposition::TranspositionTable& transposition_table);

        /// Start a search on a given position.
        SearchResult Start(std::shared_ptr<Search> search);

        /// Method called when a new moved is searched at the root.
        void OnSearchMoveAtRoot(DepthType depth, double time, std::uint16_t move_number, std::uint16_t moves_number, NodeCounterType nodes, Move move);

        /// Method called when a new best move is found at the root.
        void OnNewBestMove(const PV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes);
    
    private:
        transposition::TranspositionTable& transposition_table_;

        void GetRootMoves(Board board, MoveList& move_list);
    };
}

#endif // M8_CHESS_SEARCH_ITERATIVE_DEEPENING_HPP_
