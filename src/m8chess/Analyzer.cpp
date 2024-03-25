/// @file Analyzer.cpp
/// @author Mathieu Pagé
/// @date March 2024
/// @brief Contains the Analyzer class definitions.

#include <chrono>
#include <csignal>
#include <limits>
#include <stack>

#include "m8common/options/Options.hpp"
#include "m8common/console.hpp"
#include "m8common/Output.hpp"
#include "m8common/Signal.hpp"

#include "m8chess/search/IterativeDeepening.hpp"
#include "m8chess/search/Search.hpp"
#include "m8chess/search/Searcher.hpp"
#include "m8chess/transposition/TranspositionTable.hpp"

#include "m8chess/Board.hpp"
#include "m8chess/SAN.hpp"

#include "m8chess/Analyzer.hpp"

namespace m8
{
    Analyzer::Analyzer(std::string_view fen, DepthType depth, float time)
    : board_(fen, eval::GeneratePieceSqTable()),
      depth_(depth),
      time_(time)
    {
        EnableSignalHandler();
    }

    void Analyzer::Analyze()
    {
        Output out;
        out <<board_ <<std::endl;

        transposition::TranspositionTable transposition_table(options::Options::get().tt_size * 1024 * 1024);
        transposition_table.Empty();

        search::Searcher searcher(transposition_table);
        searcher.Attach(this);

        auto time_manager = CreateTimeManager();

        auto search = std::make_shared<search::Search>(board_, std::move(time_manager), depth_);

        searcher.Run(search);
    }

    std::unique_ptr<TimeManager> Analyzer::CreateTimeManager()
    {
        if (time_ == std::numeric_limits<float>::infinity())
        {
            return std::make_unique<TimeManager>(std::nullopt, std::nullopt, std::nullopt, std::nullopt, true);
        }

        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<float>(time_));
        return std::make_unique<TimeManager>(std::nullopt, std::nullopt, std::nullopt, milliseconds, false);
    }

    void Analyzer::OnSearchStarted()
    {
        const std::string fixed_column_names("| dpth |   time   | score |  nodes  |");

        auto console_width = std::max<int>(GetConsoleWidth(), 80);
        auto fixed_columns_width = fixed_column_names.size();
        auto available_width = console_width - fixed_columns_width;
        auto header_width = console_width;
        auto pv_width = available_width - 1;
        auto before_pv = (pv_width - 2) / 2;
        auto after_pv = pv_width - before_pv - 2;

        Output out;
        out <<'\n' <<std::string(header_width, '-') <<'\n'
            <<fixed_column_names <<std::string(before_pv, ' ') <<"pv" <<std::string(after_pv, ' ') <<"|" <<'\n'
            <<std::string(header_width, '-') <<std::endl;
    }

    void Analyzer::OnNewBestMove(const search::PV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes)
    {
        DisplaySearchTableLine(false, pv, eval, depth, time, nodes);
    }

    void Analyzer::OnSearchMoveAtRoot(DepthType depth, double time, std::uint16_t move_number, std::uint16_t moves_number, NodeCounterType nodes, Move move)
    {
        if (time < 0.25)
        {
            return;
        }
        
        ClearLine();

        // Prepare the pv section
        auto console_width = std::max<int>(GetConsoleWidth(), 80);
        auto pv_width = console_width - 40;

        // display depth
        Output out;
        out << "|" << std::setw(3) << depth <<"...|";

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
        oss <<RenderSAN(move, board_) <<" (" <<AddMetricSuffix(nodes / time, 3) <<"nps)";
        out << ' ' << std::setw(pv_width) << std::left << oss.str() << " |";

        out.flush();
    }

    void Analyzer::OnIterationCompleted(const search::PV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes)
    {
        DisplaySearchTableLine(true, pv, eval, depth, time, nodes);
    }

    void Analyzer::OnSearchCompleted(const search::PV& pv, double time, const search::SearchStats& stats)
    {
        ClearLine();

        auto console_width = std::max<std::uint32_t>(GetConsoleWidth(), 80);
        auto footer_width = console_width;
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

        Output out;
        out << std::string(footer_width, '-') <<std::endl;

        while (stats_view.size())
        {
            auto line = stats_view;
            if (footer_content_width < line.size())
            {
                auto pos_last_space = stats_view.substr(0, footer_content_width).rfind(' ');
                line = line.substr(0, pos_last_space);
            }

            out << "| " <<std::left <<std::setw(footer_content_width) <<line <<std::setw(0) <<" |" <<std::endl;

            stats_view.remove_prefix(line.size());
            stats_view.remove_prefix((std::min)(stats_view.find_first_not_of(' '), stats_view.size()));
        }

        out << std::string(footer_width, '-') <<std::endl;
    }

    void Analyzer::ClearLine()
    {
        Output out;
        auto width = GetConsoleWidth();
        out <<'\r';
        for (auto x = 0; x < width; ++x)
        {
            out <<' ';
        }
        out << '\r';
    }

    std::string Analyzer::FormatTime(double time)
    {
        std::ostringstream out;
        auto minutes = static_cast<int>(time / 60);
        auto seconds = time - minutes * 60;
        out << minutes << ":"
            << std::setw(5) << std::setfill('0') << std::fixed << std::setprecision(2) << seconds;
        return out.str();
    }

    std::string Analyzer::FormaterEval(int eval)
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

    std::vector<std::string> Analyzer::RenderPVMoves(const search::PV& pv)
    {
        std::vector<std::string> moves;
        std::stack<UnmakeInfo> unmake_info_stack;

        for (size_t x = 0; x < pv.count(); ++x)
        {
            std::string str_move = RenderSAN(pv[x], board_);
            moves.push_back(str_move);
            unmake_info_stack.push(board_.Make(pv[x]));
        }

        while (unmake_info_stack.size() > 0)
        {
            board_.Unmake(pv[unmake_info_stack.size() - 1], unmake_info_stack.top());
            unmake_info_stack.pop();
        }
        
        return moves;
    }
    
    std::vector<std::string> Analyzer::JoinsPVMoves(std::vector<std::string>::const_iterator first,
                                                    std::vector<std::string>::const_iterator last,
                                                    size_t max_width)
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

    void Analyzer::DisplaySearchTableLine(bool is_iteration_complete, const search::PV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes)
    {
        using namespace std;

        if (time > 0.05)
        {
            ClearLine();

            // Prepare the pv
            auto console_width = std::max<int>(GetConsoleWidth(), 80);
            auto pv_width = console_width - 40;
            auto pv_san = RenderPVMoves(pv);
            auto pv_str = JoinsPVMoves(pv_san.begin(), pv_san.end(), pv_width - 2);

            // display depth
            Output out;
            out << "|" << std::setw(3) << depth;

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
            out << ' ' << setw(pv_width) << left << pv_str[0] << " |" <<std::endl;

            // Display the other lines of the PV
            for (auto next = pv_str.begin() + 1; next < pv_str.end(); ++next)
            {
                out <<"|      |          |       |        | " << setw(pv_width) << left << *next << " |" <<std::endl;
            }
        }
    }
}