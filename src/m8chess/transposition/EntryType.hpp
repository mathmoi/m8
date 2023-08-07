/// @file EntryType.hpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023

#ifndef M8_TRANSPOSITION_ENTRY_TYPE_HPP_
#define M8_TRANSPOSITION_ENTRY_TYPE_HPP_

#include <cstdint>

namespace m8::transposition
{
    /// Type of evalation in the transposition table entry
    enum class EntryType : std::uint8_t
    {
        /// The transposition entry containe an exact evaluation
        Exact = 1,

        /// The transposition entry contains a lower bound evaluation
        LowerBound = 2,

        /// The transposition entry contains an upper bound evaluation
        UpperBound = 3
    };
}

#endif // M8_TRANSPOSITION_ENTRY_TYPE_HPP_