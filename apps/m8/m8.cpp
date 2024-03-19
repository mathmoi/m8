/// @file   m8.cpp
/// @author Mathieu Pagé
/// @date   May 2015
/// @brief  Contains the entry point (main) of m8.

#include <algorithm>
#include <fstream>
#include <utility>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "m8chess/Init.hpp"
#include "m8common/options/Options.hpp"
#include "m8common/logging.hpp"
#include "m8common/Output.hpp"
#include "commands/CommandFactory.hpp"

namespace po = boost::program_options;

namespace m8
{
    std::tuple<std::unique_ptr<commands::Command>, int, char**> GetCommand(int argc, char* argv[])
    {
        // If there is no commands, we return uci, the default command.
        if (   argc < 2
            || !std::all_of(argv[1], argv[1] + std::strlen(argv[1]), [](char c) { return std::isalpha(c); }))
        {
            return { commands::CreateCommand("uci"), argc, argv };
        }

        auto command = commands::CreateCommand(argv[1]);
        if (command)
        {
            return { std::move(command), argc - 1, argv + 1 };
        }

        return { std::move(command), argc, argv };
    }

    void DisplayHelpMessage(boost::program_options::options_description &all_options)
    {
        Output out;
        out << "usage : m8 [command] [options]\n"
            << '\n'
            << "Allowed commands\n"
            << "  uci    Launch m8 in UCI mode (this is the default command)\n"
            << "  perft  Run a perft test, counting the nodes reachables from a position at a given depth.\n"
            << "  bench  Run a benchmark"
            << '\n'
            << "The command is optional. If a command is not provided, m8 execute in UCI mode.\n";

        out << all_options << std::endl;
    }

    std::pair<po::options_description, po::variables_map>  ParseOptions(commands::Command* command, int argc, char* argv[])
    {
        po::options_description all_options;
        all_options.add(m8::options::GenerateGlobalOptionsDescriptions());
        if (command)
        {
            all_options.add(command->GetOptionsDescriptions());
        }
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, all_options), vm);
        return { all_options, vm };
    }
}

/// Main function. The entry point of the application.
///
/// @param argc Number of arguments in argv
/// @param argv Arguments passed on the command line when the application
///             is started.
/// @returns Return code of the application. Should be 0 if the application 
///          terminated correctly and another value otherwise.
int main(int argc, char* argv[])
{
    M8_LOG_SCOPE_THREAD();

    try {
        m8::options::ReadOptionsFromFile("m8.json");

        std::unique_ptr<m8::commands::Command> command(nullptr);
        std::tie(command, argc, argv) = m8::GetCommand(argc, argv);

        auto [all_options, vm] = m8::ParseOptions(command.get(), argc, argv);
        
        if (vm.count("help") || !command) {
            m8::DisplayHelpMessage(all_options);
            return 0;
        }
        
        po::notify(vm);

        m8::InitializePreCalc();
        (*command)();
    }
    catch (const po::required_option& ex)
    {
        M8_ERROR << ex.what() <<'\n'
                 << "To see a list of all options available, including required options, use \"m8 <command> --help\".";
        std::cerr << ex.what() <<'\n'
                  << "To see a list of all options available, including required options, use \"m8 <command> --help\"."
                  << std::endl;
    }
    catch (const po::unknown_option& ex)
    {
        M8_ERROR << ex.what() <<'\n'
                 << "To see a list of all options available use \"m8 <command> --help\".";
        std::cerr << ex.what() <<'\n'
                  << "To see a list of all options available use \"m8 <command> --help\"."
                  << std::endl;
    }
    catch (const std::exception& ex)
    {
        M8_FATAL << "Unhandled exception: " << ex.what();
        std::cerr << "Unhandled exception: " << ex.what() <<std::endl;
        return -1;
    }
    catch (...)
    {
        M8_FATAL << "Unhandled exception";
        std::cerr << "Unhandled exception" << std::endl;
        return -2;
    }
   
    return 0;
}
