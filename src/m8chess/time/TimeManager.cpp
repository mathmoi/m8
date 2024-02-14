/// @file TimeManager.cpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023

#include "m8chess/time/TimeManager.hpp"

#include "m8chess/time/TimePerMoveTimeManager.hpp"
#include "m8chess/time/ConventionalTimeManager.hpp"
#include "m8chess/time/IncrementalTimeManager.hpp"

namespace m8::time
{
    std::unique_ptr<TimeManager> TimeManager::CreateTimeManager(const TimeControl& time_control,
                                                                ChessClock& clock)
    {
        switch (time_control.type())
        {
        case TimeControlType::Conventional:
            return std::make_unique<ConventionalTimeManager>(clock, static_cast<const ConventionalTimeControl&>(time_control));

        case TimeControlType::Incremental:
            return std::make_unique<IncrementalTimeManager>(clock, static_cast<const IncrementalTimeControl&>(time_control));
        
        case TimeControlType::TimePerMove:
            return std::make_unique<TimePerMoveTimeManager>(clock);
        
        default:
            assert(!"This code should never be reached.");
            return std::unique_ptr<TimeManager>(); // To silence warning about reaching the end of the method
        }
    }
}