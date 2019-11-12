/// @file   m8Intrf.hpp
/// @author Mathieu Pag�
/// @date   May 2015
/// @brief  Contains the class m8Intrf that implements m8's shell like interface.

#ifndef M8_M8_INTRF_HPP_
#define M8_M8_INTRF_HPP_

#include "ShellIntrf.hpp"
#include "Engine.hpp"
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

      Engine engine_;

      /// Object that implements the shell interface.
      ShellIntrf shell_intrf_;

      /// Setup the shell interface.
      void SetupShellInterf();

      /// Handles the exit command.
      void HandleExit();

      /// Handles a user move.
      ///
      /// @params args_list Arguments of the command inputed by the user.
      void HandleUserMove(std::vector<std::string> args_list) const;

      /// Handles the help command.
      void HandleHelp() const;

      /// Handles the display command.
      void HandleDisplay() const;

      /// Handleds the fen command.
      void HandleFen(std::vector<std::string> args_list);

      /// Handles the perft command.
      void HandlePerft(std::vector<std::string> args_list);

      /// Handles the options command.
      void HandleOptions() const;

      /// Handles the option command.
      void HandleOption(std::vector<std::string> args_list) const;

	  /// Handles the go command.
	  void HandleGo();

      void DisplayOption(const Option& option) const;
      void DisplayOption(const std::string& option_name) const;
      void EditOption(const std::string& option_name, const std::string& value) const;

	  void DisplayEngineMove(const std::string& move);

	  void ClearLine();
   };

}

#endif // M8_M8_INTRF_HPP_