/// @file   m8Intrf.cpp
/// @author Mathieu Pagé
/// @date   May 2015
/// @brief  Contains the class m8Intrf that implements m8's shell like interface.

#include <chrono>
#include <cstdint>
#include <queue>
#include <string>
#include <sstream>
#include <vector>

#include <boost/algorithm/string.hpp>

#include "../m8chess/SAN.hpp"

#include "../m8common/chronoHelpers.hpp"
#include "../m8common/console.hpp"
#include "../m8common/logging.hpp"
#include "../m8common/stringHelpers.hpp"
#include "../m8common/Utils.hpp"

#include "engine/InvalidEngineCommandException.hpp"
#include "engine/InvalidMoveException.hpp"

#include "options/Options.hpp"

#include "m8Intrf.hpp"
#include "version.hpp"

// This macro can be used in the m8Intrf class to output both to std::cout and to the log system.
#define M8_OUT_LINE(p)   std::cout p <<std::endl;        M8_OUTPUT p;
#define M8_OUT(p)        std::cout p; std::cout.flush(); M8_OUTPUT p;
#define M8_EMPTY_LINE()  std::cout   <<std::endl;

namespace m8
{
    m8Intrf::m8Intrf()
        : engine_(eval::GeneratePieceSqTable()),
          xboard_(false),
          shell_intrf_()
    {
        engine_.Attach(this);
        SetupShellInterf();
    }

    void m8Intrf::Execute()
    {
        M8_OUT_LINE(<<"m8 " <<kM8Version);
        M8_OUT_LINE(<<"Author: Mathieu Pagé <m@mathieupage.com>");
        M8_OUT_LINE(<<"Url: https://www.mathieupage.com");

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
        shell_intrf_.AddCmd(ShellCmd("st",
            "Set the exact number of seconds to use for each move",
            "st {seconds}",
            std::bind(&m8Intrf::HandleSt, this, std::placeholders::_1)));
        shell_intrf_.AddCmd(ShellCmd("sd",
            "Set the maximum depth the engine should search",
            "sd {depth}",
            std::bind(&m8Intrf::HandleSd, this, std::placeholders::_1)));
        shell_intrf_.AddCmd(ShellCmd("level",
            "Set the time control in conventional or incremental mode",
            "level {moves} {base} {increment}",
            std::bind(&m8Intrf::HandleLevel, this, std::placeholders::_1)));
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
            "accepted {Feature name}",
            std::bind([] {})));

        shell_intrf_.AddCmd(ShellCmd("rejected",
            "Indicate to the engine that a feature was rejected",
            "rejected {Feature name}",
            std::bind(&m8Intrf::HandleRejected, this, std::placeholders::_1)));

        shell_intrf_.AddCmd(ShellCmd("ping",
            "Ask the engine if it's ready to handle more commands",
            "ping {N}",
            std::bind(&m8Intrf::HandlePing, this, std::placeholders::_1)));

        shell_intrf_.AddCmd(ShellCmd("setboard",
            "Set the position of the board as a XFen string",
            "setboard [XFenString]",
            std::bind(&m8Intrf::HandleFen, this, std::placeholders::_1)));

        // TODO : Implement theses commands correctly
        shell_intrf_.AddCmd(ShellCmd("random",   "", "", std::bind([] {})));
        shell_intrf_.AddCmd(ShellCmd("post",     "", "", std::bind([] {})));
        shell_intrf_.AddCmd(ShellCmd("hard",     "", "", std::bind([] {})));
        shell_intrf_.AddCmd(ShellCmd("easy",     "", "", std::bind([] {})));
        shell_intrf_.AddCmd(ShellCmd("undo",     "", "", std::bind([] {})));
        shell_intrf_.AddCmd(ShellCmd("time",     "", "", std::bind([] {})));
        shell_intrf_.AddCmd(ShellCmd("otim",     "", "", std::bind([] {})));
        shell_intrf_.AddCmd(ShellCmd("computer", "", "", std::bind([] {})));
    }

    void m8Intrf::HandleExit()
    {
        engine_.Stop();
        shell_intrf_.set_abort(true);
    }

    void m8Intrf::HandleHelp() const
    {
        shell_intrf_.DisplayHelp();
    }

    void m8Intrf::HandleDisplay() const
    {
        std::lock_guard<std::recursive_mutex> lock(output_mutex_);
        DisplayBoard();
    }

    void m8Intrf::HandleFen(std::vector<std::string> args_list)
    {
        if (args_list.size() == 1)
        {
            std::lock_guard<std::recursive_mutex> lock(output_mutex_);
            M8_OUT_LINE(<<' ' << engine_.board().fen());
        }
        else
        {
            std::string fen = join(args_list.begin() + 1, args_list.end(), " ");

            try
            {
                CallEngineCommand([this, fen]() { engine_.SetBoard(fen); }, "fen");
            }
            catch (const InvalFenError&)
            {
                std::lock_guard<std::recursive_mutex> lock(output_mutex_);
                M8_OUT_LINE(<< "Invalid fen string.");
            }
        }
    }

    void m8Intrf::HandlePerft(std::vector<std::string> args_list)
    {
        // Check number of arguments
        if (args_list.size() != 2)
        {
            std::lock_guard<std::recursive_mutex> lock(output_mutex_);
            M8_OUT_LINE(<< "Usage : perft {Depth}");
            return;
        }

        // Convert argument to integer
        auto depth = ConvertArgument<int>(args_list[1], "Usage : perft {Depth}");
        if (depth.has_value())
        {
            if (depth.value() < 1 || 255 < depth.value())
            {
                std::lock_guard<std::recursive_mutex> lock(output_mutex_);
                M8_OUT_LINE(<<"The depth must be between 1 and 255");
            }
            else
            {
                CallEngineCommand([this, depth]() { engine_.Perft(depth.value(), this); }, "perft");
            }
        }
    }

    void m8Intrf::HandleOptions() const
    {
        std::lock_guard<std::recursive_mutex> lock(output_mutex_);

        M8_EMPTY_LINE();

        for (auto& pair : options::Options::get().modifiable_options)
        {
            M8_OUT_LINE(<<pair.second->name() <<'=' <<pair.second->ToString());
        }   

        M8_EMPTY_LINE();
    }

    void m8Intrf::DisplayOption(const options::ModifiableOption& option) const
    {
        std::lock_guard<std::recursive_mutex> lock(output_mutex_);

        M8_EMPTY_LINE();
        M8_OUT_LINE(<<"Option name: " << option.name());
        M8_OUT_LINE(<<"Description: " << option.description());
        M8_OUT_LINE(<<"Value: " << option.ToString());
        M8_EMPTY_LINE();
    }

    void m8Intrf::DisplayOption(const std::string& option_name) const
    {
        auto& modifiable_options = options::Options::get().modifiable_options;
        auto it = modifiable_options.find(option_name);
        if (it != modifiable_options.cend())
        {
            DisplayOption(*it->second);
        }
        else
        {
            std::lock_guard<std::recursive_mutex> lock(output_mutex_);

            M8_OUT_LINE(<< "Option \"" << option_name << "\" does not exist.");
        }
    }

    void m8Intrf::EditOption(const std::string& option_name, const std::string& value) const
    {
        auto& modifiable_options = options::Options::get().modifiable_options;
        auto it = modifiable_options.find(option_name);
        if (it != modifiable_options.end())
        {
            it->second->set_value(value);
        }
        else
        {
            std::lock_guard<std::recursive_mutex> lock(output_mutex_);

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
            std::lock_guard<std::recursive_mutex> lock(output_mutex_);
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
        std::lock_guard<std::recursive_mutex> lock(output_mutex_);
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

    void m8Intrf::HandleRejected(std::vector<std::string> args_list)
    {
        if (args_list[1] == "san")
        {
            options::Options::get().use_san = false;
        }
    }

    void m8Intrf::HandleGo()
    {
        CallEngineCommand([this]() {engine_.Go(); }, "go");
    }

    void m8Intrf::HandleForce()
    {
        CallEngineCommand([this]() {engine_.Force(); }, "force");
    }

    void m8Intrf::HandleSt(std::vector<std::string> args_list)
    {
        // Check number of arguments
        if (args_list.size() != 2)
        {
            std::lock_guard<std::recursive_mutex> lock(output_mutex_);
            M8_OUT_LINE(<< "Usage : st {seconds}");
            return;
        }

        // Convert argument to float
        auto seconds = ConvertArgument<float>(args_list[1], "Usage : st {seconds}");
        if (seconds.has_value())
        {
            time::ChessClock::Duration time_per_move = FloatToNanoseconds(seconds.value());
            CallEngineCommand([this, time_per_move]() {engine_.SetTimeControl(time_per_move); }, "st");
        }
    }

    void m8Intrf::HandleSd(std::vector<std::string> args_list)
    {
        const std::string kUsage("Usage : sd {depth}");

        // Check number of arguments
        if (args_list.size() != 2)
        {
            std::lock_guard<std::recursive_mutex> lock(output_mutex_);
            M8_OUT_LINE(<< kUsage);
            return;
        }

        auto depth = ConvertArgument<DepthType>(args_list[1], kUsage);

        if (depth.has_value())
        {
            CallEngineCommand([this, depth]() {engine_.SetDepth(depth.value()); }, "sd");
        }
    }

    void m8Intrf::HandleLevel(std::vector<std::string> args_list)
    {
        const std::string kUsage = "Usage : level {moves} {base} {increment}";

        // Check number of arguments
        if (args_list.size() != 4)
        {
            std::lock_guard<std::recursive_mutex> lock(output_mutex_);
            M8_OUT_LINE(<< kUsage);
            return;
        }

        // The first arguments is the number of move in a control
        auto moves = ConvertArgument<std::uint32_t>(args_list[1], kUsage);
        if (!moves.has_value())
        {
            return;
        }
        
        // The base argument is either an integer representing the number of
        // minutes or two integers separated by a colon representing minutes and
        // seconds.
        std::vector<std::string> base_tokens;
        boost::split(base_tokens, args_list[2], boost::is_any_of(":"), boost::token_compress_on);

        if (2 < base_tokens.size())
        {
            std::lock_guard<std::recursive_mutex> lock(output_mutex_);
            M8_OUT_LINE(<< kUsage);
        }

        auto base_minutes = ConvertArgument<std::uint32_t>(base_tokens[0], kUsage);
        if (!base_minutes.has_value())
        {
            return;
        }
        std::chrono::seconds base = std::chrono::minutes(base_minutes.value());

        if (base_tokens.size() == 2)
        {
            auto base_seconds = ConvertArgument<std::uint32_t>(base_tokens[1], kUsage);
            if (!base_seconds.has_value())
            {
                return;
            }

            base += std::chrono::seconds(base_seconds.value());
        }

        // The last argument is the increment in seconds
        auto increment_seconds = ConvertArgument<float>(args_list[3], kUsage);
        if (!increment_seconds.has_value())
        {
            return;
        }
        auto increment = FloatToNanoseconds(increment_seconds.value());

        // We cannot have both a number of moves and an increment
        if (increment.count() > 0 && moves > 0)
        {
            std::lock_guard<std::recursive_mutex> lock(output_mutex_);
            M8_OUT_LINE(<< kUsage <<" (moves and increment can not both be greater than zero)");
            return;
        }

        if (moves.value() == 0)
        {
            CallEngineCommand([this, base, increment]() {engine_.SetTimeControl(base, increment); }, "level");
        }
        else
        {
            CallEngineCommand([this, moves, base]() {engine_.SetTimeControl(moves.value(), base); }, "st");
        }
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
        std::lock_guard<std::recursive_mutex> lock(output_mutex_);
        M8_OUT_LINE(<< "pong " <<args_list[1]);
    }

    void m8Intrf::HandleUserMove(std::vector<std::string> args_list)
    {
        M8_TRACE <<"m8Intrf::HandleUserMove(std::vector<std::string> args_list)";

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
            std::lock_guard<std::recursive_mutex> lock(output_mutex_);
            M8_OUT_LINE(<< "Usage : usermove {move}");
        }

        if (move.length() > 0)
        {
            try
            {
                auto sucess = CallEngineCommand([this, move]() {engine_.UserMove(move); }, move);
                
                if (sucess && !xboard_ && options::Options::get().display_auto)
                {
                    std::lock_guard<std::recursive_mutex> lock(output_mutex_);
                    DisplayBoard();
                }
            }
            catch (const engine::InvalidMoveException&)
            {
                M8_OUT_LINE(<< "Illegal move: " << move);
            }
        }
    }

    void m8Intrf::OnPartialPerftResult(const std::string& move, std::uint64_t count)
    {
        std::lock_guard<std::recursive_mutex> lock(output_mutex_);

        ClearLine();
        M8_OUT_LINE(<< ' ' << move << '\t' << count);
    }

    void m8Intrf::OnPerftCompleted(std::uint64_t count, double time)
    {
        std::lock_guard<std::recursive_mutex> lock(output_mutex_);
        ClearLine();
        M8_OUT_LINE(<< std::endl
                    << " Nodes: " << count << std::endl
                    << " Time : " << time << std::endl
                    << " Nodes per second: " << AddMetricSuffix(static_cast<std::uint64_t>(count / time), 3) << std::endl);
        shell_intrf_.DisplayInvit();
    }

    void m8Intrf::DisplayBoard() const
    {
        M8_EMPTY_LINE();
        M8_OUT_LINE(<< engine_.board());

        if (options::Options::get().display_eval)
        {
            M8_OUT_LINE(<< "Current evaluation: " << engine_.current_evaluation());
        }

        M8_EMPTY_LINE();
    }

    void m8Intrf::DisplaySearchTableHeader() const
    {
        const std::string fixed_column_names(" | dpth |   time   | score |  nodes  |");

        auto console_width = std::max<int>(GetConsoleWidth(), 80);
        auto fixed_columns_width = fixed_column_names.size();
        auto available_width = console_width - fixed_columns_width;
        auto header_width = console_width - 1;
        auto pv_width = available_width - 1;
        auto before_pv = (pv_width - 2) / 2;
        auto after_pv = pv_width - before_pv - 2;

        {
            std::lock_guard<std::recursive_mutex> lock(output_mutex_);

            ClearLine();
            M8_OUT_LINE(<< ' ' << std::string(header_width, '-') << std::endl
                << fixed_column_names << std::string(before_pv, ' ') << "pv" << std::string(after_pv, ' ') << "|" << std::endl
                << ' ' << std::string(header_width, '-'));
        }
    }

    /// Join the string from an iterator pair by separating them by a space.
    std::vector<std::string> m8Intrf::JoinsPVMoves(std::vector<std::string>::const_iterator first,
                                                   std::vector<std::string>::const_iterator last,
                                                   size_t max_width) const
    {
        std::vector<std::string> result;
        size_t current_width = 0;
        std::ostringstream out;

        for (auto next = first; next < last; ++next)
        {
            if (next != first)
            {
                if (current_width + 1 + next->length() > max_width)
                {
                    result.push_back(out.str());
                    out.str("");
                    current_width = 0;
                }
                else
                {
                    out << ' ';
                    ++current_width;
                }
            }

            out << *next;
            current_width += next->length();
        }

        result.push_back(out.str());

        return result;
    }

    void m8Intrf::DisplaySearchTableLine(bool is_iteration_complete, const std::vector<std::string>& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes) const
    {
        using namespace std;

        if (options::Options::get().min_display_depth <= depth && time > 0.01)
        {
            std::lock_guard<std::recursive_mutex> lock(output_mutex_);
            std::ostringstream out;

            ClearLine();

            // Prepare the pv
            auto console_width = std::max<int>(GetConsoleWidth(), 80);
            auto pv_width = console_width - 41;
            auto pv_str = JoinsPVMoves(pv.begin(), pv.end(), pv_width - 2);

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
            out << std::setw(9) << FormatTime(time) << " |";

            // display evaluation
            out << setw(6) << FormaterEval(eval) << " | ";

            // Display nodes count
            out << setw(7) << AddMetricSuffix(nodes, 2) << " |";

            // Display pv
            out << ' ' << setw(pv_width) << left << pv_str[0] << " |";

            M8_OUT_LINE(<< out.str());

            // Display the other lines of the PV
            for (auto next = pv_str.begin() + 1; next < pv_str.end(); ++next)
            {
                M8_OUT_LINE( << " |      |          |       |        | " << setw(pv_width) << left << *next << " |");
            }
            
        }
    }

    void m8Intrf::DisplaySearchOutputXboard(const std::vector<std::string>& pv, EvalType eval, DepthType depth, double seconds, NodeCounterType nodes) const
    {
        if (options::Options::get().min_display_depth <= depth && seconds > 0.01)
        {
            std::lock_guard<std::recursive_mutex> lock(output_mutex_);

            std::ostringstream out;
            auto pv_str = JoinsPVMoves(pv.begin(), pv.end());
            out << depth << ' ' << eval << ' ' << static_cast<int>(seconds * 100) << ' ' << nodes << ' ' <<pv_str[0];

            M8_OUT_LINE(<< out.str());
        }
    }
    
    void m8Intrf::DisplaySearchTableFooter(double time, const search::SearchStats& stats) const
    {
        auto console_width = std::max<std::uint32_t>(GetConsoleWidth(), 80);
        auto footer_width = console_width - 1;
        auto footer_content_width = footer_width - 4;

        std::ostringstream oss;

        oss <<"time=" <<FormatTime(time)
            <<" nodes=" <<AddMetricSuffix(stats.nodes, 3)
            <<" qnodes=" <<AddMetricSuffix(stats.qnodes, 3)
            <<" nps=" <<AddMetricSuffix((stats.nodes + stats.qnodes) / time, 3)
            <<" tt_probes=" <<AddMetricSuffix(stats.tt_probes, 3)
            <<" tt_hits=" <<FormatPercentage(stats.tt_hits / static_cast<float>(stats.tt_probes), 1)
            <<" tt_exact=" <<FormatPercentage(stats.tt_hits_exact 
                                              / static_cast<float>(stats.tt_hits_exact
                                                                   + stats.tt_hits_lower
                                                                   + stats.tt_hits_upper), 1)
            <<" tt_upper=" <<FormatPercentage(stats.tt_hits_upper 
                                              / static_cast<float>(stats.tt_hits_exact
                                                                   + stats.tt_hits_lower
                                                                   + stats.tt_hits_upper), 1)
            <<" tt_lower=" <<FormatPercentage(stats.tt_hits_lower 
                                              / static_cast<float>(stats.tt_hits_exact
                                                                   + stats.tt_hits_lower
                                                                   + stats.tt_hits_upper), 1)
            <<" tt_cut=" <<FormatPercentage((stats.tt_hits_exact + stats.tt_hits_lower + stats.tt_hits_upper)
                                            / static_cast<float>(stats.tt_probes), 1);

        std::string stats_str = std::move(oss).str();
        std::string_view stats_view { stats_str };
        {
            std::lock_guard<std::recursive_mutex> lock(output_mutex_);
            M8_OUT_LINE(<< ' ' << std::string(footer_width, '-'));

            while (stats_view.size())
            {
                auto line = stats_view;
                if (footer_content_width < line.size())
                {
                    auto pos_last_space = stats_view.substr(0, footer_content_width).rfind(' ');
                    line = line.substr(0, pos_last_space);
                }

                M8_OUT_LINE(<< " | " <<std::left <<std::setw(footer_content_width) <<line <<std::setw(0) <<" |");

                stats_view.remove_prefix(line.size());
                stats_view.remove_prefix(std::min(stats_view.find_first_not_of(' '), stats_view.size()));
            }

            M8_OUT_LINE(<< ' ' << std::string(footer_width, '-') <<std::endl);
        }
    }

    std::string m8Intrf::FormaterEval(int eval) const
    {
        using namespace std;

        ostringstream out;

        if (eval < -eval::kEvalMat + eval::kMaxMat)
            out << "-MAT-" << eval + eval::kEvalMat;
        else if (eval > eval::kEvalMat - eval::kMaxMat)
            out << "MAT-" << eval::kEvalMat - eval;
        else
            out << setiosflags(ios::fixed) << setprecision(2) << eval / 100.0f;

        return out.str();
    }

    std::string m8Intrf::FormatTime(double time) const
    {
        std::ostringstream out;
        auto minutes = static_cast<int>(time / 60);
        auto seconds = time - minutes * 60;
        out << minutes << ":"
            << std::setw(5) << std::setfill('0') << std::fixed << std::setprecision(2) << seconds;
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

            std::lock_guard<std::recursive_mutex> lock(output_mutex_);
            M8_OUT_LINE(<< "Error (" << ex.what() << "): " <<command);
        }

        return sucess;
    }

    template<typename T>
    std::optional<T> m8Intrf::ConvertArgument(const std::string& arg, const std::string& usage)
    {
        try
        {
            return ConvertTo<T>(arg);
        }
        catch (const BadConvr&)
        {
            std::lock_guard<std::recursive_mutex> lock(output_mutex_);
            M8_OUT_LINE(<< usage);
            return std::nullopt;
        }
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

    void m8Intrf::OnSearchStarted()
    {
        if (!xboard_)
        {
            DisplaySearchTableHeader();
        }
    }

    void m8Intrf::OnSearchMoveAtRoot(DepthType depth, double time, std::uint16_t move_number, std::uint16_t moves_number, NodeCounterType nodes, std::string move)
    {
        if (xboard_
            || depth < options::Options::get().min_display_depth
            || time < 0.25)
        {
            return;
        }
        
        std::lock_guard<std::recursive_mutex> lock(output_mutex_);
        std::ostringstream out;

        ClearLine();

        // Prepare the pv
        auto console_width = std::max<int>(GetConsoleWidth(), 80);
        auto pv_width = console_width - 41;

        // display depth
        out << " |" << std::setw(3) << depth <<"...|";

        // display the time
        out << std::setw(9) << FormatTime(time) << " |";

        // display the move number instead of the evaluation
        std::ostringstream oss;
        oss <<move_number <<'/' <<moves_number;
        out << std::setw(6) << oss.str() << " | ";

        // Display nodes count
        out << std::setw(7) << AddMetricSuffix(nodes, 2) << " |";

        // Display the move and nps.
        oss.str("");
        oss <<move <<" (" <<AddMetricSuffix(nodes / time, 3) <<"nps)";
        out << ' ' << std::setw(pv_width) << std::left << oss.str() << " |";

        M8_OUT(<< out.str());
    }

    void m8Intrf::OnNewBestMove(const std::vector<std::string>& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes)
    {
        if (xboard_)
        {
            DisplaySearchOutputXboard(pv, eval, depth, time, nodes);
        }
        else
        {
            DisplaySearchTableLine(false, pv, eval, depth, time, nodes);
        }
    }

    void m8Intrf::OnIterationCompleted(const std::vector<std::string>& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes)
    {
        if (xboard_)
        {
            DisplaySearchOutputXboard(pv, eval, depth, time, nodes);
        }
        else
        {
            DisplaySearchTableLine(true, pv, eval, depth, time, nodes);
        }
    }

    void m8Intrf::OnSearchCompleted(const std::vector<std::string> &pv, double time, const search::SearchStats& stats)
    {
        if (xboard_)
        {
            std::lock_guard<std::recursive_mutex> lock(output_mutex_);
            M8_OUT_LINE(<< "move " << *pv.begin());
        }
        else
        {
            std::lock_guard<std::recursive_mutex> lock(output_mutex_);
            ClearLine();

            DisplaySearchTableFooter(time, stats);
            M8_OUT_LINE(<< " m8 plays " << *pv.begin());

            if (options::Options::get().display_auto)
            {
                DisplayBoard();
            }

            shell_intrf_.DisplayInvit();
        }
    }
}