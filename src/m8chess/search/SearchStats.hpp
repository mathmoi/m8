/// @file SearchStats.hpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023

#include "../Types.hpp"

#ifndef M8_SEARCH_SEARCH_STATS_HPP_
#define M8_SEARCH_SEARCH_STATS_HPP_

namespace m8::search
{
    struct SearchStats
    {
    public:
        /// Constructor
        SearchStats()
        : nodes(0),
          qnodes(0),
          tt_probes(0),
          tt_hits(0),
          tt_hits_exact(0),
          tt_hits_upper(0),
          tt_hits_lower(0)
        {}

        /// Number of nodes searched
        NodeCounterType nodes;

        /// Number of quiescence nodes searched
        NodeCounterType qnodes;

        /// Number of probes of the transposition table
        std::uint64_t tt_probes;

        /// Numbers of times a tt_probe is sucessful
        std::uint64_t tt_hits;

        /// Number of hits that contains an exact value
        std::uint64_t tt_hits_exact;

        /// Number of hits that contains an upper bound
        std::uint64_t tt_hits_upper;

        /// Number of hits that contains a lower bound
        std::uint64_t tt_hits_lower;
    };
}

#endif // M8_SEARCH_SEARCH_STATS_HPP_