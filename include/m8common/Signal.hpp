/// @file Signal.hpp
/// @author Mathieu Pagé
/// @date March 2024
/// @brief Declare a functionality that allows to stop the application when a 
///        SIGNAL is received.

#ifndef M8_SIGNAL_HPP_
#define M8_SIGNAL_HPP_

#include <atomic>

namespace m8
{
    extern std::atomic<bool> signal_received;

    void EnableSignalHandler();
    void DisableSignalHandler();
}

#endif // M8_SIGNAL_HPP_