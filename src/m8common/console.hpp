/// @file   console.hpp
/// @author Mathieu Pagé
/// @date   Novembre 2019
/// @brief  Contains functionalities about the console window.

#ifndef M8_CONSOLE_HPP_
#define M8_CONSOLE_HPP_

#include <cstdint>

#include <Windows.h>

namespace m8 {

	inline std::int16_t GetConsoleWidth()
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		int columns;

		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;

		return columns;
	}
}

#endif // M8_CONSOLE_HPP_