/// @file   SearchResult.hpp
/// @author Mathieu Pagé
/// @date   Novembre 2019
/// @brief  Contains the SearchResult class.

#ifndef M8_SEARCH_RESULT_HPP_
#define M8_SEARCH_RESULT_HPP_

#include "../eval/Eval.hpp"
#include "../Types.hpp"

#include "PV.hpp"
#include "SearchStats.hpp"

namespace m8::search {

    /// Result of a search
    struct SearchResult
    {
    public:
        /// Default constuctor
        inline SearchResult()
            : value_(0)
        {}

        // Constructor.
        inline SearchResult(const PV& pv,
                     EvalType value,
                     const SearchStats& stats)
            : value_(value),
              pv_(pv),
              stats_(stats)
        {}

        EvalType    value_;
        PV          pv_;
        SearchStats stats_;

    };
}

#endif // M8_SEARCH_RESULT_HPP_