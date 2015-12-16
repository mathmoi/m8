/// @file   magic-finder.cpp
/// @author Mathieu Pagé
/// @date   DEcember 2015
/// @brief  Contains the entry point of the magic-finder tool for m8.

#include <iostream>

#include "../common/Bb.hpp"

namespace m8 {
	void t()
	{
		m8::Bb bb = 16;
		DisplayBb(std::cout, bb);
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
	m8::t();
	return 0;
}