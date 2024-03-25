/// @file Analyzer.hpp
/// @author Mathieu Pagé
/// @date March 2024
/// @brief Contains the definition of the Analyzer class

#ifndef M8_ANALYZER_HPP_
#define M8_ANALYZER_HPP_

#include <memory>
#include <string>

#include "search/ISearchObserver.hpp"
#include "search/PV.hpp"
#include "Board.hpp"
#include "TimeManager.hpp"
#include "Types.hpp"

namespace m8
{
    class Analyzer : public search::ISearchObserver<search::PV, Move>
    {
    public:
        Analyzer(std::string_view fen, DepthType depth, float time);

        void Analyze();

        //////////////////////////////////////////////////////////////////////// 
        // Implementation of ISearchObserver

        /// Method called when the search starts.
        void OnSearchStarted();

        /// Method called when a new best move is found at the root.
        void OnNewBestMove(const search::PV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes);

        /// Method called when a new moved is searched at the root.
        void OnSearchMoveAtRoot(DepthType depth, double time, std::uint16_t move_number, std::uint16_t moves_number, NodeCounterType nodes, Move move);

        /// Method called when an iteration is completed.
        void OnIterationCompleted(const search::PV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes);

        /// Method when the search is completed.
        void OnSearchCompleted(const search::PV& pv, double time, const search::SearchStats& stats);
        
    private:
        Board board_;
        DepthType depth_;
        float time_;

        std::unique_ptr<TimeManager> CreateTimeManager();

        static void ClearLine();
        static std::string FormatTime(double time);
        static std::string FormaterEval(int eval);

        std::vector<std::string> RenderPVMoves(const search::PV& pv);
        std::vector<std::string> JoinsPVMoves(std::vector<std::string>::const_iterator first,
                                              std::vector<std::string>::const_iterator last,
                                              size_t max_width);
        void DisplaySearchTableLine(bool is_iteration_complete,
                                    const search::PV& pv,
                                    EvalType eval,
                                    DepthType depth,
                                    double time,
                                    NodeCounterType nodes);
    };
}

#endif // M8_ANALYZER_HPP_