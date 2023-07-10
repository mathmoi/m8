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
          time_control_(time_control),
          last_iteration_duration(std::chrono::seconds(0)),
          second_last_iteration_duration(std::chrono::seconds(0)),
          iterations_completed_(0)
        {}

        /// Method called when the search starts.
		inline virtual void OnSearchStarted()
        {
            auto time_on_clock = clock().time_on_clock();
            target_time_ = time_on_clock / kMovesRemainingsEstimate + time_control_.increment();
            maximum_time_ = duration_cast<ChessClock::Duration>(target_time_ * kRatioMaximumOverflow);

            maximum_time_ = std::min(maximum_time_, clock().time_on_clock() - kSafetyBuffer);
        }

        /// Method called when an iteration is started.
        inline virtual void OnIterationStarted()
        {
            iteration_start = std::chrono::steady_clock::now();
        }

        /// Method called when an iteration is completed.
		inline virtual void OnIterationCompleted(const search::PV& pv,
                                                EvalType eval,
                                                DepthType depth,
                                                double time,
                                                NodeCounterType nodes)
        {
            auto iteration_end = std::chrono::steady_clock::now();
            second_last_iteration_duration = last_iteration_duration;
            last_iteration_duration = iteration_end - iteration_start;

            ++iterations_completed_;
        }

        /// Indicate if the search can continue. The search needs to call this regularly
        /// after searching a number of nodes defined by the method
        ///  CalculateNodesBeforeNextCheck.
        virtual bool can_continue() const {
            return clock().elapsed() < maximum_time_; }

        /// Indicate if the search can start a new iteration.
        virtual bool can_start_new_iteration() const {
            // If two iterations have not been completed we can't estimate the time for 
            // the next iteration. In this condition we continue another iteration.
            if (iterations_completed_ < 2)
            {
                return true;
            }

            // Estimate the time of the next iteration
            auto next_iteration_estimated_time = last_iteration_duration * (last_iteration_duration / second_last_iteration_duration);
            auto time_before_target = target_time_ - clock().elapsed();
            
            // return true if we can complete at least half the next iteration before the
            // target time.
            return next_iteration_estimated_time / 2 < time_before_target;
        };

        /// Returns the number of nodes can be searched before we need to make another 
        /// call to can_continue.
        /// 
        /// @param nodes_searched   Number of nodes searched since the begining of the 
        ///                         current search.
        /// @return NodeCounterType Number of nodes to search before the next call to
        ///                         can_continue.
        virtual NodeCounterType CalculateNodesBeforeNextCheck(NodeCounterType nodes_searched) const
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

            std::chrono::steady_clock::time_point iteration_start;
            ChessClock::Duration last_iteration_duration,
                                 second_last_iteration_duration;

            DepthType iterations_completed_;

            ChessClock::Duration target_time_;
            ChessClock::Duration maximum_time_;
    };
}

#endif // M8_INCREMENTAL_TIME_MANAGER_HPP_