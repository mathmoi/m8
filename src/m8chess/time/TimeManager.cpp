/// @file TimeManager.cpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023

#include "TimeManager.hpp"

#include "TimePerMoveTimeManager.hpp"
#include "ConventionalTimeManager.hpp"
#include "IncrementalTimeManager.hpp"

namespace m8::time
{
    std::shared_ptr<TimeManager> TimeManager::CreateTimeManager(const TimeControl& time_control,
                                                                ChessClock& clock)
    {
        switch (time_control.type())
        {
        case TimeControlType::Conventional:
            return std::make_shared<ConventionalTimeManager>(clock, static_cast<const ConventionalTimeControl&>(time_control));

        case TimeControlType::Incremental:
            return std::make_shared<IncrementalTimeManager>(clock, static_cast<const IncrementalTimeControl&>(time_control));
        
        case TimeControlType::TimePerMove:
            return std::make_shared<TimePerMoveTimeManager>(clock);
        
        default:
            assert(!"This code should never be reached.");
            return std::shared_ptr<TimeManager>(); // To silence warning about reaching the end of the method
        }
    }
}