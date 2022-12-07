/// @file   Search.cpp 
/// @author Mathieu Pag�
/// @date   March 2018
/// @brief  Contains the Search class. This class is responsible to manage the engine's 
///         search. The search algorithm itself is contained elswhere, but is access by
///         the enigne class trough the search class.

#include <thread>

#include "Search.hpp"
#include "../eval/Eval.hpp"
#include "../../m8common/logging.hpp"

namespace m8 { namespace search
{
    Search::Search(const Board& board,
	               SearchObserver* observer)
		: board_(board),
		  state_(SearchState::Ready),
		  observer_(observer)
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
		
		start_time_ = std::chrono::steady_clock::now();
		state_ = SearchState::Searching;

		// TODO : We should make sure Search objects are not reausable. This would then become obsolete.
		if (search_thread_.joinable())
		{
			search_thread_.join();
		}

		ptr_iterative_deepening_ = std::make_unique<IterativeDeepening>(board_, this);
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
			ptr_iterative_deepening_->Stop();
		}
	}

	void Search::RunSearchThread()
 	{
		M8_LOG_SCOPE_THREAD();

		observer_->OnBeginSearch();

		auto search_result = ptr_iterative_deepening_->Search(6);

		bool was_searching = StopSearch();
		if (was_searching)
		{
			observer_->OnSearchCompleted(search_result.pv_, GetSearchTime());
		}
	}

	double Search::GetSearchTime() const
	{
		auto now = std::chrono::steady_clock::now();
		auto time_span = std::chrono::duration_cast<std::chrono::duration<double>>(now - start_time_);
		return time_span.count();
	}

	void Search::OnNewBestMove(const PV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes)
	{
		observer_->OnNewBestMove(pv, eval, depth, GetSearchTime(), nodes);
	}

	void Search::OnIterationCompleted(const PV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes)
	{
		observer_->OnIterationCompleted(pv, eval, depth, GetSearchTime(), nodes);
	}
}}