/// @file   m8Intrf.cpp
/// @author Mathieu Pag�
/// @date   May 2015
/// @brief  Contains the class m8Intrf that implements m8's shell like interface.

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

#include "m8Intrf.hpp"
#include "../m8common/stringHelpers.hpp"
#include "../m8common/Utils.hpp"
#include "options/Options.hpp"
#include "../m8common/logging.hpp"

// This macro can be used in the m8Intrf class to output both to std::cout and to the log system.
#define M8_OUT_LINE(p)   std::cout p <<std::endl; M8_OUTPUT p;
#define M8_EMPTY_LINE()  std::cout <<std::endl;

namespace m8
{
    m8Intrf::m8Intrf()
        : engine_(),
          shell_intrf_()
        
    {
        SetupShellInterf();
    }

    void m8Intrf::Execute()
    {
        shell_intrf_.Execute();
    }

    void m8Intrf::SetupShellInterf()
    {
        using namespace std::placeholders;

        shell_intrf_.set_invit(">");

        shell_intrf_.AddCmd(ShellCmd("exit",
            "Exit the application",
            "exit",
            std::bind(&m8Intrf::HandleExit, this)));
        shell_intrf_.AddCmd(ShellCmd("quit",
            "",
            "",
            std::bind(&m8Intrf::HandleExit, this)));
        shell_intrf_.AddCmd(ShellCmd("help",
            "Display a list of commands",
            "help",
            std::bind(&m8Intrf::HandleHelp, this)));
        shell_intrf_.AddCmd(ShellCmd("display",
            "Display the current board",
            "display",
            std::bind(&m8Intrf::HandleDisplay, this)));
        shell_intrf_.AddCmd(ShellCmd("fen",
            "Get or set the position of the board as a XFen string",
            "fen [XFenString]",
            std::bind(&m8Intrf::HandleFen, this, std::placeholders::_1)));
        shell_intrf_.AddCmd(ShellCmd("perft",
            "Execute a perft test",
            "perft {Depth}",
            std::bind(&m8Intrf::HandlePerft, this, std::placeholders::_1)));
        shell_intrf_.AddCmd(ShellCmd("options",
            "Display all the options and their values",
            "options",
            std::bind(&m8Intrf::HandleOptions, this)));
        shell_intrf_.AddCmd(ShellCmd("option",
            "Get or set the value of an option",
            "option {name} [value]",
            std::bind(&m8Intrf::HandleOption, this, std::placeholders::_1)));
    }

    void m8Intrf::HandleExit()
    {
        shell_intrf_.set_abort(true);
    }

    void m8Intrf::HandleHelp() const
    {
        shell_intrf_.DisplayHelp();
    }

    void m8Intrf::HandleDisplay() const
    {
        M8_EMPTY_LINE();
        M8_OUT_LINE(<< engine_.board());
        M8_EMPTY_LINE();
    }

    void m8Intrf::HandleFen(std::vector<std::string> args_list)
    {
        if (args_list.size() == 1)
        {
            M8_OUT_LINE(<<' ' << engine_.board().fen());
        }
        else
        {
            std::string fen = join(args_list.begin() + 1, args_list.end(), " ");

            try
            {
                engine_.set_fen(fen);
            }
            catch (InvalFenError)
            {
                M8_OUT_LINE(<< "Invalid fen string.");
            }
        }
    }

    void m8Intrf::HandlePerft(std::vector<std::string> args_list)
    {
        // Check number of arguments
        if (args_list.size() != 2)
        {
            M8_OUT_LINE(<< "Usage : perft {Depth}");
            return;
        }

        // Convert argument to integer
        int depth;
        try
        {
            depth = ConvertTo<int>(args_list[1]);
        }
        catch (BadConvr exc)
        {
            M8_OUT_LINE(<< "Usage : perft {Depth}");
            return;
        }

        if (depth < 1 || 255 < depth)
        {
            M8_OUT_LINE(<<"The depth must be between 1 and 255");
        }

        M8_EMPTY_LINE();

        auto result = engine_.Perft(depth, [](std::string move, std::uint64_t count) { M8_OUT_LINE(<< ' ' << move << '\t' << count); });

        M8_EMPTY_LINE();
        M8_OUT_LINE(<< " Nodes: " << result.nodes);
        M8_OUT_LINE(<< " Time : " << result.seconds);
        M8_OUT_LINE(<< " Nodes per second: " << AddMetricSuffix(static_cast<std::uint64_t>(result.nodes / result.seconds), 3));
        M8_EMPTY_LINE();
    }

    void m8Intrf::HandleOptions() const
    {
        M8_EMPTY_LINE();

        for (auto& pair : Options::get().map())
        {
            M8_OUT_LINE(<<pair.second.name() <<'=' <<pair.second.ToString());
        }

        M8_EMPTY_LINE();
    }

    void m8Intrf::DisplayOption(const Option& option) const
    {
        M8_EMPTY_LINE();
        M8_OUT_LINE(<<"Option name: " << option.name());
        M8_OUT_LINE(<<"Description: " << option.description());
        M8_OUT_LINE(<<"Value: " << option.ToString());
        M8_EMPTY_LINE();
    }

    void m8Intrf::DisplayOption(const std::string& option_name) const
    {
        auto& options = Options::get();
        auto it = options.map().find(option_name);
        if (it != options.map().cend())
        {
            DisplayOption(it->second);
        }
        else
        {
            M8_OUT_LINE(<< "Option \"" << option_name << "\" does not exist.");
        }
    }

    void m8Intrf::EditOption(const std::string& option_name, const std::string& value) const
    {
        auto& options = Options::get();
        auto it = options.map().find(option_name);
        if (it != options.map().end())
        {
            it->second.set_value(value);
        }
        else
        {
            M8_OUT_LINE(<< "Option \"" << option_name << "\" does not exist.");
        }
    }

    void m8Intrf::HandleOption(std::vector<std::string> args_list) const
    {
        if (args_list.size() == 2)
        {
            DisplayOption(args_list[1]);
        }
        else if (args_list.size() == 3)
        {
            EditOption(args_list[1], args_list[2]);
        }
        else
        {
            M8_OUT_LINE(<< "Usage : option {name} [value]");
        }
    }
}