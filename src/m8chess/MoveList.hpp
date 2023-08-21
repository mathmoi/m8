/// @file MoveList.hpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date August 2023

#ifndef M8_CHESS_MOVE_LIST_HPP_
#define M8_CHESS_MOVE_LIST_HPP_

#include <array>

#include "Move.hpp"
#include "Types.hpp"

namespace m8
{
    /// Represents a move and it's evaluation in a MoveList.
    class MoveEvalPair
    {
    public:
        Move     move;
        EvalType eval;
    };

    /// Efficient container for a list of moves 
    class MoveList
    {
    public:
        /// Constructor
        inline MoveList()
        : size_(0)
        {}

        /// Append the given move to the list
        /// 
        /// @param move 
        inline void Push(Move move)
        {
            assert(size_ < kNumberOfMovesInMoveList);
            moves_[size_++].move = move;
        }

        /// Remove a move from the list
        inline Move Pop()
        {
            assert(0 < size_);
            return moves_[--size_].move;
        }

        /// Returns the last element of the list without removing it from the list.
        inline Move back() const
        {
            assert(0 < size_);
            return moves_[size_ - 1].move;
        }

        /// Pull a specified move to the front of the list.
        /// 
        /// Internally the move is put at the end of the list because we pop elements from
        /// the back for performance reasons.
        inline void PullFront(Move move)
        {
            auto pos = std::find_if(moves_.begin(),
                                    moves_.begin() + size_,
                                    [move](MoveEvalPair& pair){return pair.move == move;});
            std::rotate(pos, pos + 1,moves_.begin() + size_);
        }

        /// Erase the move at the specified position by copying the last element
        // of the list over it and resizing the list.
        inline void Erase(size_t pos)
        {
            assert(pos < size_);
            moves_[pos] = moves_[--size_];
        }

        /// Returns true if there is any moves in the list.
        inline bool any() const { return 0 < size_; }

        /// Returns the number of moves in the list.
        inline size_t size() const { return size_; }

        /// Returns an iterator to the begining
        inline const MoveEvalPair* begin() const { return moves_.begin(); }

        /// Returns an iterator to the end
        inline const MoveEvalPair* end() const { return moves_.begin() + size_; }

        /// Returns a reference to an element of the list
        inline MoveEvalPair& operator[](size_t pos)
        {
            assert(pos < size_);
            return moves_[pos];
        }
        
    private:
        static const size_t kNumberOfMovesInMoveList = 256;

        std::array<MoveEvalPair, kNumberOfMovesInMoveList> moves_;
        size_t size_;
    };
}

#endif // M8_CHESS_MOVE_LIST_HPP_