/// @file TranspositionEntry_test.cpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023

#include "catch.hpp"

#include "../m8chess/eval/Eval.hpp"

#include "../m8chess/transposition/TranspositionEntry.hpp"

using namespace m8;
using namespace m8::eval;
using namespace m8::transposition;

TEST_CASE("TranspositionEntry_StoreData_RetrieveCorrectData")
{
    ZobristKey key          = UINT64_C(0xeadd8e089d843fc3);
    Move move               = NewMove(kE2, kE4, kWhitePawn);
    std::uint8_t generation = 222;
    EntryType type          = EntryType::Exact;
    DepthType depth         = 20;
    DepthType distance      = 3;
    EvalType eval           = 100;

    SECTION ("Min generation") { generation = 0; }
    SECTION ("Max generation") { generation = 255; }
    SECTION ("Type Exact")     { type = EntryType::Exact; }
    SECTION ("Type LowerBound")     { type = EntryType::LowerBound; }
    SECTION ("Type UpperBound")      { type = EntryType::UpperBound; }
    SECTION ("Min depth")      { depth = 0; }
    SECTION ("Max depth")      { depth = 4095; }
    SECTION ("Min eval")       { eval = -kEvalMat; }
    SECTION ("Max eval")       { eval = +kEvalMat; }

    TranspositionEntry sut(key, move, generation, type, depth, distance, eval);

    auto actual_key        = sut.key();
    auto actual_move       = sut.move();
    auto actual_generation = sut.generation();
    auto actual_type       = sut.type();
    auto actual_depth      = sut.depth();
    auto actual_eval       = sut.GetEval(distance);

    REQUIRE(key == actual_key);
    REQUIRE(move == actual_move);
    REQUIRE(generation == actual_generation);
    REQUIRE(type == actual_type);
    REQUIRE(depth == actual_depth);
    REQUIRE(eval == actual_eval);
}