/// @file chronoHelpers.hpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023

#ifndef M8_COMMON_CHRONO_HELPERS_HPP_
#define M8_COMMON_CHRONO_HELPERS_HPP_

#include <boost/lexical_cast.hpp>

#include <chrono>
#include <optional>

#include "Utils.hpp"

namespace m8
{
    /// Represents a duration in seconds stored as a floating point number.
    typedef std::chrono::duration<float> FSec;

    template<typename TDuration>
    inline FSec ToFSec(TDuration duration)
    {
        return std::chrono::duration_cast<FSec>(duration);
    }

    inline std::chrono::nanoseconds FloatToNanoseconds(float seconds)
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<float>(seconds));
    }

    template<typename TDuration>
    inline std::string ToString(std::optional<TDuration> duration)
    {
        if (duration)
        {
            return ToString(ToFSec(*duration));
        }

        return "std::nullopt";
    }
}
#endif // M8_COMMON_CHRONO_HELPERS_HPP_