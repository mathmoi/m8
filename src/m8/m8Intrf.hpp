/// @file   m8Intrf.hpp
/// @author Mathieu Pagé
/// @date   May 2015
/// @brief  Contains the class m8Intrf that implements m8's shell like interface.

#ifndef M8_M8_INTRF_HPP_
#define M8_M8_INTRF_HPP_

#include "ShellIntrf.hpp"

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

      /// Object that implements the shell interface.
      ShellIntrf shell_intrf_;

      /// Setup the shell interface.
      void SetupShellInterf();

      /// Handles the exit command.
      void HandleExit();

      /// Handles a user move.
      ///
      /// @params args_list Arguments of the command inputed by the user.
      void HandleUserMove(std::vector<std::string> args_list);

      /// Handles the help command.
      ///
      /// @params args_list Arguments of the command inputed by the user.
      void HandleHelp();
   };

}

#endif // M8_M8_INTRF_HPP_