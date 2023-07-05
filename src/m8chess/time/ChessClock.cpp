/// @file ChessClock.cpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023

#include "ChessClock.hpp"
#include "ConventionalChessClock.hpp"
#include "IncrementalChessClock.hpp"
#include "TimePerMoveChessClock.hpp"

namespace m8::time
{
    std::unique_ptr<ChessClock> ChessClock::CreateChessClock(const TimeControl& time_control)
    {
        switch (time_control.type())
        {
        case TimeControlType::Conventional:
            return std::make_unique<ConventionalChessClock>(static_cast<const ConventionalTimeControl&>(time_control));

        case TimeControlType::Incremental:
            return std::make_unique<IncrementalChessClock>(static_cast<const IncrementalTimeControl&>(time_control));
        
        case TimeControlType::TimePerMove:
            return std::make_unique<TimePerMoveChessClock>(static_cast<const TimePerMoveTimeControl&>(time_control));
        
        default:
            assert(!"This code should never be reached.");
            break;
        }
    }
}