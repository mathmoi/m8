/// @file PerftCommand.hpp
/// @author Mathieu Pagé
/// @date February 2024
/// @brief Contains the PerftCommand class

#ifndef M8_COMMANDS_PERFT_COMMAND_HPP_
#define M8_COMMANDS_PERFT_COMMAND_HPP_

#include <cstdint>
#include <iostream> // TODO : Remove this include

#include "m8chess/Board.hpp"
#include "Command.hpp"

namespace m8::commands
{
    /// Implements the Perft commands that runs a perft test.
    class PerftCommand : public Command
    {
    public:
        void operator()() const
        {
            std::cout <<"Hello, Perft at depth " <<depth_ <<"!\n"
                      <<"fen: " <<fen_ <<std::endl;
        };

        /// Returns the descriptions of the command line options supported for this command
        boost::program_options::options_description GetOptionsDescriptions()
        {
            namespace po = boost::program_options;

            po::options_description command_options("Perft Options");
            command_options.add_options()
                ("depth,d", po::value<std::uint32_t>(&depth_)->required(), "Depth of the perft test (required)")
                ("fen,f",   po::value<std::string>(&fen_)->default_value(kStartingPositionFEN), "FEN string representing the position to use for the perft test");
            return command_options;
        }
        
    private:
        std::uint32_t depth_;
        std::string fen_;
    };
}

#endif // M8_COMMANDS_PERFT_COMMAND_HPP_