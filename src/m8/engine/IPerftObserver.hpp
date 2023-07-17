/// @file   IPerftObserver.hpp
/// @author Mathieu Pagé
/// @date   January 2020

#ifndef M8_ENGINE_IPERFT_OBSERVER_HPP_
#define M8_ENGINE_IPERFT_OBSERVER_HPP_

#include <string>

#include "../../m8chess/Types.hpp"

namespace m8::engine {

    /// Contains a list of callbacks used by the engine class and the different
    /// engine states classes.
    class IPerftObserver
    {
    public:
        /// Method called everytime a partial perf result is ready
        /// 
        /// @param move  The move for which the result is available 
        /// @param count The number of nodes
        virtual void OnPartialPerftResult(const std::string& move, std::uint64_t count) = 0;

        /// Method called at the end of the perft test
        /// 
        /// @param count The number of nodes
        /// @param time  The time used to complete the test
        virtual void OnPerftCompleted(std::uint64_t count, double time) = 0;
    };
}

#endif // M8_ENGINE_IPERFT_OBSERVER_HPP_