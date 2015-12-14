/// @file   ShellIntrf.hpp
/// @author Mathieu Pagé
/// @date   May 2015
/// @brief  Contains classes that are used to create a shell-like interface.

#ifndef M8_SHELL_INTRF_HPP_
#define M8_SHELL_INTRF_HPP_

#include <functional>
#include <map>
#include <string>
#include <vector>

namespace m8
{
   /// Represent a command of the shell interface.
   class ShellCmd
   {
   public:

      /// Type of the function called when a command is executed.
      typedef std::function<void(const std::vector<std::string>)> Funct;

      /// Default constructor.
      ///
      /// @params name  Name of the command.
      /// @params des   Description of the command (short description).
      /// @params usage Usage of the command (longer description).
      ShellCmd(const std::string& name,
         const std::string& des,
         const std::string& usage,
         Funct funct);

      /// Accessor for the command's name.
      ///
      /// @returns the command's name.
      inline std::string name() const { return name_; };

      /// Accessor for the description.
      ///
      /// @returns The command's description.
      inline std::string des() const { return des_; };

      /// Accessor for the usage.
      ///
      /// @returns The command's usager.
      inline std::string Usage() const { return usage_; };

      /// Calls the function that handle this command.
      inline void operator()(const std::vector<std::string> param_list) const
      {
         funct_(param_list);
      };

   private:

      /// Command's name.
      std::string name_;

      /// Command's description.
      std::string des_;

      /// Command's usage.
      std::string usage_;

      /// Function that should be called when the command is entered by 
      /// the user.
      Funct funct_;
   };

   /// Class implemeting a shell-like interface
   class ShellIntrf
   {
   public:

      /// Default constructor.
      ShellIntrf();

      /// Add a command to the interface's list of recognized commands.
      ///
      /// @param cmd Command to add.
      void AddCmd(const ShellCmd& cmd);

      /// Accessor for the abort flag.
      ///
      /// @returns The abort flag.
      inline bool abort() const { return abort_; };

      /// Mutator for the abort flag.
      ///
      /// @param abort New value of the abort flag.
      inline void set_abort(bool abort) { abort_ = abort; };

      /// Accessor for the invite
      ///
      /// @returns The invite.
      inline std::string invit() const { return invit_; };

      /// Mutator for the invite.
      ///
      /// @param invit New invite.
      inline void set_invit(std::string invit) { invit_ = invit; };

      /// Execute the shell interface. The execution will continue until abord 
      /// is set to true.
      void Execute() const;

   private:

      typedef std::map<std::string, ShellCmd> CmdList;

      /// List of commands.
      CmdList cmd_list_;

      /// Flag indicating if we should abort the execution of the interface.
      bool abort_;

      /// Invite printed on the output when we are ready to receive a command.
      std::string invit_;
   };
}

#endif // M8_SHELL_INTRF_HPP_