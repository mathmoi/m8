/// @file UCIEngine.hpp
/// @author Mathieu Pagé
/// @date February 2024
/// @brief Contains the UCIEngine class

#ifndef M8_UCI_UCI_ENGINE_HPP_
#define M8_UCI_UCI_ENGINE_HPP_

#include "m8chess/Board.hpp"
#include "m8chess/search/PV.hpp"
#include "m8chess/search/Searcher.hpp"
#include "m8chess/search/SearchStats.hpp"
#include "m8chess/search/ISearchObserver.hpp"
#include "m8chess/transposition/TranspositionTable.hpp"

#include "UCIInterface.hpp"

namespace m8::uci
{
    class UCIEngine : public search::ISearchObserver<search::PV, Move>
    {
    public:
        /// Constructor
        UCIEngine();

        /// Handles the uci commands from the uci protocol
        void HandleUCI();

        /// Handles the isready command from the uci protocol
        void HandleIsready();

        /// Handles the position command from the uci protocol
        void HandlePosition(const std::string& fen, const std::vector<std::string>& moves);

        /// Handles the go command from the uci protocol
        /// 
        /// @param search_moves restrict search to theses moves only
        /// @param ponder       start searching in pondering mode.
        /// @param wtime        white has x msec left on the clock
        /// @param btime        black has x msec left on the clock
        /// @param winc         white increment per move in mseconds if x > 0
        /// @param binc         black increment per move in mseconds if x > 0
        /// @param moves_to_go  there are x moves to the next time control
        /// @param depth        search x plies only
        /// @param nodes        earch x nodes only
        /// @param mate         search for a mate in x moves
        /// @param move_time    search exactly x mseconds
        /// @param infinite     search until the "stop" command
        void HandleGo(std::vector<std::string> search_moves,
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
                      bool infinite);
        
        /// Handles the stop command from the uci protocol
        void HandleStop();

        /// Handles the setoption command from the uci protocol
        void HandleSetOption(std::string_view name,
                             std::optional<std::string_view> value);

        /// Method called when a new best move is found at the root.
        void OnNewBestMove(const search::PV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes);

        void OnSearchMoveAtRoot(DepthType depth, double time, std::uint16_t move_number, std::uint16_t moves_number, NodeCounterType nodes, Move move);
        
        /// Method when the search is completed.
        void OnSearchCompleted(const search::PV& pv, double time, const search::SearchStats& stats);

        /// Method called when an iteration is completed.
        void OnIterationCompleted(const search::PV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes);
        
    private:
        UCIInterface interface_;
        Board board_;
        transposition::TranspositionTable transposition_table_;
        search::Searcher searcher_;
        std::unique_ptr<time::TimeControl> time_control_;
        std::unique_ptr<time::ChessClock> clock_;

        std::vector<std::string> RenderPVMoves(const search::PV& pv);
        void SetTimeControl(std::optional<std::chrono::milliseconds> wtime,
                            std::optional<std::chrono::milliseconds> btime,
                            std::optional<std::chrono::milliseconds> winc,
                            std::optional<std::chrono::milliseconds> binc,
                            std::optional<std::uint32_t> moves_to_go,
                            std::optional<std::chrono::milliseconds> move_time,
                            bool infinite);
    };
}

#endif // M8_UCI_UCI_ENGINE_HPP_