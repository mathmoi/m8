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
          qnodes(0)
        {}

        /// Number of nodes searched
        NodeCounterType nodes;

        /// Number of quiescence nodes searched
        NodeCounterType qnodes;    
    };
}

#endif // M8_SEARCH_SEARCH_STATS_HPP_