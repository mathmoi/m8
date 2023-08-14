/// @file   IterativeDeepening.cpp
/// @author Mathieu Pagé
/// @date   Janvier 2020
/// @brief  Contains the Iterative Deepening algorithm.

#include <algorithm>

#include "../movegen/MoveGenerator.hpp"

#include "IterativeDeepening.hpp"

namespace m8::search {

    IterativeDeepening::IterativeDeepening(transposition::TranspositionTable& transposition_table)
    : transposition_table_(transposition_table)
    {}

    inline void IterativeDeepening::GetRootMoves(Board board, MoveList& move_list)
    {
        movegen::MoveGenerator<false, false> generator(board);
        for (Move move : generator)
        {
            move_list.Push(move);
        }
    }

    SearchResult IterativeDeepening::Start(std::shared_ptr<Search> search)
    {
        // Generate the root moves
        MoveList root_moves;
        GetRootMoves(search->board(), root_moves);

        AlphaBeta alpha_beta(search, transposition_table_, root_moves);
        alpha_beta.Attach(this);

        SearchResult last_result;

        NotifySearchStarted();

        DepthType current_depth = 1;
        while(current_depth <= search->max_depth() &&
              search->time_manager().can_start_new_iteration())
        {
            NotifyIterationStarted();
            SearchResult result = alpha_beta.Start(current_depth);
            if (result.type_ == ResultType::Complete)
            {
                NotifyIterationCompleted(result.pv_,
                                         result.value_,
                                         current_depth,
                                         0,
                                         result.stats_.nodes + result.stats_.qnodes);

                // We bring the best move at the front of the list, this way the best move
                // from a previous iteration is always searched first. This has two 
                // benifits. First we can use the result of a partial search. If a new
                // good move is found it can be used even if all moved have not beeen
                // searched. Second it reduce the number a new best move is found at the
                // root.
                auto best_move = result.pv_.first();
                root_moves.PullFront(best_move);
            }
            
            last_result += result;

            ++current_depth;
        }

        NotifySearchCompleted(last_result.pv_, 0, last_result.stats_);

        return last_result;
    }

    void IterativeDeepening::OnSearchMoveAtRoot(DepthType depth, double time, std::uint16_t move_number, std::uint16_t moves_number, NodeCounterType nodes, Move move)
    {
        NotifySearchMoveAtRoot(depth, time, move_number, moves_number, nodes, move);
    }

    void IterativeDeepening::OnNewBestMove(const PV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes)
    {
        NotifyNewBestMove(pv, eval, depth, time, nodes);
    }
}