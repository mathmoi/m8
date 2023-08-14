/// @file Eval.cpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date August 2023

#include "../../m8/options/Options.hpp"

#include "Eval.hpp"

namespace m8::eval
{
    std::array<EvalType, kMaxPieceType + 1> kPieceTypeValues;

    void InitializeEval()
    {
        kPieceTypeValues[kNoPieceType] = 0;
        
        for (auto piece_type : all_piece_types)
        {
            kPieceTypeValues[piece_type] = options::Options::get().eval.get_value_piece_type(piece_type);
        }
    }
}