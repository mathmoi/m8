/// @file   tests_main.cpp
/// @author Mathieu Pagé
/// @date   Feburary 2015
/// @brief  Contains the Catch framework main function.

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include "../common/Init.hpp"

int main(int argc, char* argv[])
{
    m8::InitializePreCalc();

    int result = Catch::Session().run(argc, argv);
    return result;
}