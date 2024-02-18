/// @file CliCommand.hpp
/// @author Mathieu Pagé
/// @date February 2024
/// @brief Contains the CliCommand class

#ifndef M8_COMMANDS_CLI_COMMAND_HPP_
#define M8_COMMANDS_CLI_COMMAND_HPP_

#include "Command.hpp"
#include "../m8Intrf.hpp"

namespace m8::commands
{
    /// Implements the TEMPORARY cli commands. This commands will be remove later when 
    /// the uci interface is completed.
    class CliCommand : public Command
    {
    public:
        void operator()() const
        {
            m8::m8Intrf intrf;
            intrf.Execute();
        };

        /// Returns the descriptions of the command line options supported for this command
        boost::program_options::options_description GetOptionsDescriptions()
        {
            namespace po = boost::program_options;

            po::options_description command_options("CLI Options");
            return command_options;
        }
    };
}

#endif // M8_COMMANDS_CLI_COMMAND_HPP_