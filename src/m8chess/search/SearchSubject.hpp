/// @file SearchSubject.hpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023

#ifndef M8_CHESS_SEARCH_SUBJECT_HPP_
#define M8_CHESS_SEARCH_SUBJECT_HPP_

#include <vector>
#include <functional>

#include "ISearchObserver.hpp"
#include "SearchStats.hpp"

namespace m8::search
{
    /// This is the subclass for all class that are subject in the Observer pattern for
    /// a search. It will allow derived class to notify zero or more observer of the
    /// progression of the search.
    /// 
    /// @tparam TPV Type of move used to communicate with the ISearchObserver. This can be
    ///         used to provide a strong type PV class like search::Pv or a weakly typed
    ///         one (a vector of string).
    template<typename TPV>
    class SearchSubject
    {
    public:
        /// Constructor
        SearchSubject()
        : observers_()
        {}

        /// Add an observer.
        inline void Attach(ISearchObserver<TPV>* observer)
        {
            observers_.push_back(observer);
        }

        inline void Detatch(ISearchObserver<TPV>* observer)
        {
            std::erase(observers_, observer);
        }

        inline void Notify(std::function<void(ISearchObserver<TPV>*)> notification)
        {
            for(ISearchObserver<TPV>* observer : observers_)
            {
                notification(observer);
            }
        }

    protected:
        /// Method called when the search starts.
		inline void NotifySearchStarted()
        {
            Notify( [](ISearchObserver<TPV>* observer){ observer->OnSearchStarted(); } );
        }

		/// Method called when a new best move is found at the root.
		inline void NotifyNewBestMove(const TPV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes)
        {
            Notify( [pv, eval, depth, time, nodes](ISearchObserver<TPV>* observer){ observer->OnNewBestMove(pv, eval, depth, time, nodes); } );
        }

        /// Method called when the an iteration is started.
		inline void NotifyIterationStarted()
        {
            Notify( [](ISearchObserver<TPV>* observer){ observer->OnIterationStarted(); } );
        }

		/// Method called when an iteration is completed.
		inline void NotifyIterationCompleted(const TPV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes)
        {
            Notify( [pv, eval, depth, time, nodes](ISearchObserver<TPV>* observer){ observer->OnIterationCompleted(pv, eval, depth, time, nodes); } );
        }

		/// Method when the search is completed.
		inline void NotifySearchCompleted(const TPV& pv, double time, const SearchStats& stats)
        {
            Notify( [pv, time, stats](ISearchObserver<TPV>* observer){ observer->OnSearchCompleted(pv, time, stats); } );
        }
        
    private:
        std::vector<ISearchObserver<TPV>*> observers_;
    };
}

#endif // M8_CHESS_SEARCH_SUBJECT_HP