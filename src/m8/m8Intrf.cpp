/// @file   m8Intrf.cpp
/// @author Mathieu Pagé
/// @date   May 2015
/// @brief  Contains the class m8Intrf that implements m8's shell like interface.

#include <iostream> // TODO : Remove this line
#include <vector>
#include <string>

#include "m8Intrf.hpp"

namespace m8
{
   m8Intrf::m8Intrf()
      : shell_intrf_(),
        engine_()
   {
      SetupShellInterf();
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
   }

   void m8Intrf::HandleExit()
   {
      shell_intrf_.set_abort(true);
   }

   void m8Intrf::HandleHelp()
   {
       shell_intrf_.DisplayHelp();
   }

   void m8Intrf::HandleDisplay()
   {
       std::cout <<'\n' << engine_.board() <<'\n' <<std::endl;
   }
}