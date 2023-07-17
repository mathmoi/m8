/// @file   Searcher.cpp 
/// @author Mathieu Pagé
/// @date   March 2018
/// @brief  Contains the Searcher class. This class is responsible to manage the engine's 
///         search. The search algorithm itself is contained elswhere, but is access by
///         the enigne class trough the search class.

#include <thread>

#include "Searcher.hpp"
#include "../eval/Eval.hpp"
#include "../../m8common/logging.hpp"

namespace m8 { namespace search
{
    Searcher::Searcher()
	 : state_(SearchState::Ready),
	   iterative_deepening_()
    {
		iterative_deepening_.Attach(this);
	}

	Searcher::~Searcher()
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

		iterative_deepening_.Detatch(this);
	}

	void Searcher::Start(std::shared_ptr<Search> search)
	{
		assert(state_ == SearchState::Ready);

		state_          = SearchState::Searching;
		current_search_ = search;
		start_time_     = std::chrono::steady_clock::now();

		Attach(&current_search_->time_manager());
		
		// TODO : Can we make the search thread permanant? It would wait for work.
		if (search_thread_.joinable())
		{
			search_thread_.join();
		}

		search_thread_ = std::thread(&Searcher::RunSearchThread, this, search);
	}

	bool Searcher::StopSearch()
	{
		bool was_searching = false;
		if (state_ == SearchState::Searching)
		{
			std::lock_guard<std::mutex> lock(mutex_);

			if (state_ == SearchState::Searching)
			{
				was_searching = true;
				state_ = SearchState::Stopped;
				Detatch(&current_search_->time_manager());
				current_search_->Abort();
				current_search_.reset();
			}
		}
		return was_searching;
	}

	void Searcher::Stop()
	{
		StopSearch();
	}

	void Searcher::RunSearchThread(std::shared_ptr<Search> search)
 	{
		M8_LOG_SCOPE_THREAD();

		NotifySearchStarted();

		auto search_result = iterative_deepening_.Start(search);

		bool was_searching = StopSearch();
		if (was_searching)
		{
			NotifySearchCompleted(search_result.pv_, GetSearchTime(), search_result.stats_);
		}
		state_ = SearchState::Ready;
	}

	double Searcher::GetSearchTime() const
	{
		auto now = std::chrono::steady_clock::now();
		auto time_span = std::chrono::duration_cast<std::chrono::duration<double>>(now - start_time_);
		return time_span.count();
	}

	void Searcher::OnNewBestMove(const PV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes)
	{
		NotifyNewBestMove(pv, eval, depth, GetSearchTime(), nodes);
	}

	void Searcher::OnIterationStarted()
	{
		NotifyIterationStarted();
	}

	void Searcher::OnIterationCompleted(const PV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes)
	{
		NotifyIterationCompleted(pv, eval, depth, GetSearchTime(), nodes);
	}
}}