/// @file   Search.hpp
/// @author Mathieu Pag�
/// @date   March 2018
/// @brief  Contains the Search class. This class is responsible to manage the engine's 
///         search. The search algorithm itself is contained elswhere, but is access by
///         the enigne class trough the search class.

#ifndef M8_SEARCH_HPP_
#define M8_SEARCH_HPP_

#include <vector>
#include <thread>
#include <mutex>
#include <functional>
#include <memory>
#include <chrono>

#include "../Board.hpp"
#include "../time/TimeManager.hpp"
#include "SearchResult.hpp"
#include "SearchSubject.hpp"
#include "IterativeDeepening.hpp"

namespace m8 { namespace search
{
	/// States of the search object.
	enum class SearchState
	{
		Ready,
		Searching,
		Stopped
	};

    /// Manage the seach for the engine.
    class Search : public SearchSubject<PV>, public ISearchObserver<PV>
    {
    public:
		
        /// Constructor
        Search(const Board& board,
			   std::shared_ptr<time::TimeManager> time_manager);

		/// Destructor
		~Search();

		/// Update the internal board. This cannnot be called when the search is underway.
		void set_board(const Board& board);

		/// returns the search state.
		inline SearchState state() const { return state_; };

		/// Start the search
		void Start();

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
        
		Board board_;
		SearchState state_;
		std::unique_ptr<IterativeDeepening> ptr_iterative_deepening_;
		std::shared_ptr<time::TimeManager> time_manager_; // TODO : Could this pointer be unique? Do we really share the ownership?

		std::chrono::steady_clock::time_point start_time_;

		double GetSearchTime() const;
		void RunSearchThread();
		bool StopSearch();
    };

}}

#endif // M8_SEARCH_HPP_