/// @file   ISearchObserver.hpp
/// @author Mathieu Pagé
/// @date   January 2020
/// @brief  Contains a virtual base class (interface) for classes that can observe a 
///         search and receive "events".

#ifndef M8_SEARCH_SEARCH_OBSERVER_HPP_
#define M8_SEARCH_SEARCH_OBSERVER_HPP_

#include "SearchStats.hpp"

namespace m8::search {

    /// Interface for classes that can observe the search
    ///
    /// @tparam TPV Type of move used to communicate with the ISearchObserver. This can be
    ///         used to provide a strong type PV class like search::Pv or a weakly typed
    ///         one (a vector of string).
    template<typename TPV>
    class ISearchObserver
    {
    public:
        /// Method called when the search starts.
        inline virtual void OnSearchStarted() {};

        /// Method called when a new best move is found at the root.
        inline virtual void OnNewBestMove(const TPV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes) {};

        /// Method called when an iteration is started.
        inline virtual void OnIterationStarted() {};

        /// Method called when an iteration is completed.
        inline virtual void OnIterationCompleted(const TPV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes) {};

        /// Method when the search is completed.
        inline virtual void OnSearchCompleted(const TPV& pv, double time, const SearchStats& stats) {};
    };
}

#endif // M8_SEARCH_SEARCH_OBSERVER_HPP_
