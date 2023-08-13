/// @file MoveGenerator.hpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date August 2023

#ifndef M8_CHESS_MOVE_GENERATOR_HPP_
#define M8_CHESS_MOVE_GENERATOR_HPP_

#include <iterator>
#include <functional>

#include "../Board.hpp"
#include "../Move.hpp"

#include "MoveGeneration.hpp"

namespace m8::movegen
{
    /// Class responsible to generate moves 
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
                    first_ = moves_.data();
                    last_ = movegen::GenerateAllCaptures(generator_->board_, first_);
                    current_step_ = GenerationStep::DistributeCaptures;
                    if (first_ < last_)
                    {
                        return;
                    }
                    /* Intentionally ommited break */

                case GenerationStep::DistributeCaptures:
                    ++first_;
                    if (first_ < last_)
                    {
                        return;
                    }

                    // At this point there is no more captures to distribute. If we are
                    // in a qsearch we are done.
                    if (generator_->qsearch_)
                    {
                        current_step_ = GenerationStep::Done;
                        return;
                    }

                    current_step_ = GenerationStep::GenerateQuietMoves;
                    /* Intentionally ommited break */

                case GenerationStep::GenerateQuietMoves:
                    first_ = moves_.data();
                    last_ = movegen::GenerateAllQuietMoves(generator_->board_, first_);
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
                Done
            };

            const MoveGenerator* generator_;
            GenerationStep current_step_;
            MoveList moves_;
            Move* first_;
            Move* last_;
        };

        /// Constructor
        /// 
        /// @param board Position for which to generate moves.
        inline MoveGenerator(Board& board, bool qsearch)
        : board_(board),
          qsearch_(qsearch)
        {}

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
        Board& board_;
        bool qsearch_;
    
    };

    static_assert(std::input_iterator<MoveGenerator::Iterator>);
}

#endif // M8_CHESS_MOVE_GENERATOR_HPP_