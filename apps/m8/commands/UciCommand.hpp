/// @file UciCommand.hpp
/// @author Mathieu Pagé
/// @date February 2024
/// @brief Contains the UciCommand class

#ifndef M8_COMMANDS_UCI_COMMAND_HPP_
#define M8_COMMANDS_UCI_COMMAND_HPP_

#include <cstdint>
#include <iostream> // TODO : Remove this include

#include "Command.hpp"

namespace m8::commands
{
    /// Implements the Perft commands that runs a perft test.
    class UciCommand : public Command
    {
    public:
        void operator()() const
        {
            std::cout <<"Hello, UCI!" <<std::endl;
        };

        /// Returns the descriptions of the command line options supported for this command
        boost::program_options::options_description GetOptionsDescriptions()
        {
            namespace po = boost::program_options;

            po::options_description command_options;
            // command_options.add_options()
            //     ("depth,d", po::value<std::uint32_t>(), "Depth of the perft test")
            //     ("fen,f",   po::value<std::string>()->default_value(kStartingPositionFEN), "FEN string representing the position to use for the perft test");
            return command_options;
        }
        
    private:
    
    
    };
}

#endif // M8_COMMANDS_UCI_COMMAND_HPP_