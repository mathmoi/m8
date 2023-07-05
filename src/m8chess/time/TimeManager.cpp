/// @file TimeManager.cpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023

#include "TimeManager.hpp"

#include "TimePerMoveTimeManager.hpp"
#include "TimePerMoveChessClock.hpp"

namespace m8::time
{
    std::shared_ptr<TimeManager> TimeManager::CreateTimeManager(const TimeControl& time_control,
                                                                ChessClock& clock)
    {
        switch (time_control.type())
        {
        case TimeControlType::Conventional:
            assert(!"This path is not implemented yet.");

        case TimeControlType::Incremental:
            assert(!"This path is not implemented yet.");
        
        case TimeControlType::TimePerMove:
            return std::make_shared<TimePerMoveTimeManager>(clock);
        
        default:
            assert(!"This code should never be reached.");
            break;
        }
    }
}