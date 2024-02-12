/// @file Search.hpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023

#ifndef M8_SEARCH_SEARCH_HPP_
#define M8_SEARCH_SEARCH_HPP_

#include <memory>

#include "../time/TimeManager.hpp"

#include "../Board.hpp"
#include "../Types.hpp"

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
                      std::unique_ptr<time::TimeManager> time_manager,
                      DepthType max_depth)
        : board_(board),
          time_manager_(std::move(time_manager)),
          max_depth_(max_depth),
          is_aborted_(false)
        {}

        /// Returns the board representing the position searched
        inline const Board& board() const { return board_; }

        /// Returns a reference to the time manager for the search.
        inline time::TimeManager& time_manager() const { return *time_manager_; }

        /// Returns the maximum depth for the search.
        inline DepthType max_depth() const { return max_depth_; }

        /// Indicate if the search is aborted
        inline bool is_aborted() const { return is_aborted_; }

        /// Abort the search
        inline void Abort() { is_aborted_ = true; }
        
    private:
        Board board_;
        std::unique_ptr<time::TimeManager> time_manager_;
        DepthType max_depth_;
        bool is_aborted_;
    };
}

#endif // M8_SEARCH_SEARCH_HPP_