/// @file Signal.cpp
/// @author Mathieu Pagé
/// @date March 2024
/// @brief Defines the functionalities that allows to stop the application when
///        a SIGNAL is received.

#include "csignal"

#include "m8common/Signal.hpp"

namespace m8
{
    std::atomic<bool> signal_received(false);

    void signalHandler(int signal)
    {
        signal_received.store(true);
    }

    void EnableSignalHandler()
    {
        signal(SIGINT, signalHandler);
    }

    void DisableSignalHandler()
    {
        signal(SIGINT, SIG_DFL);
    }
}