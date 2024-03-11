/// @file TimeManager.cpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023

#include "m8common/chronoHelpers.hpp"
#include "m8common/logging.hpp"

#include "m8chess/time/TimeManager.hpp"

namespace m8::time
{
    TimeManager::TimeManager(std::optional<std::chrono::milliseconds> time,
                             std::optional<std::chrono::milliseconds> increment,
                             std::optional<std::uint32_t>             moves_to_go,
                             std::optional<std::chrono::milliseconds> move_time,
                             bool                                     infinite)
    : clock_(TimerDirection::Up),
      iteration_clock_(TimerDirection::Up),
      last_iteration_duration(std::chrono::seconds(0)),
      second_last_iteration_duration(std::chrono::seconds(0)),
      iterations_completed_(0)
    {
        if (move_time)
        {
            min_duration = move_time.value() - kSafetyBuffer;
            target_duration = min_duration;
            max_duration = min_duration;
        }
        else if (infinite)
        {
            min_duration = Duration::max();
            target_duration = min_duration;
            max_duration = min_duration;
        }
        else
        {
            // Conventional/Incremental time control
            auto time_on_clock = time ? time.value() : Duration(0);
            auto mtg = moves_to_go.value_or(kMovesToGoEstimate);
            auto inc = increment.value_or(std::chrono::milliseconds(0));
            min_duration = Duration(0);
            target_duration = (time_on_clock + (mtg - 1) * inc) / mtg;
            max_duration = std::chrono::duration_cast<Duration>(std::min(target_duration * kMaxOvertargetFactor,
                                                                         time_on_clock * kMaxMaxDurationRatio));
        }
        M8_DEBUG <<"TimeManager::TimeManager: time:" <<ToString(time)
                 <<" increment:"       <<ToString(increment)
                 <<" moves_to_go:"     <<(moves_to_go ? boost::lexical_cast<std::string>(*moves_to_go) : "std::nullopt")
                 <<" move_time:"       <<ToString(move_time)
                 <<" infinite:"        <<(infinite ? "true" : "false")
                 <<" min_duration:"    <<ToString(ToFSec(min_duration))
                 <<" target_duration:" <<ToString(ToFSec(target_duration))
                 <<" max_duration:"    <<ToString(ToFSec(max_duration));
    }
    
    bool TimeManager::can_continue() const
    {
        return need_to_continue()
            || clock_.time_on_clock() < max_duration;
    }

    bool TimeManager::can_start_new_iteration() const
    {
        if (need_to_continue())
        {
            return true;
        }

        if (!can_continue())
        {
            return false;
        }

        // return true if we can complete at least half the next iteration before the
        // target time.
        auto time_before_target = target_duration - clock_.time_on_clock();
        auto next_iteration_estimate = next_iteration_estimated_time();
        M8_DEBUG <<"Next iteration estimated time: " <<ToString(ToFSec(next_iteration_estimate));
        return next_iteration_estimate / 2 < time_before_target;
    }

    void TimeManager::OnSearchStarted()
    {
        clock_.Reset();
        clock_.Start();
    }

    void TimeManager::OnIterationStarted()
    {
        iteration_clock_.Start();
    }

    void TimeManager::OnIterationCompleted(const search::PV& pv,
                                           EvalType eval,
                                           DepthType depth,
                                           double time,
                                           NodeCounterType nodes)
    {
        second_last_iteration_duration = last_iteration_duration;
        last_iteration_duration = iteration_clock_.time_on_clock();
        ++iterations_completed_;

        M8_DEBUG <<"Iteration time: " <<ToString(ToFSec(last_iteration_duration));
    }

    void TimeManager::OnSearchCompleted(const search::PV& pv, double time, const search::SearchStats& stats)
    {
        M8_DEBUG <<"Search time: " <<ToString(ToFSec(clock_.time_on_clock()));
    }

    NodeCounterType TimeManager::CalculateNodesBeforeNextCheck(NodeCounterType nodes_searched) const
    {
        auto elapsed = clock_.time_on_clock();
        auto time_searched = ToFSec(elapsed);

        NodeCounterType nps = nodes_searched / time_searched.count();

        auto duration_before_check = (max_duration - elapsed) / 2;
        duration_before_check = std::clamp(duration_before_check,
                                            kMinDurationBetweenChecks,
                                            kMaxDurationBetweenChecks);

        return nps * ToFSec(duration_before_check).count();
    }

    bool TimeManager::need_to_continue() const
    {
        return iterations_completed_ < kMinDepth
            || clock_.time_on_clock() < min_duration;
    }

    TimeManager::Duration TimeManager::next_iteration_estimated_time() const
    {
        return last_iteration_duration * (last_iteration_duration / second_last_iteration_duration);
    }
}