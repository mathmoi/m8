/// @file   Search.cpp
/// @author Mathieu Pagé
/// @date   March 2018
/// @brief  Contains the Search class. This class is responsible to manage the engine's 
///         search. The search algorithm itself is contained elswhere, but is access by
///         the enigne class trough the search class.

#include <thread>

#include "Search.hpp"
#include "Minimax.hpp"
#include "../eval/Eval.hpp"
#include "../../m8common/logging.hpp"

namespace m8 { namespace search
{
    Search::Search(const Board& board,
	               SearchCompletedCallback search_completed_callback)
		: board_(board),
		  state_(SearchState::Ready),
		  search_completed_callback_(search_completed_callback)
    {}

	Search::~Search()
	{
		if (search_thread_.joinable())
		{
			if (search_thread_.get_id() == std::this_thread::get_id())
			{
				search_thread_.detach();
			}
			else
			{
				search_thread_.join();
			}
		}
	}

	void Search::set_board(const Board& board)
	{
		assert(state_ == SearchState::Ready);

		board_ = board;
	}

	void Search::Start()
	{
		assert(state_ == SearchState::Ready);
		
		state_ = SearchState::Searching;

		// TODO : We should make sure Search objects are not reausable. This would then become obsolete.
		if (search_thread_.joinable())
		{
			search_thread_.join();
		}

		ptr_alpha_beta = std::make_unique<AlphaBeta>(board_);
		search_thread_ = std::thread(&Search::RunSearchThread, this);
	}

	bool Search::StopSearch()
	{
		bool was_searching = false;
		if (state_ == SearchState::Searching)
		{
			std::lock_guard<std::mutex> lock(mutex_);

			if (state_ == SearchState::Searching)
			{
				was_searching = true;
				state_ = SearchState::Stopped;
			}
		}

		return was_searching;
	}

	void Search::Stop()
	{
		bool was_searching = StopSearch();
		if (was_searching)
		{
			ptr_alpha_beta->Stop();
		}
	}

	void Search::RunSearchThread()
 	{
		M8_LOG_SCOPE_THREAD();

		auto search_result = ptr_alpha_beta->Search(7);

		bool was_searching = StopSearch();
		if (was_searching)
		{
			search_completed_callback_(search_result);
		}
	}
}}