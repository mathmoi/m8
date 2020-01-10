/// @file   console.hpp
/// @author Mathieu Pag�
/// @date   Novembre 2019
/// @brief  Contains functionalities about the console window.

#ifndef M8_CONSOLE_HPP_
#define M8_CONSOLE_HPP_

#include <cstdint>

#if defined(_MSC_VER)
#	include <Windows.h>
#elif defined(__GNUC__)
#	include <sys/ioctl.h>
#	include <unistd.h>
#endif

namespace m8 {

	inline std::int16_t GetConsoleWidth()
	{
		int columns;

#if defined(_MSC_VER)
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
#elif defined(__GNUC__)
		struct winsize size;
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
		columns = size.ws_col;
#endif
		return columns;
	}
}

#endif // M8_CONSOLE_HPP_