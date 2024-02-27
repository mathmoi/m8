/// @file TimeManager.hpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023

#ifndef m8_TIME_TIME_MANAGER_HPP_
#define m8_TIME_TIME_MANAGER_HPP_

#include <memory>

#include "../Types.hpp"
#include "../search/PV.hpp"
#include "../search/ISearchObserver.hpp"
#include "ChessClock.hpp"

namespace m8::time
{
    /// Base class for all time managers. Time manager are responsible for the engine's
    /// time management. They tell it when to stop searching.
    class TimeManager : public search::ISearchObserver<search::PV, Move>
    {
    public:

        /// Constructor
        /// 
        /// @param clock Pointer to the engine's chess clock
        TimeManager(ChessClock& clock)
        : clock_(clock),
          last_iteration_duration(std::chrono::seconds(0)),
          second_last_iteration_duration(std::chrono::seconds(0)),
          iterations_completed_(0)
        {}

        /// Create a TimeManager object
        /// 
        /// @param time_control Time control to use
        /// @param clock        Clock used to keep the engine's time
        static std::unique_ptr<TimeManager> CreateTimeManager(const TimeControl& time_control,
                                                              ChessClock& clock);

        /// Initialise the time manager for a search. This should be called before the
        /// begining of each search.
        virtual void InitSearch() {}

        /// Indicate if the search can continue. The search needs to call this regularly
        /// after searching a number of nodes defined by the method.
        ///  CalculateNodesBeforeNextCheck.
        virtual bool can_continue() const = 0;

        /// Indicate if the search can start a new iteration.
        virtual bool can_start_new_iteration() const = 0;

        /// Method called when an iteration is started.
        inline void OnIterationStarted()
        {
            iteration_start = std::chrono::steady_clock::now();
        }

        /// Method called when an iteration is completed.
        inline void OnIterationCompleted(const search::PV& pv,
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

        /// Returns the number of nodes can be searched before we need to make another 
        /// call to can_continue.
        /// 
        /// @param nodes_searched   Number of nodes searched since the begining of the 
        ///                         current search
        /// @return NodeCounterType Number of nodes to search before the next call to
        ///                         can_continue
        virtual NodeCounterType CalculateNodesBeforeNextCheck(NodeCounterType nodes_searched) const = 0;

    protected:
        const DepthType            kMinDepth                 = 3;
        const ChessClock::Duration kSafetyBuffer             = std::chrono::milliseconds( 50);
        const ChessClock::Duration kMinDurationBetweenChecks = std::chrono::milliseconds( 10);
        const ChessClock::Duration kMaxDurationBetweenChecks = std::chrono::milliseconds(250); // TODO : When we get a bench method. Check the incidence of this parameter on performances.
        
        inline const ChessClock& clock() const { return clock_; }

        /// Indicate if the search need to continue because the minimum depth is not 
        /// searched yet.
        inline bool need_to_continue() const
        {
            return iterations_completed_ < kMinDepth;
        }

        inline ChessClock::Duration next_iteration_estimated_time() const
        {
            return last_iteration_duration * (last_iteration_duration / second_last_iteration_duration);
        }

    private:
        ChessClock& clock_;

        std::chrono::steady_clock::time_point iteration_start;
        ChessClock::Duration last_iteration_duration,
                                second_last_iteration_duration;

        DepthType iterations_completed_;
    };
}

#endif // m8_TIME_TIME_MANAGER_HPP_