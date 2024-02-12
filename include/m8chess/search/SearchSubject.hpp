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
    /// @tparam TMove Similarly the TMove type is the type used to represent a move.
    template<typename TPV, typename TMove>
    class SearchSubject
    {
    public:
        /// Constructor
        SearchSubject()
        : observers_()
        {}

        /// Add an observer.
        inline void Attach(ISearchObserver<TPV, TMove>* observer)
        {
            observers_.push_back(observer);
        }

        inline void Detatch(ISearchObserver<TPV, TMove>* observer)
        {
            std::erase(observers_, observer);
        }

        inline void Notify(std::function<void(ISearchObserver<TPV, TMove>*)> notification)
        {
            for(ISearchObserver<TPV, TMove>* observer : observers_)
            {
                notification(observer);
            }
        }

    protected:
        /// Method called when the search starts.
        inline void NotifySearchStarted()
        {
            Notify( [](ISearchObserver<TPV, TMove>* observer){ observer->OnSearchStarted(); } );
        }

        /// Method called when a new best move is found at the root.
        inline void NotifyNewBestMove(const TPV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes)
        {
            Notify( [pv, eval, depth, time, nodes](ISearchObserver<TPV, TMove>* observer){ observer->OnNewBestMove(pv, eval, depth, time, nodes); } );
        }

        /// Method called when the an iteration is started.
        inline void NotifyIterationStarted()
        {
            Notify( [](ISearchObserver<TPV, TMove>* observer){ observer->OnIterationStarted(); } );
        }

        /// Method called when a new moved is searched at the root.
        inline void NotifySearchMoveAtRoot(DepthType depth, double time, std::uint16_t move_number, std::uint16_t moves_number, NodeCounterType nodes, TMove move)
        {
            Notify(
            [depth, time, move_number, moves_number, nodes, move](ISearchObserver<TPV, TMove>* observer)
            {
                observer->OnSearchMoveAtRoot(depth, time, move_number, moves_number, nodes, move);
            });
        }

        /// Method called when an iteration is completed.
        inline void NotifyIterationCompleted(const TPV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes)
        {
            Notify( [pv, eval, depth, time, nodes](ISearchObserver<TPV, TMove>* observer){ observer->OnIterationCompleted(pv, eval, depth, time, nodes); } );
        }

        /// Method when the search is completed.
        inline void NotifySearchCompleted(const TPV& pv, double time, const SearchStats& stats)
        {
            Notify( [pv, time, stats](ISearchObserver<TPV, TMove>* observer){ observer->OnSearchCompleted(pv, time, stats); } );
        }
        
    private:
        std::vector<ISearchObserver<TPV, TMove>*> observers_;
    };
}

#endif // M8_CHESS_SEARCH_SUBJECT_HP