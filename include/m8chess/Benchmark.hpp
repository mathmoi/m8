/// @file Benchmark.hpp
/// @author Mathieu Pagé
/// @date March 2024
/// @brief Contains the benchmark class that runs benchmark of the search algorithm

#ifndef M8_BENCHMARK_HPP_
#define M8_BENCHMARK_HPP_

#include <mutex>
#include <thread>


#include "m8chess/search/Searcher.hpp"

#include "BenchmarkResult.hpp"
#include "Types.hpp"

namespace m8
{
    class Benchmark
    {
    public:
        /// Constructor
        /// 
        /// @param deltaDepth Positive or negative delta to apply to the depth of all the
        ///                   position searched in the benchmark.
        /// @param runs Number of times each positions is runned.
        /// @param threads_count Number of runs that can be executed in parrallel.
        Benchmark(DepthType deltaDepth, std::uint32_t runs, std::uint32_t threads_count);

        /// Run the benchmark.
        void Run();
        
    private:
        DepthType deltaDepth_;
        std::uint32_t runs_;
        std::uint32_t threads_count_;
        std::vector<BenchmarkResult> results_;

        std::vector<std::thread> threads_;
        std::mutex mutex_;
        bool abort_;

        static std::array<std::pair<std::string, DepthType>, 64> positions;

        BenchmarkResult RunPosition(std::string_view fen, DepthType depth);
        BenchmarkResult RunBenchmark();
        void RunBenchmarks();
    };
}

#endif // M8_BENCHMARK_HPP_