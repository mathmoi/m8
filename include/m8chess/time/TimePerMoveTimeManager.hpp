/// @file TimePerMoveTimeManager.hpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023

#ifndef M8_TIME_TIME_PER_MOVE_TIME_MANAGER_HPP_
#define M8_TIME_TIME_PER_MOVE_TIME_MANAGER_HPP_

#include <algorithm>

#include "../../m8common/chronoHelpers.hpp"
#include "TimeManager.hpp"

namespace m8::time
{
    class TimePerMoveTimeManager : public TimeManager
    {
    public:

        /// Constructor
        /// 
        /// @param clock Pointer to the engine's chess clock.
        TimePerMoveTimeManager(ChessClock& clock)
        : TimeManager(clock)
        {}

        /// Indicate if the search can continue. The search needs to call this regularly
        /// after searching a number of nodes defined by the method
        ///  CalculateNodesBeforeNextCheck.
        bool can_continue() const
        {
            if (need_to_continue())
            {
                return true;
            }
            
            return kSafetyBuffer < clock().time_on_clock();
        }

        /// Indicate if the search can start a new iteration.
        bool can_start_new_iteration() const { return can_continue(); };

        /// Returns the number of nodes can be searched before we need to make another 
        /// call to can_continue.
        /// 
        /// @param nodes_searched   Number of nodes searched since the begining of the 
        ///                         current search.
        /// @return NodeCounterType Number of nodes to search before the next call to
        ///                         can_continue.
        NodeCounterType CalculateNodesBeforeNextCheck(NodeCounterType nodes_searched) const
        {
            auto time_searched = ToFSec(clock().elapsed());
            NodeCounterType nps = nodes_searched / time_searched.count();

            auto duration_before_check = (clock().time_on_clock() - kSafetyBuffer) / 2;
            duration_before_check = std::clamp(duration_before_check,
                                               kMinDurationBetweenChecks,
                                               kMaxDurationBetweenChecks);
            
            return nps * ToFSec(duration_before_check).count();
        }
    };
}

#endif // M8_TIME_TIME_PER_MOVE_TIME_MANAGER_HPP_