/// @file TranspositionEntry.hpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023

#ifndef M8_TRANSPOSITION_TRANSPOSITION_ENTRY_HPP_
#define M8_TRANSPOSITION_TRANSPOSITION_ENTRY_HPP_

#include "../../m8common/Utils.hpp"

#include "../eval/Eval.hpp"

#include "../Move.hpp"
#include "../Types.hpp"

#include "EntryType.hpp"
#include "Zobrist.hpp"

namespace m8::transposition
{
    /// Represents an entry in the transposition table
    class TranspositionEntry
    {
    public:
        /// Default contructor
        TranspositionEntry() {}

        /// Constructor
        /// 
        /// @param key        Hash key for the position
        /// @param move       Best move in the position
        /// @param generation Generation of the entry. This can be used to 
        ///                   prefer recent entries when deciding how to overwrite.
        /// @param type       Type of evaluation in the entry (exact, lower
        ///                   bound, upper bound)
        /// @param depth      Depth of the search used to get the evaluation
        /// @param eval       Evaluation of the position
        inline TranspositionEntry(ZobristKey key, Move move, std::uint8_t generation, EntryType type, DepthType depth, DepthType distance, EvalType eval)
        {
            eval = eval::RemoveDistanceFromMate(eval, distance);
            std::uint64_t unsigned_eval = static_cast<std::int64_t>(eval) + (INT64_C(1) << (kEvalSize - 1));

            assert(static_cast<std::uint64_t>(move)          < (UINT64_C(1) << kMoveSize));
            assert(static_cast<std::uint64_t>(generation)    < (UINT64_C(1) << kGenerationSize));
            assert(static_cast<std::uint64_t>(type)          < (UINT64_C(1) << kTypeSize));
            assert(static_cast<std::uint64_t>(depth)         < (UINT64_C(1) << kDepthSize));
            assert(static_cast<std::uint64_t>(unsigned_eval) < (UINT64_C(1) << kEvalSize));

            data_ = (static_cast<std::uint64_t>(move)          << kMovePos)
                  | (static_cast<std::uint64_t>(generation)    << kGenerationPos)
                  | (static_cast<std::uint64_t>(type)          << kTypePos)
                  | (static_cast<std::uint64_t>(depth)         << kDepthPos)
                  | (static_cast<std::uint64_t>(unsigned_eval) << kEvalPos);

            key_ = key ^ data_; // For lock-less hash table, xor the data and the key.
        }

        /// Returns the hash key for the position
        inline ZobristKey key() const { return key_ ^ data_; }

        /// Returns the move.
        inline Move move() const { return (data_ & kMoveMask) >> kMovePos; }

        /// Returns the generation
        inline std::uint8_t generation() const { return (data_ & kGenerationMask) >> kGenerationPos; }

        /// Returns the type
        inline EntryType type() const { return static_cast<EntryType>((data_ & kTypeMask) >> kTypePos); }

        /// Returns the depth
        inline DepthType depth() const { return (data_ & kDepthMask) >> kDepthPos; }

        /// Returns the evaluation
        inline EvalType GetEval(DepthType distance) const
        {
            EvalType eval = static_cast<std::int64_t>((data_ & kEvalMask) >> kEvalPos) - (INT64_C(1) << (kEvalSize - 1));
            return eval::AddDistanceToMate(eval, distance);
        }

        /// Calculate the age of the entry based on the current generation.é
        inline std::uint8_t GetAge(std::uint8_t current_generation) const
        {
            return current_generation - generation();
        }
        
    private:
        static const std::uint64_t kMoveSize       = 26;
        static const std::uint64_t kGenerationSize = 8;
        static const std::uint64_t kTypeSize       = 2;
        static const std::uint64_t kDepthSize      = 12;
        static const std::uint64_t kEvalSize       = 16;

        static const std::uint64_t kMovePos       = 0;
        static const std::uint64_t kGenerationPos = kMovePos       + kMoveSize;
        static const std::uint64_t kTypePos       = kGenerationPos + kGenerationSize;
        static const std::uint64_t kDepthPos      = kTypePos       + kTypeSize;
        static const std::uint64_t kEvalPos       = kDepthPos      + kDepthSize;

        static const std::uint64_t kMoveMask       = CalculateMask(kMovePos, kMoveSize);
        static const std::uint64_t kGenerationMask = CalculateMask(kGenerationPos, kGenerationSize);
        static const std::uint64_t kTypeMask       = CalculateMask(kTypePos, kTypeSize);
        static const std::uint64_t kDepthMask      = CalculateMask(kDepthPos, kDepthSize);
        static const std::uint64_t kEvalMask       = CalculateMask(kEvalPos, kEvalSize);

        ZobristKey    key_;
        std::uint64_t data_;
    };
    
}

#endif // M8_TRANSPOSITION_TRANSPOSITION_ENTRY_HPP_