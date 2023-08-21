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
#include "../MoveLegality.hpp"
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
              current_step_(GenerationStep::UseTranspositionMove)
            {
                if (root)
                {
                    moves_ = *(generator_->moves_);
                    current_step_ = GenerationStep::OnlyDistributeMoves;
                }
                
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
                assert(current_step_ != GenerationStep::Done);
                return current_move_;
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
                case GenerationStep::UseTranspositionMove:
                    if (IsPseudoLegal(*(generator_->data_.board_), generator_->data_.tt_move_))
                    {
                        current_move_ = generator_->data_.tt_move_;
                        current_step_ = GenerationStep::GenerateCaptures;
                        return;
                    }
                    current_step_ = GenerationStep::GenerateCaptures;
                    /* Intentionally ommited break */

                case GenerationStep::GenerateCaptures:
                    // Generate all captures
                    movegen::GenerateAllCaptures(*(generator_->data_.board_), moves_);
                    
                    // Score all captures using MVV/LVA, keep track of the best one and
                    // make it the current move.
                    {
                        size_t indx_best = -1;
                        EvalType best_eval = eval::kMinEval;
                        for (size_t x = moves_.size() -1; x < moves_.size(); --x)
                        {
                            if (generator_->data_.tt_move_ == moves_[x].move)
                            {
                                moves_.Erase(x);
                            }
                            else
                            {
                                moves_[x].eval = GetMvvLvaValue(moves_[x].move);
                                if (best_eval < moves_[x].eval)
                                {
                                    best_eval = moves_[x].eval;
                                    indx_best = x;
                                }
                            }
                        }
                        if (best_eval != eval::kMinEval)
                        {
                            current_move_ = moves_[indx_best].move;
                            moves_.Erase(indx_best);
                            current_step_ = GenerationStep::DistributeCaptures;
                            return;
                        }
                    }

                    current_step_ = GenerationStep::DistributeCaptures;
                    /* Intentionally ommited break */

                case GenerationStep::DistributeCaptures:
                    // Find the best capture and make it the current move
                    if (moves_.any())
                    {
                        size_t indx_best = 0;
                        EvalType best_eval = eval::kMinEval;
                        for (size_t x = 0; x < moves_.size(); ++x)
                        {
                            if (best_eval < moves_[x].eval)
                            {
                                best_eval = moves_[x].eval;
                                indx_best = x;
                            }
                        }
                        current_move_ = moves_[indx_best].move;
                        moves_.Erase(indx_best);
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
                    movegen::GenerateAllQuietMoves(*(generator_->data_.board_), moves_);
                    current_step_ = GenerationStep::DistributeRemainingMoves;
                    /* Intentionally ommited break */

                case GenerationStep::DistributeRemainingMoves:
                    // Remove the TT move if it's the next move
                    if (moves_.any() && generator_->data_.tt_move_ == moves_.back())
                    {
                        moves_.Pop();
                    }

                    if (moves_.any())
                    {
                        current_move_ = moves_.Pop();
                        return;
                    }
                    current_step_ = GenerationStep::Done;
                    return;
                    /* Intentionally ommited break */

                case GenerationStep::OnlyDistributeMoves:
                    if (moves_.any())
                    {
                        current_move_ = moves_.Pop();
                        return;
                    }
                    current_step_ = GenerationStep::Done;
                    return;

                case GenerationStep::Done:
                    /* Nothing to do */
                    break;
                };
            }

            /// Enumerations of the differents steps of move generation
            enum class GenerationStep {
                UseTranspositionMove,
                GenerateCaptures,
                DistributeCaptures,
                GenerateQuietMoves,
                DistributeRemainingMoves,
                OnlyDistributeMoves,
                Done
            };

            const MoveGenerator* generator_;
            GenerationStep current_step_;
            MoveList moves_;
            Move current_move_;
        };

        /// Constructor
        /// 
        /// @param board   Position for which to generate moves.
        /// @param tt_move Best move from the transposition table if one is available
        inline MoveGenerator(Board& board,
                             Move tt_move = kNullMove)
        {
            assert(!root);
            data_.board_ = &board;
            data_.tt_move_ = tt_move;
        }

        /// Constructor from a pre-generated list. Can be use at the root of the search to
        /// use the pre-generated list provided by the iterative-deepening algorithm.
        /// 
        /// @param first fist move a pre-generatad list
        /// @param last  last move of a pre-generated list
        inline MoveGenerator(const MoveList& moves)
        {
            assert(root);
            moves_ = &moves;
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
            struct {
                Board* board_;
                Move tt_move_;
            } data_;
            const MoveList* moves_;
        };
    };

    static_assert(std::input_iterator<MoveGenerator<true, false>::Iterator>);
    static_assert(std::input_iterator<MoveGenerator<false, false>::Iterator>);
    static_assert(std::input_iterator<MoveGenerator<false, true>::Iterator>);
}

#endif // M8_CHESS_MOVE_GENERATOR_HPP_