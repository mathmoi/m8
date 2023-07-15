/// @file Search.hpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023

#ifndef M8_SEARCH_SEARCH_HPP_
#define M8_SEARCH_SEARCH_HPP_

#include <memory>

#include "../time/TimeManager.hpp"
#include "../Board.hpp"

namespace m8::search
{
    /// Class representing a specific search of a chess position
    class Search
    {
    public:
        /// Constructor
        /// 
        /// @param board        board of the initial position searched
        /// @param time_manager time_manager to use for this search. The class 
        ///                     will take ownership of the time_manager.
        inline Search(const Board& board,
                      std::unique_ptr<time::TimeManager> time_manager)
        : board_(board),
          time_manager_(std::move(time_manager))
        {}

        /// Returns the board representing the position searched
        const Board& board() const { return board_; }

        /// Return a reférence to the time manager for the search.
        time::TimeManager& time_manager() const { return *time_manager_; }
        
    private:
        Board board_;
        std::unique_ptr<time::TimeManager> time_manager_;    
    };
}

#endif // M8_SEARCH_SEARCH_HPP_