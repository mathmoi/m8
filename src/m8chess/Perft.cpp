/// @file   Perft.cpp
/// @author Mathieu Pag�
/// @date   Janurary 2018
/// @brief  Contains functionalities of the perft command.

#include <chrono>

#include "Perft.hpp"
#include "MoveGen.hpp"
#include "Checkmate.hpp"

namespace m8
{
    template <bool is_root = false>
    std::uint64_t Perft(int depth, Board& board, MoveGen& generator, std::function<void(Move, std::uint64_t)> callback)
    {
        std::uint64_t count = 0;

        MoveList moves;
        Move* last = generator.GenerateAllMoves(moves.data());

        for (Move* next = moves.data(); next < last; ++next)
        {
            std::uint64_t child_count = 0;

            UnmakeInfo unmake_info = board.Make(*next);

            if (!IsInCheck(OpposColor(board.side_to_move()), board, generator))
            {
                if (depth == 1)
                {
                    child_count = 1;
                }
                else
                {
                    child_count = Perft(depth - 1, board, generator, nullptr);
                }
            }

            board.Unmake(*next, unmake_info);

            if (is_root)
            {
                callback(*next, child_count);
            }

            count += child_count;
        }

        return count;
    }

    PerftResult Perft(int depth, Board& board, std::function<void(Move, std::uint64_t)> callback)
    {
        using namespace std::chrono;

        PerftResult result;

        auto start = high_resolution_clock::now();
        MoveGen generator(board);
        result.nodes = Perft<true>(depth, board, generator, callback);
        auto end = high_resolution_clock::now();

        duration<double> time_span = duration_cast<duration<double>>(end - start);
        result.seconds = time_span.count();

        return result;
    }
}