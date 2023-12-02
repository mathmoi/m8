/// @file   m8Intrf.hpp
/// @author Mathieu Pagé
/// @date   May 2015
/// @brief  Contains the class m8Intrf that implements m8's shell like interface.

#ifndef M8_M8_INTRF_HPP_
#define M8_M8_INTRF_HPP_

#include <mutex>

#include "ShellIntrf.hpp"
#include "engine/Engine.hpp"
#include "engine/IPerftObserver.hpp"
#include "options/Options.hpp"

namespace m8
{
   /// Class that implements m8's shell-like interface.
   class m8Intrf : public search::ISearchObserver<std::vector<std::string>, std::string>,
                   public engine::IPerftObserver
   {
   public:
      /// Default constructor.
      m8Intrf();

      /// Execute the interface.
      void Execute();

      /// Method called when the search starts.
      void OnSearchStarted();

      /// Method called when a new moved is searched at the root.
      void OnSearchMoveAtRoot(DepthType depth, double time, std::uint16_t move_number, std::uint16_t moves_number, NodeCounterType nodes, std::string move);

      /// Method called when a new best move is found at the root.
      void OnNewBestMove(const std::vector<std::string> &pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes);

      /// Method called when an iteration is completed.
      void OnIterationCompleted(const std::vector<std::string> &pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes);

      /// Method when the search is completed.
      void OnSearchCompleted(const std::vector<std::string> &pv, double time, const search::SearchStats& stats);

      /// Method called everytime a partial perf result is ready
      /// 
      /// @param move  The move for which the result is available 
      /// @param count The number of nodes
      void OnPartialPerftResult(const std::string& move, std::uint64_t count);

      /// Method called at the end of the perft test
      /// 
      /// @param count The number of nodes
      /// @param time  The time used to complete the test
      void OnPerftCompleted(std::uint64_t count, double time);

   private:
      engine::Engine engine_;
      bool xboard_;

      /// Object that implements the shell interface.
      ShellIntrf shell_intrf_;

      mutable std::recursive_mutex output_mutex_;

      /// Setup the shell interface.
      void SetupShellInterf();

      /// Setup the xboard mode.
      void SetupXboardMode();

      void HandleExit();
      void HandleUserMove(std::vector<std::string> args_list);
      void HandleHelp() const;
      void HandleDisplay() const;
      void HandleFen(std::vector<std::string> args_list);
      void HandlePerft(std::vector<std::string> args_list);
      void HandleOptions() const;
      void HandleOption(std::vector<std::string> args_list) const;
      void HandleXboard();
      void HandleProtover(std::vector<std::string> args_list);
      void HandleGo();
      void HandleForce();
      void HandleStop();
      void HandleNew();
      void HandleRejected(std::vector<std::string> args_list);
      void HandlePing(std::vector<std::string> args_list);
      void HandleSt(std::vector<std::string> args_list);
      void HandleLevel(std::vector<std::string> args_list);
      void HandleSd(std::vector<std::string> args_list);

      void DisplayOption(const options::ModifiableOption &option) const;
      void DisplayOption(const std::string &option_name) const;
      void EditOption(const std::string &option_name, const std::string &value) const;

      void DisplayBoard() const;

      std::vector<std::string> JoinsPVMoves(std::vector<std::string>::const_iterator first,
                                            std::vector<std::string>::const_iterator last,
                                            size_t max_width = (std::numeric_limits<size_t>::max)()) const;
      void DisplaySearchTableHeader() const;
      void DisplaySearchTableLine(bool is_iteration_complete, const std::vector<std::string> &pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes) const;
      void DisplaySearchTableFooter(double time, const search::SearchStats& stats) const;
      void DisplaySearchOutputXboard(const std::vector<std::string> &pv, EvalType eval, DepthType depth, double seconds, NodeCounterType nodes) const;
      std::string FormaterEval(int eval) const;
      std::string FormatTime(double time) const;

      bool CallEngineCommand(std::function<void()> call, const std::string &command);
      template<typename T>
      std::optional<T> ConvertArgument(const std::string& arg, const std::string& usage);
      void ClearLine() const;
   };
}

#endif // M8_M8_INTRF_HPP_