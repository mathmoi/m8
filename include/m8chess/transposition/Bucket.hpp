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
            auto entry = std::find_if(entries_.begin(), entries_.end(), [key](const TranspositionEntry entry) {return entry.key() == key; });
            return entry != entries_.end() ? entry : nullptr;
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
            auto entry = std::min_element(std::begin(entries_),
                                          std::end(entries_),
                                          [generation, key](const TranspositionEntry& lhs, const TranspositionEntry rhs)
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
            *entry = TranspositionEntry(key, move, generation, type, depth, distance, eval);
        }
        
    private:
        static const std::size_t kNumberOfEntries = 4;

        std::array<TranspositionEntry, kNumberOfEntries> entries_;
    };
}

#endif // M8_TRANSPOSITION_BUCKET_HPP_