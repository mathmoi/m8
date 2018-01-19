/// @file   Timer.hpp
/// @author Mathieu Pagé
/// @date   January 2018
/// @brief  Contains a timer class that can be used to measure time intervals.

#ifndef M8_TIMER_HPP_
#define M8_TIMER_HPP_

#include <cstdint>

#if defined(_MSC_VER)
#  define NOMINMAX 
#  include <windows.h>
#else

#endif

namespace m8
{
    /// Class allowing to efficiently measure precise time intervals.
    class Timerrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
    {
    private:
        LARGE_INTEGER start_;

        static inline std::int64_t GetFrequency()
        {
            LARGE_INTEGER freq;
            QueryPerformanceFrequency(&freq);

            return freq.QuadPart;
        }

    public:

        /// Start the timer.
        inline void start()
        {
            QueryPerformanceCounter(&start_);
        }

        /// Return the number of time unit elapsed since the timer was started. The timer
        /// must have previously been strated otherwise results are undefined.
        inline std::int64_t elapsed() const
        {
            LARGE_INTEGER now;
            QueryPerformanceCounter(&now);

            return now.QuadPart - start_.QuadPart;
        }

        /// return the frequency of the timer. The frequency is the number of unit of
        /// time in a second. This can be used to calculate the time in seconds based on
        /// the result returned by elapsed.
        static inline std::int64_t frequency()
        {
            static std::int64_t freq = GetFrequency();
            return freq;
        }
    };
}

#endif // M8_TIMER_HPP_