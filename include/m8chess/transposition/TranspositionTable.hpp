/// @file TranspositionTable.hpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023

#ifndef M8_TRANSPOSITION_TRANSPOSITION_TABLE_HPP_
#define M8_TRANSPOSITION_TRANSPOSITION_TABLE_HPP_

#include <algorithm>
#include <cassert>
#include <cstring>
#include <vector>

#include "../../m8common/Bb.hpp"

#include "Bucket.hpp"

namespace m8::transposition
{
    /// Transposition table
    class TranspositionTable
    {
    public:
        inline TranspositionTable(size_t size)
        : data_(nullptr),
          buckets_count_(0),
          generation_(0)
        {
            assert(64 == sizeof(Bucket));
            assert(1024 <= size);

            Resize(size);
        }

        inline ~TranspositionTable()
        {
            if (data_ != nullptr)
            {
                std::free(data_);
            }
        }

        /// Increment the current generation. This need to be call once between each
        /// search to increment the current generation value. This is usefull to
        /// differentiate entry from the current search from entry of previous search.
        inline void IncrementGeneration() { ++generation_; }

        /// Returns a pointer to the entry in the transposition table corresponding to the
        /// key passed in parameters. If there is no information stored for the current
        /// position a null pointer is retured.
        inline TranspositionEntry* operator[](ZobristKey key)
        {
            auto& bucket = data_[key & mask_];
            return bucket[key];
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
            auto& bucket = data_[key & mask_];
            bucket.Insert(key, move, generation_, type, depth, distance, eval);
        }

        /// Resize the hash table.
        ///
        /// @param size Size of the hash table in bytes.
        inline void Resize(size_t size)
        {
            assert(1024 <= size);

            auto new_count = CalculateNumberEntry(size);
            if (new_count != buckets_count_)
            {
                if (data_ != nullptr)
                {
                    std::free(data_);
                }

                buckets_count_ = new_count;
                data_ = static_cast<Bucket*>(std::aligned_alloc(64, buckets_count_ * sizeof(Bucket)));
                mask_ = buckets_count_ - 1;
            }
        }

        /// Remove all data from the hash table. This is not normally needed as outdated
        /// data will usually not have a negative impact on performance. This is needed
        /// when running benchmarks reusing the sames positions multiples times. If the
        /// table is not zeroed-out there is a chance that the memory allocated for the
        /// table was used by a previous table and contains valid entries data. When this
        /// happens it will positively incluence the performance and make the benchmark
        /// unreliable.
        inline void Empty()
        {
#           pragma GCC diagnostic push
#           pragma GCC diagnostic ignored "-Wclass-memaccess"

            std::memset(data_, 0, buckets_count_ * sizeof(Bucket));
            
#           pragma GCC diagnostic pop
        }
        
    private:
        static inline const size_t kAssumedCacheLineSize = 64;
        static inline const size_t kMinSizeTable = 4 * 1024 * 1024;

        Bucket*      data_;
        size_t       buckets_count_;
        ZobristKey   mask_;
        std::uint8_t generation_;

        inline static size_t CalculateNumberEntry(size_t size)
        {
            return (UINT64_C(1) << GetMsb((std::max)(size, kMinSizeTable))) / sizeof(Bucket);
        }
    };
}

#endif // M8_TRANSPOSITION_TRANSPOSITION_TABLE_HPP_