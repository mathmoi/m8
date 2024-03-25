/// @file AnalyzeCommand.hpp
/// @author Mathieu Pagé
/// @date March 2024
/// Contains the AnalyzeCommand class

#ifndef M8_COMMANDS_ANALYZE_COMMAND_HPP_
#define M8_COMMANDS_ANALYZE_COMMAND_HPP_

#include <cstdint>

#include "m8chess/Analyzer.hpp"
#include "m8chess/Types.hpp"

#include "Command.hpp"

namespace m8::commands
{
    class AnalyzeCommand : public Command
    {
    public:
        AnalyzeCommand()
        : time_()
        {}
    
        void operator()()
        {
            Analyzer analyzer(fen_, depth_, time_);
            analyzer.Analyze();
        }

        /// Returns the descriptions of the command line options supported for this command
        boost::program_options::options_description GetOptionsDescriptions()
        {
            namespace po = boost::program_options;

            po::options_description command_options("Analyze Options");
            command_options.add_options()
                ("fen",   po::value<std::string>(&fen_)->default_value(kStartingPositionFEN),
                    "FEN string representing the position to analyze")
                ("depth", po::value<DepthType>(&depth_)->default_value(std::numeric_limits<DepthType>::max()),
                    "Maximum depth of the analysis")
                ("time",  po::value<float>(&time_)->default_value(std::numeric_limits<float>::infinity()),
                    "Time to analyze the position");

            return command_options;
        }
        
    private:
        std::string fen_;
        DepthType depth_;
        float time_;

    };
}

#endif // M8_COMMANDS_ANALYZE_COMMAND_HPP_