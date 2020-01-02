/// @file   Search.cpp
/// @author Mathieu Pagé
/// @date   March 2018
/// @brief  Contains the Search class. This class is responsible to manage the engine's 
///         search. The search algorithm itself is contained elswhere, but is access by
///         the enigne class trough the search class.

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
			search_thread_.join();
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

		if (search_thread_.joinable())
		{
			search_thread_.join();
		}

		ptr_minimax_ = std::make_unique<Minimax>(board_);
		search_thread_ = std::thread(&Search::RunSearchThread, this);
	}

	void Search::Stop()
	{
		if (state_ == SearchState::Searching)
		{
			std::lock_guard<std::mutex> lock(mutex_);

			if (state_ == SearchState::Searching)
			{
				ptr_minimax_->Stop();
			}
		}
	}

	void Search::RunSearchThread()
	{
		M8_LOG_SCOPE_THREAD();

		auto search_result = ptr_minimax_->Search(4);

		{
			std::lock_guard<std::mutex> lock(mutex_);

			ptr_minimax_.reset(nullptr);
			state_ = SearchState::Ready;
		}

		search_completed_callback_(search_result);

	}
}}