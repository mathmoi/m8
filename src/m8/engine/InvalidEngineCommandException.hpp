/// @file	InvalidEngineCommandException.hpp
/// @author Mathieu Pagé
/// @date	January 2020
/// @brief	Contains the InvalidEngineCommandException class.

#ifndef M8_ENGINE_INVALID_ENGINE_COMMAND_EXCEPTION_HPP_
#define M8_ENGINE_INVALID_ENGINE_COMMAND_EXCEPTION_HPP_

#include <stdexcept>

namespace m8::engine {

	/// Exception used by the engine classes to indicate that a command is not valid.
	class InvalidEngineCommandException : public std::runtime_error
	{
	public:
		/// Constructor.
		InvalidEngineCommandException()
			: std::runtime_error("Command not legal now")
		{}
	};
}

#endif // M8_ENGINE_INVALID_ENGINE_COMMAND_EXCEPTION_HPP_