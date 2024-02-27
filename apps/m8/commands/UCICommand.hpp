/// @file UCICommand.hpp
/// @author Mathieu Pagé
/// @date February 2024
/// @brief Contains the UCICommand class

#ifndef M8_COMMANDS_UCI_COMMAND_HPP_
#define M8_COMMANDS_UCI_COMMAND_HPP_

#include <cstdint>

#include "m8common/Output.hpp"

#include "../uci/UCI.hpp"

#include "Command.hpp"

namespace m8::commands
{
    /// Implements the Perft commands that runs a perft test.
    class UCICommand : public Command
    {
    public:
        void operator()()
        {
            uci::UCI uci;
            uci.Run();
        };

        /// Returns the descriptions of the command line options supported for this command
        boost::program_options::options_description GetOptionsDescriptions()
        {
            namespace po = boost::program_options;

            po::options_description command_options;
            return command_options;
        }
    };
}

#endif // M8_COMMANDS_UCI_COMMAND_HPP_