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

namespace m8
{
    bool ReadOptions(int argc, char* argv[])
    {

        auto& options = m8::Options::get();

        bool ini_file_exists = boost::filesystem::exists("m8.ini");
        if (ini_file_exists)
        {
            std::ifstream m8_ini("m8.ini");
            options.ReadOptions(m8_ini);
            m8_ini.close();
        }

        bool stop_execution = options.ReadOptions(argc, argv, std::cout);

        if (!ini_file_exists && options.ini().value())
        {
            std::ofstream ofile("m8.ini");
            options.CreateOptionsFile(ofile);
            ofile.close();
        }

        return stop_execution;
    }

    void LogOptions()
    {
        for (auto& pair : Options::get().map())
        {
            M8_INFO <<"Option " << pair.second.name() << '=' << pair.second.ToString();
        }
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
        m8::LogOptions();

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