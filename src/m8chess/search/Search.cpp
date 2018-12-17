/// @file   Search.cpp
/// @author Mathieu Pagé
/// @date   March 2018
/// @brief  Contains the Search class. This class is responsible to manage the engine's 
///         search. The search algorithm itself is contained elswhere, but is access by
///         the enigne class trough the search class.

#include "Search.hpp"
#include "../eval/Eval.hpp"

namespace m8 { namespace search
{
    Search::Search()
        : searching_(false),
          board_(kStartingPositionFEN),
          thread_pool_(2)
    {}

    int Search::AlphaBeta(int alpha, int beta, int depth_left)
    {
        if (depth_left == 0)
        {
            return eval::Evaluate(board_);
        }
    }

    void Search::Start(/*const Board& board*/)
    {
        // TODO : Something
    }
}}