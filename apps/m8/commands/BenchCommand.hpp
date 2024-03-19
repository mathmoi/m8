/// @file BenchCommand.hpp
/// @author Mathieu Pagé
/// @date march 2024
/// @brief Contains the BenchCommand class

#ifndef M8_COMMANDS_BENCH_COMMAND_HPP_
#define M8_COMMANDS_BENCH_COMMAND_HPP_

#include <thread>

#include "m8common/Output.hpp"

#include "m8chess/Benchmark.hpp"

#include "Command.hpp"

namespace m8::commands
{
    /// Implements the bench command that runs a bench test.
    class BenchCommand : public Command
    {
    public:
        BenchCommand()
        : deltaDepth_(0),
          threads_(0)
        {}

        void operator()()
        {
            Benchmark bench(deltaDepth_, runs_, threads_);
            bench.Run();
        };

        /// Returns the descriptions of the command line options supported for this command
        boost::program_options::options_description GetOptionsDescriptions()
        {
            namespace po = boost::program_options;

            unsigned int num_cpus = std::thread::hardware_concurrency();

            po::options_description command_options("Bench Options");
            command_options.add_options()
                ("delta-depth", po::value<std::int16_t>(&deltaDepth_), "Depth to add or remove from the default depth of each position.")
                ("threads",     po::value<std::uint32_t>(&threads_)->default_value(num_cpus), "Number of parallele threads to use for the benchmark.")
                ("runs",        po::value<std::uint32_t>(&runs_)->default_value(num_cpus), "Number of times all the position are searched. The result will be the means of the runs after the fastest and slowest runs are removed.");
            return command_options;
        }
        
    private:
        std::int16_t deltaDepth_;
        std::uint32_t threads_;
        std::uint32_t runs_;
    };
}

#endif // M8_COMMANDS_BENCH_COMMAND_HPP_