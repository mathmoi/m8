/// @file   m8Intrf.cpp
/// @author Mathieu Pagé
/// @date   May 2015
/// @brief  Contains the class m8Intrf that implements m8's shell like interface.

#include <iostream>
#include <vector>
#include <string>

#include "m8Intrf.hpp"
#include "../m8common/stringHelpers.hpp"


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
        shell_intrf_.AddCmd(ShellCmd("fen",
            "Get or set the position of the board as a XFen string",
            "fen [XFenString]",
            std::bind(&m8Intrf::HandleFen, this, _1)));
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
        std::cout << '\n' << engine_.board() << '\n' << std::endl;
    }

    void m8Intrf::HandleFen(std::vector<std::string> args_list)
    {
        if (args_list.size() == 1)
        {
            std::cout <<' ' << engine_.board().fen() << std::endl;
        }
        else
        {
            std::string fen = join(args_list.begin() + 1, args_list.end(), " ");

            try
            {
                engine_.set_fen(fen);
            }
            catch (InvalFenError)
            {
                std::cout << "Invalid fen string." << std::endl;
            }
        }
    }
}