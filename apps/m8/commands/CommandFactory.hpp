/// @file CommandFactory.hpp
/// @author Mathieu Pagé
/// @date February 2024
/// @brief Contains a factory method for commands

#ifndef M8_COMMANDS_COMMAND_FACTORY_HPP_
#define M8_COMMANDS_COMMAND_FACTORY_HPP_

#include <memory>
#include <string>

#include "Command.hpp"
#include "PerftCommand.hpp"
#include "UCICommand.hpp"

namespace m8::commands
{
    /// Create a Command object
    /// 
    /// @param command_name name of the command to create
    /// @return Returns a unique pointer to a command. If a command with the given name
    ///         cannot be created a null pointer is returned.
    std::unique_ptr<Command> CreateCommand(const std::string command_name)
    {
        if (command_name == "perft")
        {
            return std::make_unique<PerftCommand>();
        }

        if (command_name == "uci")
        {
            return std::make_unique<UCICommand>();
        }

        return nullptr;
    }
}

#endif // M8_COMMANDS_COMMAND_FACTORY_HPP_