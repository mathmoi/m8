/// @file   m8Intrf.hpp
/// @author Mathieu Pag�
/// @date   May 2015
/// @brief  Contains the class m8Intrf that implements m8's shell like interface.

#ifndef M8_M8_INTRF_HPP_
#define M8_M8_INTRF_HPP_

#include <mutex>

#include "ShellIntrf.hpp"
#include "engine/Engine.hpp"
#include "options/Options.hpp"

namespace m8
{
   /// Class that implements m8's shell-like interface.
   class m8Intrf
   {
   public:

      /// Default constructor.
      m8Intrf();

      /// Execute the interface.
      void Execute();

   private:

      engine::Engine engine_;
	  bool xboard_;

      /// Object that implements the shell interface.
      ShellIntrf shell_intrf_;

      mutable std::mutex output_mutex_;

      /// Setup the shell interface.
      void SetupShellInterf();

	  /// Setup the xboard mode.
	  void SetupXboardMode();

      engine::EngineCallbacks CreateEngineCallbacks();

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
      void HandlePing(std::vector<std::string> args_list);

      void DisplayOption(const Option& option) const;
      void DisplayOption(const std::string& option_name) const;
      void EditOption(const std::string& option_name, const std::string& value) const;

	  void DisplayEngineMove(const std::string& move);
      void DisplayPerftPartialResult(std::string move, std::uint64_t count);
      void DisplayPerftResult(std::uint64_t count, double seconds);
      void DisplayBoard() const;

      bool CallEngineCommand(std::function<void()> call, const std::string& command);
	  void ClearLine() const;
   };
}

#endif // M8_M8_INTRF_HPP_