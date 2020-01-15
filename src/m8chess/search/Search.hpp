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

#include "../Board.hpp"
#include "SearchResult.hpp"
#include "Minimax.hpp"

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
    class Search
    {
    public:
		typedef std::function<void (const SearchResult&)> SearchCompletedCallback;

        /// Constructor
        Search(const Board& board,
			   SearchCompletedCallback search_completed_callback);

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

    private:
		std::thread search_thread_;
		std::mutex mutex_;
        
		Board board_;
		SearchState state_;
		std::unique_ptr<Minimax> ptr_minimax_;

		SearchCompletedCallback search_completed_callback_;

		void RunSearchThread();
		bool StopSearch();
    };

}}

#endif // M8_SEARCH_HPP_