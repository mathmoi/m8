/// @file   Searcher.hpp
/// @author Mathieu Pagé
/// @date   March 2018
/// @brief  Contains the Searcher class. This class is responsible to manage the engine's 
///         search. The search algorithm itself is contained elswhere, but is access by
///         the enigne class trough the search class.

#ifndef M8_SEARCH_SEARCHER_HPP_
#define M8_SEARCH_SEARCHER_HPP_

#include <chrono>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "../time/TimeManager.hpp"

#include "../Board.hpp"

#include "IterativeDeepening.hpp"
#include "Search.hpp"
#include "SearchResult.hpp"
#include "SearchSubject.hpp"

namespace m8::search
{
	/// States of the search object.
	enum class SearchState
	{
		Ready,
		Searching,
		Stopped
	};

    /// Manage the seach for the engine.
    class Searcher : public SearchSubject<PV>, public ISearchObserver<PV>
    {
    public:
		
        /// Constructor
        Searcher();

		/// Destructor
		~Searcher();

		/// returns the search state.
		inline SearchState state() const { return state_; };

		/// Start the search
		///
		void Start(std::shared_ptr<Search> search);

		/// Stop the search
		void Stop();

		////////////////////////////////////////////////////////////////////////////////// 
		/// Implementation of ISearchObserver
		//////////////////////////////////////////////////////////////////////////////////

		/// Method called when a new best move is found at the root.
		inline void OnNewBestMove(const PV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes);

		/// Method called when an iteration is started.
		inline void OnIterationStarted();

		/// Method called when an iteration is completed.
		inline void OnIterationCompleted(const PV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes);

    private:
		std::thread search_thread_;
		std::mutex mutex_;
        
		SearchState state_;
		IterativeDeepening iterative_deepening_;

		std::shared_ptr<Search> current_search_;

		std::chrono::steady_clock::time_point start_time_;

		double GetSearchTime() const;
		void RunSearchThread(std::shared_ptr<Search> search);
		bool StopSearch();
    };

}

#endif // M8_SEARCH_SEARCHER_HPP_