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

    /// Type of search result
    enum class ResultType
    {
        /// The searche ended before any usefull informations was found. 
        /// Only the stats_ can be used.
        None,

        /// The search ended after some move were completely searched at the root, but
        /// before all moves were searched. The PV and value can be used, but they might
        /// have been different if the search had the time to search all the root moves.
        Partial,

        /// The search completely searched all moves.
        Complete
    };

    /// Result of a search
    struct SearchResult
    {
    public:
        /// Default constuctor
        inline SearchResult()
            : type_(ResultType::None),
              value_(0)
        {}

        // Constructor.
        inline SearchResult(ResultType type,
                            const PV& pv,
                            EvalType value,
                            const SearchStats& stats)
        : type_(type),
          value_(value),
          pv_(pv),
          stats_(stats)
        {}

        /// Overload the operator += to augment a SearchResult with another one. The data
        /// is copied from the rhs search result to the current result based on the type
        /// or the rhs search result.
        inline SearchResult& operator+= (const SearchResult& rhs)
        {
            if (rhs.type_ == ResultType::Partial || rhs.type_ == ResultType::Complete)
            {
                type_  = rhs.type_;
                value_ = rhs.value_;
                pv_    = rhs.pv_;
            }

            stats_ = rhs.stats_;

            return *this;
        }

        ResultType  type_;
        EvalType    value_;
        PV          pv_;
        SearchStats stats_;
    };
}

#endif // M8_SEARCH_RESULT_HPP_