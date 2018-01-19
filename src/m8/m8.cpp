/// @file   m8.cpp
/// @author Mathieu Pagé
/// @date   May 2015
/// @brief  Contains the entry point (main) of m8.

#include "m8Intrf.hpp"
#include "../m8chess/Init.hpp"

/// Main function. The entry point of the application.
///
/// @param argc Number of arguments in argv
/// @param argv Arguments passed on the command line when the application
///             is started.
/// @returns Return code of the application. Should be 0 if the application 
///          terminated correctly and another value otherwise.
int main(int argc, char* argv[])
{
   m8::InitializePreCalc();

   m8::m8Intrf intrf;
   intrf.Execute();

   return 0;
}