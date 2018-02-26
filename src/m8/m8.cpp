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
#include "../m8common/logging.hpp"

#include "../m8chess/eval/PieceSqTable.hpp"

namespace m8
{
    bool ReadOptions(int argc, char* argv[])
    {
        ReadOptionsFromFile("m8.json");

        bool stop_execution = ReadOptionsFromCommandLine(argc, argv, std::cout);
        return stop_execution;
    }
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

    int ret = 0;

    M8_LOG_SCOPE_THREAD();

    try {
        bool stop_execution = m8::ReadOptions(argc, argv);

        if (!stop_execution)
        {
            m8::InitializePreCalc();
            
            m8::m8Intrf intrf;
            intrf.Execute();
        }
    }
    catch (const std::exception& ex) {
        M8_FATAL << "Unhandled exception: " << ex.what();
        std::cerr << "Unhandled exception: " << ex.what() <<std::endl;
        ret = -1;
    }
    catch (...) {
        M8_FATAL << "Unhandled exception";
        std::cerr << "Unhandled exception" << std::endl;
        ret = -2;
    }
   
    return ret;
}