/// @file Bucket.hpp
/// @author Mathieu Pagé
/// @date April 2024
/// Contains The Bucket class representing a bucket in the transposition table.

#ifndef M8_TRANSPOSITION_BUCKET_HPP_
#define M8_TRANSPOSITION_BUCKET_HPP_

#include "TranspositionEntry.hpp"

namespace m8::transposition
{
    /// Represent a bucket in the transposition table.
    class alignas(64) Bucket
    {
    public:
        /// Get from the bucket the transposition table entry corresponding to the hash
        /// key.
        /// 
        /// @param key Hash key of the position
        /// @return A pointer to the position or nullptr if no entries in the bucket 
        ///         correspond to the position.
        inline TranspositionEntry* operator[](ZobristKey key)
        {
            if (always_replace_entry_.key() == key)
            {
                return &always_replace_entry_;
            }

            for (int i = 0; i < 3; ++i)
            {
                if (depth_prefered_entries_[i].key() == key)
                {
                    return &depth_prefered_entries_[i];
                }
            }

            return nullptr;
        }

        /// Store a new position in the bucket
        /// 
        /// @param key        Hash key for the position
        /// @param move       Best move in the position
        /// @param generation Current generation of the search. This can be used to prefer
        ///                   entries of the current seatch to entries of a previous search
        ///                   when deciding how to overwrite.
        /// @param type       Type of evaluation in the entry (exact, lower bound, upper
        ///                   bound)
        /// @param depth      Depth of the search used to get the evaluation
        /// @param eval       Evaluation of the position
        inline void Insert(ZobristKey key, Move move, std::uint8_t generation, EntryType type, DepthType depth, DepthType distance, EvalType eval)
        {
            if (always_replace_entry_.key() != key)
            {
                auto candidate = std::min_element(std::begin(depth_prefered_entries_),
                                                  std::end(depth_prefered_entries_),
                                                  [generation, key](const TranspositionEntry& lhs, const TranspositionEntry& rhs)
                                                  {
                                                    // Returns true if lhs is a better candidate than rhs to store the new position.
                                                    return lhs.key() == key ||
                                                           (
                                                             rhs.key() != key &&
                                                             (
                                                               lhs.GetAge(generation) > rhs.GetAge(generation) ||
                                                               (
                                                                 lhs.GetAge(generation) == rhs.GetAge(generation) &&
                                                                 lhs.depth() < rhs.depth()
                                                               )
                                                             )
                                                           );                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
                                                  });

                if (candidate->key() == key ||
                    candidate->generation() != generation ||
                    candidate->depth() <= depth)
                {
                    *candidate = TranspositionEntry(key, move, generation, type, depth, distance, eval);
                    return;
                }
            }

            always_replace_entry_ = TranspositionEntry(key, move, generation, type, depth, distance, eval);
        }
        
    private:
        TranspositionEntry depth_prefered_entries_[3];
        TranspositionEntry always_replace_entry_;
    };
}

#endif // M8_TRANSPOSITION_BUCKET_HPP_