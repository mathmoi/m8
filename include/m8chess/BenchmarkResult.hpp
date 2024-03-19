/// @file BenchmarkResult.hpp
/// @author Mathieu Pagé
/// @date March 2024
/// @brief Contains the benchmark result struct

#ifndef M8_BENCHMARK_RESULT_HPP_
#define M8_BENCHMARK_RESULT_HPP_

#include "m8common/chronoHelpers.hpp"
#include "m8common/Timer.hpp"

#include "m8chess/Types.hpp"

namespace m8
{
    /// Class use to contains the result of a benchmark test
    class BenchmarkResult
    {
    public:
        /// Constructor
        /// 
        /// @param duration duration of the test.
        /// @param nodes    Number of nodes searched in the test.
        inline BenchmarkResult(Timer::ClockType::duration duration = std::chrono::seconds(0),
                               NodeCounterType nodes = 0)
        : duration_(duration),
          nodes_(nodes)
        {}

        /// Returns the duration of the test
        inline Timer::ClockType::duration duration() const { return duration_; }

        /// Returns the number of nodes searched durint the test
        inline NodeCounterType nodes() const { return nodes_; }

        /// Returns the nodes per seconds for the test.
        inline NodeCounterType nps() const { return nodes_ / ToFSec(duration_).count(); }

        inline BenchmarkResult& operator+=(const BenchmarkResult rhs)
        {
            this->duration_ += rhs.duration_;
            this->nodes_    += rhs.nodes_;
            return *this;
        }

        inline BenchmarkResult operator+(const BenchmarkResult rhs)
        {
            return BenchmarkResult(this->duration_ + rhs.duration_,
                                   this->nodes_    + rhs.nodes_);
        }

    private:
        Timer::ClockType::duration duration_;
        NodeCounterType nodes_;
    };
}

#endif // M8_BENCHMARK_RESULT_HPP_