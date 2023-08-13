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

    inline Move* IterativeDeepening::GetRootMoves(Board board, Move* first)
    {
        movegen::MoveGenerator<false, false> generator(board);
        Move* last = first;
        for (Move move : generator)
        {
            *(last++) = move;
        }
        return last;
    }

    SearchResult IterativeDeepening::Start(std::shared_ptr<Search> search)
    {
        // Generate the root moves
        MoveList root_moves;
        Move* first_root_move = root_moves.data();
        Move* last_root_move = GetRootMoves(search->board(), first_root_move);

        AlphaBeta alpha_beta(search, transposition_table_, first_root_move, last_root_move);
        alpha_beta.Attach(this);

        std::optional<SearchResult> result;
        std::optional<SearchResult> last_result;

        NotifySearchStarted();

        DepthType current_depth = 1;
        while(current_depth <= search->max_depth() &&
              search->time_manager().can_start_new_iteration())
        {
            NotifyIterationStarted();
            result = alpha_beta.Start(current_depth);
            if (result.has_value())
            {
                NotifyIterationCompleted(result.value().pv_,
                                         result.value().value_,
                                         current_depth,
                                         0,
                                         result.value().stats_.nodes + result.value().stats_.qnodes);
                last_result = result;

                // If the best move found at the root is not at the top of the root moves
                // list we pull it to the front. This way the best move from a previous
                // iteration is always searched first. This has two benifits. First we can
                // use the result of a partial search. If a new good move is found it can
                // be used even if all moved have not beeen searched. Second it reduce the
                // number a new best move is found at the root.
                auto best_move = result.value().pv_.first();
                if (*first_root_move != best_move)
                {
                    auto pos_best = std::find(first_root_move, last_root_move, best_move);
                    std::rotate(first_root_move, pos_best, pos_best + 1);
                }
            }

            ++current_depth;
        }

        NotifySearchCompleted(last_result.value().pv_, 0, last_result.value().stats_);

        return last_result.value();
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