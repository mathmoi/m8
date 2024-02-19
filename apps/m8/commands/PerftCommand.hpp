/// @file PerftCommand.hpp
/// @author Mathieu Pagé
/// @date February 2024
/// @brief Contains the PerftCommand class

#ifndef M8_COMMANDS_PERFT_COMMAND_HPP_
#define M8_COMMANDS_PERFT_COMMAND_HPP_

#include <cstdint>

#include "m8common/Output.hpp"
#include "m8common/Utils.hpp"

#include "m8chess/Board.hpp"
#include "m8chess/Perft.hpp"
#include "Command.hpp"

namespace m8::commands
{
    /// Implements the Perft commands that runs a perft test.
    class PerftCommand : public Command, public IPerftObserver
    {
    public:
        void operator()()
        {
            Board board(fen_);
            m8::Perft perft(depth_, board, this);
            perft.Run();
        };

        /// Returns the descriptions of the command line options supported for this command
        boost::program_options::options_description GetOptionsDescriptions()
        {
            namespace po = boost::program_options;

            po::options_description command_options("Perft Options");
            command_options.add_options()
                ("depth",   po::value<std::uint32_t>(&depth_)->required(), "Depth of the perft test (required)")
                ("fen",     po::value<std::string>(&fen_)->default_value(kStartingPositionFEN), "FEN string representing the position to use for the perft test")
                ("threads", po::value<std::int32_t>(&options::Options::get().perft_threads), "Number of parallele threads to use for the perft test");
            return command_options;
        }

        /// Method called everytime a partial perf result is ready
        /// 
        /// @param move  The move for which the result is available 
        /// @param count The number of nodes
        void OnPartialPerftResult(const std::string& move, std::uint64_t count)
        {
            Output out;
            out << move << '\t' << count <<'\n';
        }

        /// Method called at the end of the perft test
        /// 
        /// @param count The number of nodes
        /// @param time  The time used to complete the test
        void OnPerftCompleted(std::uint64_t count, double time)
        {
            Output out;
            out << '\n'
                << "Threads: " <<options::Options::get().perft_threads <<'\n'
                << "Nodes: " << count << '\n'
                << "Time : " << time << '\n'
                << "Nodes per second: " << AddMetricSuffix(static_cast<std::uint64_t>(count / time), 3) << std::endl;
        }
        
    private:
        std::uint32_t depth_;
        std::string fen_;
    };
}

#endif // M8_COMMANDS_PERFT_COMMAND_HPP_