/// @file   AlphaBeta.hpp
/// @author Mathieu Pagé
/// @date   Decembre 2018
/// @brief  Contains the AlphaBeta class. This class is responsible for the search
///         algorithm of the chess engine.

#include <chrono>

#include "../eval/Eval.hpp"

#include "../movegen/MoveGenerator.hpp"

#include "../Checkmate.hpp"

#include "AlphaBeta.hpp"
#include "Search.hpp"

namespace m8::search {

    AlphaBeta::AlphaBeta(std::shared_ptr<Search> search,
                         transposition::TranspositionTable& transposition_table,
                         Move* first_root_move,
                         Move* last_root_move)
        : board_(search->board()),
          first_root_move_(first_root_move),
          last_root_move_(last_root_move),
          continue_(true),
          nodes_count_next_time_check_(kNodesBeforeFirstCheck),
          search_(search),
          transposition_table_(transposition_table)
    {}

    template<bool root, bool qsearch>
    EvalType AlphaBeta::AlphaBetaSearch(EvalType alpha, EvalType beta, DepthType depth, DepthType distance, PV& pv)
    {
        PV local_pv;

        pv.Clear();

        qsearch ? stats_.qnodes++ : stats_.nodes++;

        // We check if we need to abort the search because of time constraint
        if (nodes_count_next_time_check_ <= stats_.all_nodes())
        {
            continue_ = !search_->is_aborted() && search_->time_manager().can_continue();
            if (!continue_)
            {
                return 0;
            }
            nodes_count_next_time_check_ = stats_.all_nodes() + search_->time_manager().CalculateNodesBeforeNextCheck(stats_.all_nodes());
        }

        // In internal search nodes (not root and not qsearch) we probe the
        // transposition table. If we find an acceptable exact score or a lower 
        // bound better than beta we might cut the search imediately. If we find
        // a lower bound better than alpha but not better than beta we can immediately raise alpha
        if (!qsearch && !root)
        {
            auto tt_entry = transposition_table_[board_.hash()];
            ++stats_.tt_probes;
            if (tt_entry != nullptr)
            {
                ++stats_.tt_hits;
                if (depth <= tt_entry->depth())
                {
                    auto tt_eval = tt_entry->GetEval(distance);
                    if (tt_entry->type() == transposition::EntryType::Exact)
                    {
                        ++stats_.tt_hits_exact;
                        return tt_eval;
                    }

                    if (tt_entry->type() == transposition::EntryType::LowerBound
                        && tt_eval >= beta)
                    {
                        ++stats_.tt_hits_lower;
                        return beta;
                    }

                    if (tt_entry->type() == transposition::EntryType::UpperBound
                        && tt_eval <= alpha)
                    {
                        ++stats_.tt_hits_upper;
                        return alpha;
                    }
                }
            }
        }

        // If we are in the qsearch we must evaluate the stand path option.
        auto original_alpha = alpha;
        if (qsearch)
        {
            EvalType stand_path = eval::Evaluate(board_);
            if (stand_path >= beta)
            {
                return beta;
            }
            if (alpha < stand_path)
            {
                alpha = stand_path;
            }
        }

        // Evaluate all moves
        movegen::MoveGenerator generator = root ? movegen::MoveGenerator<root, qsearch>(first_root_move_, last_root_move_)
                                                : movegen::MoveGenerator<root, qsearch>(board_);
        bool found_a_move = false;
        std::uint16_t move_count = 0;
        for (auto move : generator)
        {
            ++move_count;

            if (root)
            {
                NotifySearchMoveAtRoot(depth, 0, move_count, last_root_move_ - first_root_move_, stats_.nodes + stats_.qnodes, move);
            }

            UnmakeInfo unmake_info = board_.Make(move);

            // If the position is invalid because the side to move is in check we must 
            // undo the current move and continue with the next one.
            if (IsInvalidCheckPosition(board_))
            {
                board_.Unmake(move, unmake_info);
            }
            else
            {
                found_a_move = true;
                EvalType value;

                // If we are at depth > 1 we need to make a recursive call to the search
                // function. If we are at depth 1 or in the qsearch we need to make a
                // recursive call to the qsearch function.
                if (!qsearch && depth > 1)
                {
                    // Recursive call to the search function
                    value = -AlphaBetaSearch<false, false>(-beta, -alpha, depth - 1, distance + 1, local_pv);
                }
                else
                {
                    // Call to the qsearch
                    value = -AlphaBetaSearch<false, true>(-beta, -alpha, 0, distance + 1, local_pv);
                }
                
                board_.Unmake(move, unmake_info);

                // If we are aborting the search we need to leave immediately.
                if (!continue_)
                {
                    return 0;
                }

                // If the value of the current move is better or equal to beta we can 
                // abort the search at this node.
                if (value >= beta)
                {
                    if (!qsearch)
                    {
                        transposition_table_.Insert(board_.hash(),
                                                    move,
                                                    transposition::EntryType::LowerBound,
                                                    depth,
                                                    distance,
                                                    value);
                    }
                    return beta;
                }

                // If value is better than alpha we possibly have a new best move at this
                // node.
                if (value > alpha)
                {
                    alpha = value;
                    if (!qsearch)
                    {
                        pv.Replace(move, local_pv);
                    }

                    // If it is a new best move we notify the user.
                    if (root)
                    {
                        NotifyNewBestMove(pv, alpha, depth, 0, stats_.nodes + stats_.qnodes);
                    }
                }
            }
        }

        // If we have not found any legal move during the search we are either checkmate 
        // or stalemate. We need to return a value accordingly.
        if (!qsearch && !found_a_move)
        {
            if (IsInCheck(board_.side_to_move(), board_))
            {
                assert(IsMat(board_));
                return eval::GetMateValue(distance);
            }

            return eval::kEvalDraw;
        }

        // If we are not in the qsearch we store the current evaluation in the 
        // transposition table. At this point it can be an upper bound (we did not improve 
        // alpha) or an exact score.
        if (!qsearch)
        {
            auto type_tt_entry = (alpha == original_alpha ? transposition::EntryType::UpperBound
                                                          : transposition::EntryType::Exact);
            transposition_table_.Insert(board_.hash(),
                                        (pv.any() ? pv.first() : kNullMove),
                                        type_tt_entry,
                                        depth,
                                        distance,
                                        alpha);
        }
        return alpha;
    }

    std::optional<SearchResult> AlphaBeta::Start(DepthType depth)
    {
        PV pv;

        NotifySearchStarted();

        auto value = AlphaBetaSearch<true, false>(eval::kMinEval, eval::kMaxEval, depth, 0, pv);
        
        // If we are aborting the search we need to leave immediately.
        if (!continue_)
        {
            // TODO : If we make sure that we always search the previous best move first, we could still use the result of a partial search.
            return std::nullopt;
        }

        auto result = SearchResult(pv, value, stats_);

        NotifySearchCompleted(pv, 0, stats_);

        return result;
    }

}