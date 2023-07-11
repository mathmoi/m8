/// @file IncrementalTimeManager.hpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023

#ifndef M8_INCREMENTAL_TIME_MANAGER_HPP_
#define M8_INCREMENTAL_TIME_MANAGER_HPP_

#include <algorithm>
#include <chrono>

#include "TimeManager.hpp"
#include "IncrementalTimeControl.hpp"

namespace m8::time
{
    class IncrementalTimeManager : public TimeManager
    {
    private:
        const float kRatioMaximumOverflow = 3.0;
        const std::uint32_t kMovesRemainingsEstimate = 35;

    public:

        /// Constructor
        /// 
        /// @param clock        Pointer to the engine's chess clock.
        /// @param time_control Time control
        IncrementalTimeManager(ChessClock& clock,
                               IncrementalTimeControl time_control)
        : TimeManager(clock),
          time_control_(time_control)
        {}

        /// Method called when the search starts.
		inline void OnSearchStarted()
        {
            auto time_on_clock = clock().time_on_clock();
            target_time_ = time_on_clock / kMovesRemainingsEstimate + time_control_.increment();
            maximum_time_ = duration_cast<ChessClock::Duration>(target_time_ * kRatioMaximumOverflow);

            maximum_time_ = std::min(maximum_time_, clock().time_on_clock() - kSafetyBuffer);
        }

        /// Indicate if the search can continue. The search needs to call this regularly
        /// after searching a number of nodes defined by the method
        ///  CalculateNodesBeforeNextCheck.
        bool can_continue() const {
            if (need_to_continue())
            {
                return true;
            }
            
            return clock().elapsed() < maximum_time_; }

        /// Indicate if the search can start a new iteration.
        bool can_start_new_iteration() const {
            if (need_to_continue())
            {
                return true;
            }

            // return true if we can complete at least half the next iteration before the
            // target time.
            auto time_before_target = target_time_ - clock().elapsed();
            return next_iteration_estimated_time() / 2 < time_before_target;
        };

        /// Returns the number of nodes can be searched before we need to make another 
        /// call to can_continue.
        /// 
        /// @param nodes_searched   Number of nodes searched since the begining of the 
        ///                         current search.
        /// @return NodeCounterType Number of nodes to search before the next call to
        ///                         can_continue.
        NodeCounterType CalculateNodesBeforeNextCheck(NodeCounterType nodes_searched) const
        {
            auto elapsed = clock().elapsed();
            auto time_searched = ToFSec(elapsed);
            NodeCounterType nps = nodes_searched / time_searched.count();

            auto duration_before_check = (maximum_time_ - elapsed) / 2;
            duration_before_check = std::clamp(duration_before_check,
                                               kMinDurationBetweenChecks,
                                               kMaxDurationBetweenChecks);

            return nps * ToFSec(duration_before_check).count();
        }

        private:
            IncrementalTimeControl time_control_;

            ChessClock::Duration target_time_;
            ChessClock::Duration maximum_time_;
    };
}

#endif // M8_INCREMENTAL_TIME_MANAGER_HPP_