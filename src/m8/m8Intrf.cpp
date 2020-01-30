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
#include "engine/InvalidMoveException.hpp"
#include "../m8chess/SAN.hpp"
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
        : engine_(eval::GeneratePieceSqTable(),
                  CreateEngineCallbacks(),
                  this),
		  xboard_(false),
          shell_intrf_()
    {
        SetupShellInterf();
    }

    engine::EngineCallbacks m8Intrf::CreateEngineCallbacks()
    {
        engine::EngineCallbacks callbacks;

        callbacks.partial_perft_result_callback = std::bind(&m8Intrf::DisplayPerftPartialResult, this, std::placeholders::_1, std::placeholders::_2);
        callbacks.perft_result_callback = std::bind(&m8Intrf::DisplayPerftResult, this, std::placeholders::_1, std::placeholders::_2);

        return callbacks;
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
        // TODO : Est-ce que cette commande est obsolete, puisqu'on utilise "force"
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
        shell_intrf_.AddCmd(ShellCmd("",
            "Handle a move by the user",
            "",
            std::bind(&m8Intrf::HandleUserMove, this, std::placeholders::_1)));
        shell_intrf_.AddCmd(ShellCmd("usermove",
            "Handle a move by the user",
            "usermove {move}",
            std::bind(&m8Intrf::HandleUserMove, this, std::placeholders::_1)));
        shell_intrf_.AddCmd(ShellCmd("force",
            "Set the engine to play neither color",
            "force",
            std::bind(&m8Intrf::HandleForce, this)));
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
            std::bind([] {})));

        shell_intrf_.AddCmd(ShellCmd("ping",
            "Ask the engine if it's ready to handle more commands",
            "ping {N}",
            std::bind(&m8Intrf::HandlePing, this, std::placeholders::_1)));

        shell_intrf_.AddCmd(ShellCmd("setboard",
            "Set the position of the board as a XFen string",
            "setboard [XFenString]",
            std::bind(&m8Intrf::HandleFen, this, std::placeholders::_1)));

        // TODO : Implement theses commands correctly
        shell_intrf_.AddCmd(ShellCmd("random", "", "", std::bind([] {})));
        shell_intrf_.AddCmd(ShellCmd("post", "", "", std::bind([] {})));
        shell_intrf_.AddCmd(ShellCmd("hard", "", "", std::bind([] {})));
        shell_intrf_.AddCmd(ShellCmd("undo", "", "", std::bind([] {})));
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
        DisplayBoard();
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
            catch (const InvalFenError&)
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
        catch (const BadConvr&)
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
            CallEngineCommand([this, depth]() { engine_.Perft(depth); }, "perft");
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
        M8_OUT_LINE(<< "feature done=0");
		M8_OUT_LINE(<< "feature myname=\"m8 0.1\"");
        M8_OUT_LINE(<< "feature sigint=0");
        M8_OUT_LINE(<< "feature sigterm=0");
        M8_OUT_LINE(<< "feature setboard=1");
        M8_OUT_LINE(<< "feature ping=1");
        M8_OUT_LINE(<< "feature playother=0");
        M8_OUT_LINE(<< "feature colors=0");
        M8_OUT_LINE(<< "feature san=1");
		M8_OUT_LINE(<< "feature done=1");
	}

	void m8Intrf::HandleGo()
	{
        CallEngineCommand([this]() {engine_.Go(); }, "go");
	}

    void m8Intrf::HandleForce()
    {
        CallEngineCommand([this]() {engine_.Force(); }, "force");
    }

    void m8Intrf::HandleStop()
    {
        CallEngineCommand([this]() {engine_.Stop(); }, "stop");
    }

	void m8Intrf::HandleNew()
	{
        CallEngineCommand([this]() {engine_.New(); }, "new");
	}

    void m8Intrf::HandlePing(std::vector<std::string> args_list)
    {
        std::lock_guard<std::mutex> lock(output_mutex_);
        M8_OUT_LINE(<< "pong " <<args_list[1]);
    }

    void m8Intrf::HandleUserMove(std::vector<std::string> args_list)
    {
        std::string move;

        if (args_list.size() == 1)
        {
            move = args_list[0];
        }
        else if (args_list.size() == 2 && args_list[0] == "usermove")
        {
            move = args_list[1];
        }
        else
        {
            std::lock_guard<std::mutex> lock(output_mutex_);
            M8_OUT_LINE(<< "Usage : usermove {move}");
        }

        if (move.length() > 0)
        {
            try
            {
                auto sucess = CallEngineCommand([this, move]() {engine_.UserMove(move); }, move);
                
                if (sucess && Options::get().display_auto())
                {
                    std::lock_guard<std::mutex> lock(output_mutex_);
                    DisplayBoard();
                }
            }
            catch (const engine::InvalidMoveException&)
            {
                M8_OUT_LINE(<< "Illegal move: " << move);
            }
        }
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

    void m8Intrf::DisplayBoard() const
    {
        M8_EMPTY_LINE();
        M8_OUT_LINE(<< engine_.board());

        if (Options::get().display_eval())
        {
            M8_OUT_LINE(<< "Current evaluation: " << engine_.current_evaluation());
        }

        M8_EMPTY_LINE();
    }

    void m8Intrf::DisplaySearchTableHeader() const
    {
        const std::string fixed_column_names(" | depth |   time   | score |   n   |");

        auto console_width = std::max<int>(GetConsoleWidth(), 80);
        auto fixed_columns_width = fixed_column_names.size();
        auto available_width = console_width - fixed_columns_width;
        auto header_width = console_width - 1;
        auto pv_width = available_width - 1;
        auto before_pv = (pv_width - 2) / 2;
        auto after_pv = pv_width - before_pv - 2;

        {
            std::lock_guard<std::mutex> lock(output_mutex_);

            ClearLine();
            M8_OUT_LINE(<< ' ' << std::string(header_width, '-') << std::endl
                << " | pro  |  temps   | score | nodes  |" << std::string(before_pv, ' ') << "pv" << std::string(after_pv, ' ') << "|" << std::endl
                << ' ' << std::string(header_width, '-'));
        }
    }

    void m8Intrf::DisplaySearchTableLine(bool is_iteration_complete, std::string move, EvalType eval, DepthType depth, double time, NodeCounterType nodes) const
    {
        using namespace std;

        if (depth > 3 && time > 0.01)
        {
            std::ostringstream out;

            // display depth
            out << " |" << std::setw(3) << depth;

            if (is_iteration_complete)
            {
                out << "->";
            }
            else
            {
                out << "  ";
            }

            out << " |";

            // display the time
            auto minutes = static_cast<int>(time / 60);
            auto seconds = time - minutes * 60;
            out << setw(3) << minutes << ":"
                << setw(5) << setfill('0') << fixed << setprecision(2) << seconds << setfill(' ') << " |";

            // display evaluation
            out << setw(6) << FormaterEval(eval) << " | ";

            // Display nodes count
            out << setw(6) << AddMetricSuffix(nodes, 1) << " |";

            // Display pv
            auto console_width = std::max<int>(GetConsoleWidth(), 80);
            auto pv_width = console_width - 40;
            out << ' ' << setw(pv_width) << left << move << " |";

            {
                std::lock_guard<std::mutex> lock(output_mutex_);

                ClearLine();
                M8_OUT_LINE(<< out.str());
            }
            
        }
    }

    void m8Intrf::DisplaySearchOutputXboard(std::string move, EvalType eval, DepthType depth, double seconds, NodeCounterType nodes) const
    {
        std::lock_guard<std::mutex> lock(output_mutex_);
        M8_OUT_LINE(<<depth <<' ' <<eval <<' ' <<static_cast<int>(seconds * 100) <<' ' <<move);
    }
    
    void m8Intrf::DisplaySearchTableFooter() const
    {
        auto console_width = std::max<int>(GetConsoleWidth(), 80);
        auto footer_width = console_width - 1;

        {
            std::lock_guard<std::mutex> lock(output_mutex_);
            M8_OUT_LINE(<< ' ' << std::string(footer_width, '-') << std::endl);
        }
        
    }

    std::string m8Intrf::FormaterEval(int eval) const
    {
        using namespace std;

        ostringstream out;

        if (eval < -eval::kEvalMat + eval::kMaxMat)
            out << "-MAT-" << eval - eval::kEvalMat;
        else if (eval > eval::kEvalMat - eval::kMaxMat)
            out << "MAT-" << eval::kEvalMat - eval;
        else
            out << setiosflags(ios::fixed) << setprecision(2) << eval / 100.0f;

        return out.str();
    }

    bool m8Intrf::CallEngineCommand(std::function<void()> call, const std::string& command)
    {
        bool sucess = true;

        try {
            call();
        }
        catch (const engine::InvalidEngineCommandException& ex)
        {
            sucess = false;

            std::lock_guard<std::mutex> lock(output_mutex_);
            M8_OUT_LINE(<< "Error (" << ex.what() << "): " <<command);
        }

        return sucess;
    }

	void m8Intrf::ClearLine() const
	{
		auto width = GetConsoleWidth();
		std::cout << '\r';
		for (auto x = 0; x < width; ++x)
		{
			std::cout << ' ';
		}
		std::cout << '\r';
	}

    void m8Intrf::OnBeginSearch()
    {
        if (!xboard_)
        {
            DisplaySearchTableHeader();
        }
    }

    void m8Intrf::OnNewBestMove(std::string move, EvalType eval, DepthType depth, double time, NodeCounterType nodes)
    {
        if (xboard_)
        {
            DisplaySearchOutputXboard(move, eval, depth, time, nodes);
        }
        else
        {
            DisplaySearchTableLine(false, move, eval, depth, time, nodes);
        }
    }

    void m8Intrf::OnIterationCompleted(std::string move, EvalType eval, DepthType depth, double time, NodeCounterType nodes)
    {
        if (xboard_)
        {
            DisplaySearchOutputXboard(move, eval, depth, time, nodes);
        }
        else
        {
            DisplaySearchTableLine(true, move, eval, depth, time, nodes);
        }
    }

    void m8Intrf::OnSearchCompleted(std::string move, double time)
    {
        DisplaySearchTableFooter();

        if (xboard_)
        {
            std::lock_guard<std::mutex> lock(output_mutex_);
            M8_OUT_LINE(<< "move " << move);
        }
        else
        {
            std::lock_guard<std::mutex> lock(output_mutex_);
            ClearLine();
            M8_OUT_LINE(<< " m8 plays " << move);

            if (Options::get().display_auto())
            {
                DisplayBoard();
            }

            shell_intrf_.DisplayInvit();
        }
    }
}