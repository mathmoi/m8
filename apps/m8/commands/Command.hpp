/// @file Command.hpp
/// @author Mathieu Pagé
/// @date February 2024
/// @brief Contains the base class for all commands

#include <boost/program_options.hpp>

#include <string>

#ifndef M8_COMMANDS_COMMAND_HPP_
#define M8_COMMANDS_COMMAND_HPP_

namespace m8::commands
{
    /// Base class for all m8 commands
    class Command
    {
    public:
        /// Pure virtual method that execute the command.
        virtual void operator()() const = 0;

        /// Returns the descriptions of the command line options supported for this command
        virtual boost::program_options::options_description GetOptionsDescriptions() = 0;
    };
}

#endif // M8_COMMANDS_COMMAND_HPP_