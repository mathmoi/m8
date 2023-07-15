/// @file   IterativeDeepening.cpp
/// @author Mathieu Pagé
/// @date   Janvier 2020
/// @brief  Contains the Iterative Deepening algorithm.

#include "IterativeDeepening.hpp"

namespace m8::search {

    IterativeDeepening::IterativeDeepening(const Board& board,
                                          time::TimeManager& time_manager)
        : alpha_beta_(board,
                      time_manager),
          time_manager_(time_manager)
    {
        alpha_beta_.Attach(this);
    }

    SearchResult IterativeDeepening::Search(DepthType depth)
    {
        std::optional<SearchResult> result;
        std::optional<SearchResult> last_result;

        NotifySearchStarted();

        DepthType current_depth = 1;
        while(current_depth <= kMinimumSearchDepth ||
                  (current_depth <= depth &&
                 time_manager_.can_start_new_iteration()))
        {
            NotifyIterationStarted();
            result = alpha_beta_.Search(current_depth);
            if (result.has_value())
            {
                NotifyIterationCompleted(result.value().pv_,
                                         result.value().value_,
                                         current_depth,
                                         0,
                                         result.value().stats_.nodes);

                last_result = result;
            }

            ++current_depth;
        }

        NotifySearchCompleted(last_result.value().pv_, 0, last_result.value().stats_);

        return last_result.value();
    }

    void IterativeDeepening::Stop()
    {
        alpha_beta_.Stop();
    }

    void IterativeDeepening::OnNewBestMove(const PV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes)
    {
        NotifyNewBestMove(pv, eval, depth, time, nodes);
    }
}