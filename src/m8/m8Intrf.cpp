/// @file   m8Intrf.cpp
/// @author Mathieu Pag�
/// @date   May 2015
/// @brief  Contains the class m8Intrf that implements m8's shell like interface.

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

#include "m8Intrf.hpp"
#include "options/Options.hpp"
#include "../m8common/stringHelpers.hpp"
#include "../m8common/Utils.hpp"
#include "../m8common/logging.hpp"
#include "../m8common/console.hpp"

// This macro can be used in the m8Intrf class to output both to std::cout and to the log system.
#define M8_OUT_LINE(p)   std::cout p <<std::endl; M8_OUTPUT p;
#define M8_EMPTY_LINE()  std::cout <<std::endl;

namespace m8
{
    m8Intrf::m8Intrf()
        : engine_(),
		  xboard_(false),
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
		shell_intrf_.AddCmd(ShellCmd("go",
			"Force the engine to play the side to move and to start playing",
			"go",
			std::bind(&m8Intrf::HandleGo, this)));
        shell_intrf_.AddCmd(ShellCmd("stop",
            "Stops the current operation",
            "stop",
            std::bind(&m8Intrf::HandleStop, this)));
		shell_intrf_.AddCmd(ShellCmd("xboard",
			"Enable xboard mode",
			"xboard",
			std::bind(&m8Intrf::HandleXboard, this)));
		shell_intrf_.AddCmd(ShellCmd("new",
			"Start a new game",
			"new",
			std::bind(&m8Intrf::HandleNew, this)));
    }

	void m8Intrf::SetupXboardMode()
	{
		xboard_ = true;
		shell_intrf_.set_invit("");

		shell_intrf_.AddCmd(ShellCmd("protover", 
			"Indicate to the engine the xboard protocol version",
			"protover {vesion}",
			std::bind(&m8Intrf::HandleProtover, this, std::placeholders::_1)));

		shell_intrf_.AddCmd(ShellCmd("accepted",
			"Indicate to the engine that a feature was accepted",
			"protover {Feature name}",
			std::bind(&m8Intrf::HandleAccepted, this)));


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
        std::lock_guard<std::mutex> lock(output_mutex_);

        M8_EMPTY_LINE();
        M8_OUT_LINE(<< engine_.board());
        M8_EMPTY_LINE();
    }

    void m8Intrf::HandleFen(std::vector<std::string> args_list)
    {
        if (args_list.size() == 1)
        {
            std::lock_guard<std::mutex> lock(output_mutex_);
            M8_OUT_LINE(<<' ' << engine_.board().fen());
        }
        else
        {
            std::string fen = join(args_list.begin() + 1, args_list.end(), " ");

            try
            {
                CallEngineCommand([this, fen]() { engine_.set_fen(fen); }, "fen");
            }
            catch (InvalFenError)
            {
                std::lock_guard<std::mutex> lock(output_mutex_);
                M8_OUT_LINE(<< "Invalid fen string.");
            }
        }
    }

    void m8Intrf::HandlePerft(std::vector<std::string> args_list)
    {
        // Check number of arguments
        if (args_list.size() != 2)
        {
            std::lock_guard<std::mutex> lock(output_mutex_);
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
            std::lock_guard<std::mutex> lock(output_mutex_);
            M8_OUT_LINE(<< "Usage : perft {Depth}");
            return;
        }

        if (depth < 1 || 255 < depth)
        {
            std::lock_guard<std::mutex> lock(output_mutex_);
            M8_OUT_LINE(<<"The depth must be between 1 and 255");
        }
        else
        {
            CallEngineCommand([this, depth]()
                              { engine_.Perft(depth,
                                              std::bind(&m8Intrf::DisplayPerftPartialResult, this, std::placeholders::_1, std::placeholders::_2),
                                              std::bind(&m8Intrf::DisplayPerftResult, this, std::placeholders::_1, std::placeholders::_2)); },
                              "perft");
        }
    }

    void m8Intrf::HandleOptions() const
    {
        std::lock_guard<std::mutex> lock(output_mutex_);

        M8_EMPTY_LINE();

        for (auto& pair : Options::get().modifiable_options_map())
        {
            M8_OUT_LINE(<<pair.second->name() <<'=' <<pair.second->ToString());
        }   

        M8_EMPTY_LINE();
    }

    void m8Intrf::DisplayOption(const Option& option) const
    {
        std::lock_guard<std::mutex> lock(output_mutex_);

        M8_EMPTY_LINE();
        M8_OUT_LINE(<<"Option name: " << option.name());
        M8_OUT_LINE(<<"Description: " << option.description());
        M8_OUT_LINE(<<"Value: " << option.ToString());
        M8_EMPTY_LINE();
    }

    void m8Intrf::DisplayOption(const std::string& option_name) const
    {
        auto& options = Options::get();
        auto it = options.modifiable_options_map().find(option_name);
        if (it != options.modifiable_options_map().cend())
        {
            DisplayOption(*it->second);
        }
        else
        {
            std::lock_guard<std::mutex> lock(output_mutex_);

            M8_OUT_LINE(<< "Option \"" << option_name << "\" does not exist.");
        }
    }

    void m8Intrf::EditOption(const std::string& option_name, const std::string& value) const
    {
        auto& options = Options::get();
        auto it = options.modifiable_options_map().find(option_name);
        if (it != options.modifiable_options_map().end())
        {
            it->second->set_value(value);
        }
        else
        {
            std::lock_guard<std::mutex> lock(output_mutex_);

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
            std::lock_guard<std::mutex> lock(output_mutex_);
            M8_OUT_LINE(<< "Usage : option {name} [value]");
        }
    }

	void m8Intrf::HandleXboard()
	{
		SetupXboardMode();
		std::cout << std::endl;
	}

	void m8Intrf::HandleProtover(std::vector<std::string> args_list)
	{
        std::lock_guard<std::mutex> lock(output_mutex_);
		M8_OUT_LINE(<< "feature myname=\"m8 0.1\"");
		M8_OUT_LINE(<< "feature done=1");
	}

	void m8Intrf::HandleGo()
	{
        // TODO : Reimplement this

        /*
		// If the engine is already searching, do nothing.
		if (engine_.state() != engine::EngineStateEnum::Searching)
		{
			if (engine_.state() != engine::EngineStateEnum::Ready)
			{
				M8_OUT_LINE(<< "The engine is not ready");
			}
			else
			{
				engine_.Go();
			}
		}
        */
	}

    void m8Intrf::HandleStop()
    {
        CallEngineCommand([this]() {engine_.Stop(); }, "stop");
    }

	void m8Intrf::HandleNew()
	{
        // TODO : Continue here
		//engine_.New();
	}

	void m8Intrf::HandleAccepted()
	{
		// Do nothing
	}

	void m8Intrf::DisplayEngineMove(const std::string& move)
	{
        std::lock_guard<std::mutex> lock(output_mutex_);
		ClearLine();
		M8_OUT_LINE(<< " m8 plays " << move);
		shell_intrf_.DisplayInvit();
	}

    void m8Intrf::DisplayPerftPartialResult(std::string move, std::uint64_t count)
    {
        std::lock_guard<std::mutex> lock(output_mutex_);

        ClearLine();
        M8_OUT_LINE(<< ' ' << move << '\t' << count);
    }

    void m8Intrf::DisplayPerftResult(std::uint64_t count, double seconds)
    {
        std::lock_guard<std::mutex> lock(output_mutex_);
        ClearLine();
        M8_OUT_LINE(<< std::endl
                    << " Nodes: " << count << std::endl
                    << " Time : " << seconds << std::endl
                    << " Nodes per second: " << AddMetricSuffix(static_cast<std::uint64_t>(count / seconds), 3) << std::endl);
        shell_intrf_.DisplayInvit();
    }

    void m8Intrf::CallEngineCommand(std::function<void()> call, const std::string& command)
    {
        try {
            call();
        }
        catch (engine::InvalidEngineCommandException ex)
        {
            std::lock_guard<std::mutex> lock(output_mutex_);
            M8_OUT_LINE(<< "Error (" << ex.what() << "): " <<command);
        }
    }

	void m8Intrf::ClearLine()
	{
		auto width = GetConsoleWidth();
		std::cout << '\r';
		for (auto x = 0; x < width; ++x)
		{
			std::cout << ' ';
		}
		std::cout << '\r';
	}
}