/// @file ConventionalTimeManager.hpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023

#ifndef M8_CONVENTIONAL_TIME_MANAGER_HPP_
#define M8_CONVENTIONAL_TIME_MANAGER_HPP_

#include <algorithm>

#include "../../m8common/chronoHelpers.hpp"
#include "TimeManager.hpp"
#include "ConventionalChessClock.hpp"

namespace m8::time
{
    class ConventionalTimeManager : public TimeManager
    {
    private:
        const float kRatioMaximumOverflow = 3.0;

        const ConventionalTimeControl::NumberOfMoves kMovesSafetyMargin = 1;

    public:

        /// Constructor
        /// 
        /// @param clock        Pointer to the engine's chess clock.
        /// @param time_control Time control
        ConventionalTimeManager(ChessClock& clock,
                                ConventionalTimeControl time_control)
        : TimeManager(clock),
          time_control_(time_control),
          last_iteration_duration(std::chrono::seconds(0)),
          second_last_iteration_duration(std::chrono::seconds(0)),
          iterations_completed_(0)
        {}

        /// Method called when the search starts.
		inline virtual void OnSearchStarted()
        {
            // Calculate a target time to get to the next control
            auto moves_before_control   = dynamic_cast<const ConventionalChessClock&>(clock()).moves_before_control();
            auto time_before_control    = clock().time_on_clock() - kSafetyBuffer;
            auto target_current_control = time_before_control / (moves_before_control + kMovesSafetyMargin);

            // Calculate a target time to get to the second next control
            auto moves_before_second_control = moves_before_control + time_control_.moves();
            auto time_before_second_control  = time_before_control + time_control_.duration();
            auto target_next_control         = time_before_second_control / (moves_before_second_control + kMovesSafetyMargin);

            // Choose the minimum target time for the next or second next control
            target_time_ = std::min(target_current_control, target_next_control);

            // Calculate a maximum time based on a ratio of the target time. If there is
            // only one move left, we can't take more then the time on the clock. If there
            // is more than one move left, we can't take more than half the time remaining
            // on the clock.
            maximum_time_ = duration_cast<ChessClock::Duration>(target_time_ * kRatioMaximumOverflow);
            maximum_time_ = std::min(maximum_time_,
                                     1 < moves_before_control ? time_before_control / 2  :  time_before_control) - kSafetyBuffer;
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
        virtual bool can_continue() const { return clock().elapsed() < maximum_time_; }

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
            ConventionalTimeControl time_control_;

            std::chrono::steady_clock::time_point iteration_start;
            ChessClock::Duration last_iteration_duration,
                                 second_last_iteration_duration;

            DepthType iterations_completed_;

            ChessClock::Duration target_time_;
            ChessClock::Duration maximum_time_;
    };
}

#endif // M8_CONVENTIONAL_TIME_MANAGER_HPP_