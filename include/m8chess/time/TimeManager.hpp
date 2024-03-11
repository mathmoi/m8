/// @file TimeManager.hpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023

#ifndef m8_TIME_TIME_MANAGER_HPP_
#define m8_TIME_TIME_MANAGER_HPP_

#include <memory>

#include "m8common/Timer.hpp"

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
        TimeManager(std::optional<std::chrono::milliseconds> time,
                    std::optional<std::chrono::milliseconds> increment,
                    std::optional<std::uint32_t>             moves_to_go,
                    std::optional<std::chrono::milliseconds> move_time,
                    bool                                     infinite);

        /// Indicate if the search can continue. The search needs to call this regularly
        /// after searching a number of nodes defined by the method.
        ///  CalculateNodesBeforeNextCheck.
        bool can_continue() const;

        /// Indicate if the search can start a new iteration.
        bool can_start_new_iteration() const;

        /// Method called when the search starts.
        void OnSearchStarted();

        /// Method called when an iteration is started.
        void OnIterationStarted();

        /// Method called when an iteration is completed.
        void OnIterationCompleted(const search::PV& pv,
                                  EvalType eval,
                                  DepthType depth,
                                  double time,
                                  NodeCounterType nodes);
        
        /// Method when the search is completed.
        void OnSearchCompleted(const search::PV& pv, double time, const search::SearchStats& stats);

        /// Returns the number of nodes can be searched before we need to make another 
        /// call to can_continue.
        /// 
        /// @param nodes_searched   Number of nodes searched since the begining of the 
        ///                         current search
        /// @return NodeCounterType Number of nodes to search before the next call to
        ///                         can_continue
        NodeCounterType CalculateNodesBeforeNextCheck(NodeCounterType nodes_searched) const;

    private:
        typedef Timer::ClockType::duration Duration;

        const DepthType     kMinDepth                 = 3;
        const double        kMaxOvertargetFactor      = 5.0;
        const double        kMaxMaxDurationRatio      = 0.8;
        const std::uint32_t kMovesToGoEstimate        = 35;
        const Duration      kSafetyBuffer             = std::chrono::milliseconds( 50);
        const Duration      kMinDurationBetweenChecks = std::chrono::milliseconds( 10);
        const Duration      kMaxDurationBetweenChecks = std::chrono::milliseconds(250); // TODO : When we get a bench method. Check the incidence of this parameter on performances.
        
        Timer clock_;
        Timer iteration_clock_;

        Duration last_iteration_duration,
                 second_last_iteration_duration;
        DepthType iterations_completed_;

        Duration min_duration;
        Duration max_duration;
        Duration target_duration;

        bool need_to_continue() const;
        ChessClock::Duration next_iteration_estimated_time() const;
    };
}

#endif // m8_TIME_TIME_MANAGER_HPP_o