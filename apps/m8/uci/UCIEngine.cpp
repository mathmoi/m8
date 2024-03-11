/// @file UCIEngine.cpp
/// @author Mathieu Pagé
/// @date February 2024
/// @brief Contains the UCIEngine class implementation

#include <chrono>
#include <exception>
#include <stack>

#include "m8common/options/Options.hpp"

#include "m8chess/Board.hpp"
#include "m8chess/CoordinateNotation.hpp"

#include "UCIEngine.hpp"

namespace m8::uci
{
    UCIEngine::UCIEngine()
    : board_(kStartingPositionFEN, eval::GeneratePieceSqTable()),
      transposition_table_(options::Options::get().tt_size*1024*1024),
      searcher_(transposition_table_)
    {
        searcher_.Attach(this);
    }

    void UCIEngine::HandleUCI()
    {
        interface_.SendId("name", "m8");
        interface_.SendId("author", "Mathieu Pagé <m@mathieupage.com>");
        interface_.SendSpinOption("Hash", 1, 1024*1024, options::Options::get().tt_size);
        interface_.SendCheckOption("UCI_Chess960", false);
        interface_.SendUciok();
    }

    void UCIEngine::HandleIsready()
    {
        interface_.SendReadyok();
    }

    void UCIEngine::HandlePosition(const std::string& fen, const std::vector<std::string>& moves)
    {
        board_ = Board(fen, eval::GeneratePieceSqTable());
        for (const std::string& str_move : moves)
        {
            Move move = ParseCoordinateNotation(str_move, board_);
            board_.Make(move);
        }

        M8_DEBUG <<board_.fen();
    }

    void UCIEngine::HandleGo(std::vector<std::string> search_moves,
                             bool ponder,
                             std::optional<std::chrono::milliseconds> wtime,
                             std::optional<std::chrono::milliseconds> btime,
                             std::optional<std::chrono::milliseconds> winc,
                             std::optional<std::chrono::milliseconds> binc,
                             std::optional<std::uint32_t> moves_to_go,
                             std::optional<std::uint32_t> depth,
                             std::optional<std::uint64_t> nodes,
                             std::optional<std::uint32_t> mate,
                             std::optional<std::chrono::milliseconds> move_time,
                             bool infinite)
    {
        auto time = board_.side_to_move() == kWhite ? wtime : btime;
        auto inc  = board_.side_to_move() == kWhite ? winc  : binc;
        auto time_manager = std::make_unique<TimeManager>(time, inc, moves_to_go, move_time, infinite);

        auto search = std::make_shared<search::Search>(board_, std::move(time_manager), 100);
        searcher_.Start(search);
    }

    void UCIEngine::HandleStop()
    {
        searcher_.Stop();
    }

    void UCIEngine::HandleSetOption(std::string_view name,
                                    std::optional<std::string_view> value)
    {
        auto& modifiable_options = options::Options::get().modifiable_options;
        auto it = modifiable_options.find(std::string(name));
        if (it != modifiable_options.end())
        {
            it->second->set_value(*value);
        }

        // We resize the transposition table here in case it was resize by the option
        // command. This will have no effect if the size remains the same.
        transposition_table_.Resize(options::Options::get().tt_size * 1024 * 1024);
    }

    void UCIEngine::OnNewBestMove(const search::PV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes)
    {
        if (time > 0.01 && pv.count() > 1)
        {
            interface_.SendInfo(depth,
                                std::nullopt,
                                std::chrono::milliseconds(static_cast<int>(time * 1000)),
                                nodes,
                                RenderPVMoves(pv),
                                std::nullopt,
                                eval,
                                std::nullopt,
                                std::nullopt,
                                std::nullopt,
                                nodes / time,
                                std::nullopt,
                                std::nullopt,
                                std::nullopt);
        }
    }

    void UCIEngine::OnSearchMoveAtRoot(DepthType depth, double time, std::uint16_t move_number, std::uint16_t moves_number, NodeCounterType nodes, Move move)
    {
        if (time > 1)
        {   
            interface_.SendInfo(depth,
                                std::nullopt,
                                std::chrono::milliseconds(static_cast<int>(time * 1000)),
                                nodes,
                                std::nullopt,
                                std::nullopt,
                                std::nullopt,
                                RenderCoordinateNotation(move, board_, options::Options::get().chess960),
                                move_number,
                                std::nullopt,
                                nodes / time,
                                std::nullopt,
                                std::nullopt,
                                std::nullopt);
        }
    }

    void UCIEngine::OnSearchCompleted(const search::PV& pv, double time, const search::SearchStats& stats)
    {
        auto move = RenderCoordinateNotation(pv.first(), board_, options::Options::get().chess960);
        interface_.SendBestMove(move, std::nullopt);
    }

    void UCIEngine::OnIterationCompleted(const search::PV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes)
    {
        if (time > 0.1 || pv.count() > 1)
        {
            interface_.SendInfo(depth,
                                std::nullopt,
                                std::chrono::milliseconds(static_cast<int>(time * 1000)),
                                nodes,
                                RenderPVMoves(pv),
                                std::nullopt,
                                eval,
                                std::nullopt,
                                std::nullopt,
                                std::nullopt,
                                nodes / time,
                                std::nullopt,
                                std::nullopt,
                                std::nullopt);
        }
    }

    std::vector<std::string> UCIEngine::RenderPVMoves(const search::PV& pv)
    {
        std::vector<std::string> moves;
        std::stack<UnmakeInfo> unmake_info_stack;

        for (auto move : pv)
        {
            std::string str_move = RenderCoordinateNotation(move, board_, options::Options::get().chess960);
            moves.push_back(str_move);
            unmake_info_stack.push(board_.Make(move));
        }

        while (unmake_info_stack.size() > 0)
        {
            board_.Unmake(pv[unmake_info_stack.size() - 1], unmake_info_stack.top());
            unmake_info_stack.pop();
        }
        
        return moves;
    }
}
