/// @file   Searcher.cpp 
/// @author Mathieu Pagé
/// @date   March 2018
/// @brief  Contains the Searcher class. This class is responsible to manage the engine's 
///         search. The search algorithm itself is contained elswhere, but is access by
///         the enigne class trough the search class.

#include <thread>

#include "m8chess/search/Searcher.hpp"
#include "m8chess/eval/Eval.hpp"
#include "m8common/logging.hpp"

namespace m8 { namespace search
{
    Searcher::Searcher(transposition::TranspositionTable& transposition_table)
     : destroying_(false),
       search_thread_(&Searcher::RunSearchThread, this),   
       state_(SearchState::Ready),
       transposition_table_(transposition_table),
       iterative_deepening_(transposition_table)
    {
        iterative_deepening_.Attach(this);
    }

    Searcher::~Searcher()
    {
        {
            std::lock_guard lock(mutex_);
            destroying_ = true;
        }
        condition_variable_.notify_all();

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

        {
            std::lock_guard lock(mutex_);

            transposition_table_.IncrementGeneration();

            state_          = SearchState::Searching;
            current_search_ = search;
            start_time_     = std::chrono::steady_clock::now();

            Attach(&current_search_->time_manager());
        }
        condition_variable_.notify_one();
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
        if (state_ == SearchState::Searching)
        {
            std::lock_guard<std::mutex> lock(mutex_);

            if (state_ == SearchState::Searching)
            {
                current_search_->Abort();
            }
        }
    }

    void Searcher::RunSearchThread()
     {
        M8_LOG_SCOPE_THREAD();

        while(!destroying_)
        {
            std::shared_ptr<Search> search_to_run;
            {
                std::unique_lock lock(mutex_);
                condition_variable_.wait(lock, [this]{ return destroying_ || current_search_; });

                if (!destroying_ && current_search_)
                {
                    search_to_run = current_search_;
                }
            }

            if (search_to_run)
            {
                NotifySearchStarted();
                auto search_result = iterative_deepening_.Start(search_to_run);
                bool was_searching = StopSearch();
                if (was_searching)
                {
                    NotifySearchCompleted(search_result.pv_, GetSearchTime(), search_result.stats_);
                }
                state_ = SearchState::Ready;
            }
        }
    }

    double Searcher::GetSearchTime() const
    {
        auto now = std::chrono::steady_clock::now();
        auto time_span = std::chrono::duration_cast<std::chrono::duration<double>>(now - start_time_);
        return time_span.count();
    }

    void Searcher::OnSearchMoveAtRoot(DepthType depth, double time, std::uint16_t move_number, std::uint16_t moves_number, NodeCounterType nodes, Move move)
    {
        NotifySearchMoveAtRoot(depth, GetSearchTime(), move_number, moves_number, nodes, move);
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
