/// @file MoveGenerator.hpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date August 2023

#ifndef M8_CHESS_MOVE_GENERATOR_HPP_
#define M8_CHESS_MOVE_GENERATOR_HPP_

#include <algorithm>
#include <iterator>
#include <functional>

#include "../eval/Eval.hpp"

#include "../Board.hpp"
#include "../Move.hpp"
#include "../Types.hpp"

#include "MoveGeneration.hpp"
#include "MvvLva.hpp"

namespace m8::movegen
{
    /// Class responsible to generate moves
    template<bool root, bool qsearch>
    class MoveGenerator
    {
    public:
        /// Sentinel type use as the sentinel value for Iterator objects.
        class Sentinel {};

        /// Iterator class to iterate through the moves of a MoveGenerator
        class Iterator
        {
        public:
            using difference_type = std::ptrdiff_t;
            using value_type      = Move;

            /// Copy constructor
            /// 
            /// @param original Original instance to copy from
            inline Iterator(const Iterator& original)
            : generator_(original.generator_),
              current_step_(original.current_step_),
              moves_(original.moves_)
            {}

            /// Constructor for a new iterator
            ///
            /// @param generator Generator associated with the new iterator
            inline Iterator(const MoveGenerator& generator)
            : generator_(&generator),
              current_step_(GenerationStep::GenerateCaptures)
            {
                if (root)
                {
                    first_ = generator_->list.first_;
                    last_  = generator_->list.last_;
                    current_step_ = (first_ < last_ ? GenerationStep::DistributeListMoves
                                                    : GenerationStep::Done);
                    return;
                }
                
                // !root 
                GetNextMove();
            }

            /// Destructor
            inline ~Iterator() {}

            /// Copy assignement operator
            inline Iterator& operator=(const Iterator& rhs)
            {
                generator_    = rhs.generator_;
                current_step_ = rhs.current_step_;
                moves_        = rhs.moves_;
                return *this;
            }

            /// Pre-increment operator, increment the iterator to the next move.
            inline Iterator& operator++()
            {
                GetNextMove();
                return *this;
            }

            /// Post increment operator, increment the iterator to the next move, but
            /// returns the original value.
            inline Iterator operator++(int)
            { 
                auto copy(*this);
                ++(*this);
                return copy;
            }

            /// Comparision operator with the Sentinel type to check if we are done 
            /// iterating.
            inline bool operator==(Sentinel)
            {
                return current_step_ == GenerationStep::Done;
            }

            /// Returns the current move the iterator references.
            Move operator*() const
            {
                assert(first_ < last_);
                return *first_;
            }
        
        private:
            inline void GetNextMove()
            {
                // This switch statement implements a finite state machine that distribute
                // the moves in differents steps. The switch statement allow the iterator
                // to jump to the current step. When a step can no longer generates moves
                // the current_step_ value is update to the next step and the flow of 
                // execution continue to the next step (break keywords are not used) until
                // the next move is found.
                switch (current_step_)
                {
                case GenerationStep::GenerateCaptures:
                    // Generate all captures
                    first_ = moves_.data();
                    last_ = movegen::GenerateAllCaptures(*(generator_->board_), first_);
                    
                    // Score all captures using MVV/LVA, keep track of the best one and 
                    // move it to the front.
                    if (first_ < last_)
                    {
                        size_t indx_best = -1;
                        EvalType best_eval = eval::kMinEval;
                        for (ptrdiff_t x = 0; x < (last_ - first_); ++x)
                        {
                            evals_[x] = GetMvvLvaValue(moves_[x]);
                            if (best_eval < evals_[x])
                            {
                                best_eval = evals_[x];
                                indx_best = x;
                            }
                        }
                        std::swap(moves_[0], moves_[indx_best]);
                        std::swap(evals_[0], evals_[indx_best]);
                        current_step_ = GenerationStep::DistributeCaptures;
                        return;
                    }
                    /* Intentionally ommited break */

                case GenerationStep::DistributeCaptures:
                    // Ignoring the current move at the front of the list. Find the best
                    // captures remaining. Bring the best captures to the front of the 
                    // list and shorten the list by one element.
                    if (first_ < last_ - 1)
                    {
                        size_t indx_best = 1; // TODO : Check performance if we skip when 1 move remaining.
                        EvalType best_eval = eval::kMinEval;
                        for (ptrdiff_t x = 1; x < (last_ - first_); ++x)
                        {
                            if (best_eval < evals_[x])
                            {
                                best_eval = evals_[x];
                                indx_best = x;
                            }
                        }
                        auto indx_last = last_ - first_ - 1;
                        moves_[0] = moves_[indx_best];
                        evals_[0] = evals_[indx_best];
                        moves_[indx_best] = moves_[indx_last];
                        evals_[indx_best] = evals_[indx_last];
                        --last_;
                        return;
                    }

                    // At this point there is no more captures to distribute. If we are
                    // in a qsearch we are done.
                    if (qsearch)
                    {
                        current_step_ = GenerationStep::Done;
                        return;
                    }

                    current_step_ = GenerationStep::GenerateQuietMoves;
                    /* Intentionally ommited break */

                case GenerationStep::GenerateQuietMoves:
                    first_ = moves_.data();
                    last_ = movegen::GenerateAllQuietMoves(*(generator_->board_), first_);
                    current_step_ = GenerationStep::DistributeQuietMoves;
                    if (first_ < last_)
                    {
                        return;
                    }
                    /* Intentionally ommited break */

                case GenerationStep::DistributeQuietMoves:
                    ++first_;
                    if (first_ < last_)
                    {
                        return;
                    }
                    current_step_ = GenerationStep::Done;
                    return;
                    /* Intentionally ommited break */

                case GenerationStep::DistributeListMoves:
                    ++first_;
                    if (first_ < last_)
                    {
                        return;
                    }
                    current_step_ = GenerationStep::Done;
                    return;
                    /* Intentionally ommited break */

                case GenerationStep::Done:
                    /* Nothing to do */
                    break;
                };
            }

            /// Enumerations of the differents steps of move generation
            enum class GenerationStep {
                GenerateCaptures,
                DistributeCaptures,
                GenerateQuietMoves,
                DistributeQuietMoves,
                DistributeListMoves,
                Done
            };

            const MoveGenerator* generator_;
            GenerationStep current_step_;
            MoveList moves_;
            std::array<EvalType, kNumberOfMovesInMoveList> evals_;
            Move* first_;
            Move* last_;
        };

        /// Constructor
        /// 
        /// @param board Position for which to generate moves.
        inline MoveGenerator(Board& board)
        : board_(&board)
        {
            assert(!root);
        }

        /// Constructor from a pre-generated list. Can be use at the root of the search to
        /// use the pre-generated list provided by the iterative-deepening algorithm.
        /// 
        /// @param first fist move a pre-generatad list
        /// @param last  last move of a pre-generated list
        inline MoveGenerator(Move* first, Move* last)
        {
            assert(root);

            list.first_ = first;
            list.last_ = last;
        }

        /// Returns an iterator that can generate all moves
        inline Iterator begin() const
        {
            return Iterator(*this);
        }

        /// Returns a sentinel that can be compared to an iterator to detect if the 
        /// iterator is done generating moves.
        inline Sentinel end() const
        {
            return Sentinel();
        }
        
    private:
        union
        {
            Board* board_;
            struct
            {
                Move* first_;
                Move* last_;
            } list;
        };
    };

    static_assert(std::input_iterator<MoveGenerator<true, false>::Iterator>);
    static_assert(std::input_iterator<MoveGenerator<false, false>::Iterator>);
    static_assert(std::input_iterator<MoveGenerator<false, true>::Iterator>);
}

#endif // M8_CHESS_MOVE_GENERATOR_HPP_