/// @file   m8.cpp
/// @author Mathieu Pagé
/// @date   May 2015
/// @brief  Contains the entry point (main) of m8.

#include <iostream>
#include <fstream>

#include <boost/filesystem.hpp>

#include "m8Intrf.hpp"
#include "../m8chess/Init.hpp"
#include "options/Options.hpp"

bool ReadOptions(int argc, char* argv[])
{
    auto options = m8::Options::instance();

    if (!boost::filesystem::exists("m8.ini"))
    {
        std::ofstream ofile("m8.ini");
        options.CreateOptionsFile(ofile);
        ofile.close();
    }

    std::ifstream m8_ini("m8.ini");
    bool stop_execution = options.ReadOptions(argc, argv, m8_ini, std::cout);
    m8_ini.close();

    return stop_execution;
}

/// Main function. The entry point of the application.
///
/// @param argc Number of arguments in argv
/// @param argv Arguments passed on the command line when the application
///             is started.
/// @returns Return code of the application. Should be 0 if the application 
///          terminated correctly and another value otherwise.
int main(int argc, char* argv[])
{
    bool stop_execution = ReadOptions(argc, argv);
    if (!stop_execution)
    {
        m8::InitializePreCalc();

        m8::m8Intrf intrf;
        intrf.Execute();
    }
   
    return 0;
}