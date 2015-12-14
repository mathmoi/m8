/// @file   ShellIntrf.cpp
/// @author Mathieu Pagé
/// @date   May 2015
/// @brief  Contains classes that are used to create a shell-like interface.

#include <iostream>
#include <string>

#include <boost\algorithm\string\split.hpp>
#include <boost\algorithm\string\classification.hpp>

#include "ShellIntrf.hpp"

namespace m8
{
   ShellCmd::ShellCmd(const std::string& name,
                      const std::string& des,
                      const std::string& usage,
                      Funct funct)
      : name_(name),
        des_(des),
        usage_(usage),
        funct_(funct)
   {}

   ShellIntrf::ShellIntrf()
      : abort_(false),
        invit_("")
   {}

   void ShellIntrf::AddCmd(const ShellCmd& cmd)
   {
      cmd_list_.insert(CmdList::value_type(cmd.name(), cmd));
   }

   void ShellIntrf::Execute() const
   {
      std::string input;

      while (!abort_)
      {
         // Get a command from the input stream.
         std::cout << invit_;
         std::getline(std::cin, input);

         // Split the command line.
         std::vector<std::string> token_list;
         boost::split(token_list,
                      input,
                      boost::is_any_of(" "),
                      boost::token_compress_on);

         // Find the command and execute it.
         auto command_pair = cmd_list_.find(token_list[0]);
         if (command_pair != cmd_list_.end())
         {
            command_pair->second(token_list);
         }
         else
         {
            command_pair = cmd_list_.find("");
            if (command_pair != cmd_list_.end())
            {
               command_pair->second(token_list);
            }
            else
            {
               std::cout << "Command not found (" << token_list[0] << ")" << std::endl;
            }
         }
      }
   }
}