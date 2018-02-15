/// @file   PerftOptions.hpp
/// @author Mathieu Pagé
/// @date   Feburary 2017
/// @brief  Class containings the parameters used by the perft functionality.

#ifndef M8_PERFT_OPTIONS_HPP_
#define M8_PERFT_OPTIONS_HPP_

namespace m8
{
    /// Options for perft calculations.
    class PerftOptions
    {
    public:
        /// Set the numbers of threads to use for perft computations.
        inline void set_threads(int value) { threads_ = value; }

        /// Numbers of threads to use to compute perft.
        inline int threads() const { return threads_; }

        /// Set the minimum numbers of chinks to create to share between threads.
        inline void set_min_works_items(int value) { min_works_items_ = value; }

        /// Minimum numbers of chunk to create to share between threads.
        inline int min_works_items() const { return min_works_items_; }
        
    private:
        int threads_ = 8;
        int min_works_items_ = 100;
    };
}

#endif // M8_PERFT_OPTIONS_HPP_