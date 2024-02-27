/// @file TranspositionTable.hpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023

#ifndef M8_TRANSPOSITION_TRANSPOSITION_TABLE_HPP_
#define M8_TRANSPOSITION_TRANSPOSITION_TABLE_HPP_

#include <algorithm>
#include <cassert>
#include <vector>

#include "../../m8common/Bb.hpp"

#include "TranspositionEntry.hpp"

namespace m8::transposition
{
    /// Transposition table
    class TranspositionTable
    {
    public:
        TranspositionTable(size_t size)
        : data_(CalculateNumberEntry(size)),
          mask_(data_.size() - 1),
          generation_(0)
        {
            assert(16 == sizeof(TranspositionEntry));
            assert(1024 <= size);
        }

        /// Increment the current generation. This need to be call once between each
        /// search to increment the current generation value. This is usefull to
        /// differentiate entry from the current search from entry of previous search.
        inline void IncrementGeneration() { generation_ = (generation_ + 1) & kGenerationMask; }

        /// Returns a pointer to the entry in the transposition table corresponding to the
        /// key passed in parameters. If there is no information stored for the current
        /// position a null pointer is retured.
        inline TranspositionEntry* operator[](ZobristKey key)
        {
            auto entry = &data_[key & mask_];
            return entry->key() == key ? entry : nullptr;
        }

        /// Insert an entry in the transposition table.
        /// 
        /// @param key        Hash key for the position
        /// @param move       Best move in the position
        /// @param generation Generation of the entry. This can be used to 
        ///                   prefer recent entries when deciding how to overwrite.
        /// @param type       Type of evaluation in the entry (exact, lower
        ///                   bound, upper bound)
        /// @param depth      Depth of the search used to get the evaluation
        /// @param distance   Distance from the root of the search
        /// @param eval       Evaluation of the position
        inline void Insert(ZobristKey key, Move move, EntryType type, DepthType depth, DepthType distance, EvalType eval)
        {
            data_[key & mask_] = TranspositionEntry(key, move, generation_, type, depth, distance, eval);
        }

        /// Resize the hash table.
        ///
        /// @param size Size of the hash table in bytes.
        inline void Resize(size_t size)
        {
            assert(1024 <= size);

            data_.resize(CalculateNumberEntry(size));
            data_.shrink_to_fit();
            mask_ = data_.size() - 1;
        }
        
    private:
        static inline const size_t kMinSizeTable = 4 * 1024 * 1024;
        static inline const std::uint8_t kGenerationMask = 0x3f;

        std::vector<TranspositionEntry> data_;
        ZobristKey                      mask_;
        std::uint8_t                    generation_;

        inline static size_t CalculateNumberEntry(size_t size)
        {
            return (UINT64_C(1) << GetMsb((std::max)(size, kMinSizeTable))) / sizeof(TranspositionEntry);
        }
    };
}

#endif // M8_TRANSPOSITION_TRANSPOSITION_TABLE_HPP_