/// @file   Search.hpp
/// @author Mathieu Pagé
/// @date   March 2018
/// @brief  Contains the Search class. This class is responsible to manage the engine's 
///         search. The search algorithm itself is contained elswhere, but is access by
///         the enigne class trough the search class.

#ifndef M8_SEARCH_HPP_
#define M8_SEARCH_HPP_

#include <vector>

#include "../Board.hpp"
#include "../../m8common/ThreadPool.hpp"

namespace m8 { namespace search
{
    /// Manage the seach for the engine.
    class Search
    {
    public:

        /// Constructor
        Search();

        /// Start a search on a given position.
        void Start(/*const Board& board*/);

    private:
        void CreateThreads();
        int AlphaBeta(int alpha, int beta, int depth_left);

        bool searching_;
        Board board_;
        ThreadPool thread_pool_;
    };

}}

#endif // M8_SEARCH_HPP_