/// @file Perft.cpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023

// REMOVE
#include <iostream>

#include <algorithm>

#include "../m8/options/Options.hpp"

#include "Checkmate.hpp"
#include "MoveGen.hpp"
#include "SAN.hpp"

#include "Perft.hpp"

namespace m8
{
    void PerftMove::MakeDone()
    {
        assert(status_ == PerftMoveStatus::Shared);
        assert(searcher_ == 0);
        count_ = node_->count();
        node_.reset();
        status_ = PerftMoveStatus::Done;
    }

    PerftNode::PerftNode(Board& board)
    {
        GenerateMoves(board);
    }

    void PerftNode::GenerateMoves(Board& board)
    {
        MoveList moves;
        Move* last = GenerateAllMoves(board, moves.data());

        for (Move* next = moves.data(); next < last; ++next)
        {
            UnmakeInfo unmake_info = board.Make(*next);

            if (!IsInCheck(OpposColor(board.side_to_move()), board))
            {
                moves_.emplace_back(*next);
            }

            board.Unmake(*next, unmake_info);
        }
    }

    void Perft::StartThreads()
    {
        int threads = options::Options::get().perft.threads;
        
        for (int i = 0; i < threads; ++i)
        {
            threads_.push_back(std::thread(&Perft::RunThread, this));
        }
    }

    void Perft::Start()
    {
        start_ = std::chrono::steady_clock::now();
        StartThreads();
    }

    std::uint64_t Perft::RecursivePerft(Board& board, int depth)
    {
        std::uint64_t count = 0;

        MoveList moves;
        Move* last = GenerateAllMoves(board, moves.data());

        for (Move* next = moves.data(); next < last && !abort_; ++next)
        {
            UnmakeInfo unmake_info = board.Make(*next);

            if (!IsInCheck(OpposColor(board.side_to_move()), board))
            {
                count += (depth == 1 ? 1 : RecursivePerft(board, depth - 1));
            }

            board.Unmake(*next, unmake_info);
        }

        return count;
    }

    void Perft::CalculateAtSharedNode(PerftNode& node, Board& board, int depth)
    {
        // Find a new move that we can start to search on our own
        auto new_moves = node | std::views::filter([](const PerftMove& move){ return move.status() == PerftMoveStatus::New; });
        for (auto& perft_move : new_moves)
        {
            perft_move.MakeSearching();

            UnmakeInfo unmake_info = board.Make(perft_move.move());

            if (!IsInCheck(OpposColor(board.side_to_move()), board))
            {
                mutex_.unlock();
                auto count = RecursivePerft(board, depth - 1);
                mutex_.lock();
                
                perft_move.MakeDone(count);
            }
            else
            {
                perft_move.MakeDone(0);
            }

            board.Unmake(perft_move.move(), unmake_info);
        }
    }

    void Perft::FindNodeToHelp(PerftNode& node, Board& board, int depth, bool is_root = false)
    {
        auto new_moves = node | std::views::filter([](const PerftMove& move){ return move.status() == PerftMoveStatus::New; });
        for (auto& perft_move : new_moves)
        {
            UnmakeInfo unmake_info = board.Make(perft_move.move());

            if (!IsInCheck(OpposColor(board.side_to_move()), board))
            {
                if (depth == 1)
                {
                    perft_move.MakeDone(1);
                }
                else if (kMinParallelDepth < depth)
                {
                    perft_move.MakeShared(std::make_unique<PerftNode>(board));
                    perft_move.AddSearcher();
                    CalculateAtSharedNode(perft_move.node(), board, depth - 1);
                    perft_move.RemoveSearcher();

                    // If the node we just calculate at is done we make the move done.
                    if (perft_move.node().done())
                    {
                        perft_move.MakeDone();
                    }
                }
                else
                {
                    perft_move.MakeSearching();

                    mutex_.unlock();
                    auto count = RecursivePerft(board, depth - 1);
                    mutex_.lock();
            
                    perft_move.MakeDone(count);
                }
            }
            else
            {
                perft_move.MakeDone(0);
            }

            board.Unmake(perft_move.move(), unmake_info);

            if (is_root && perft_move.status() == PerftMoveStatus::Done)
            {
                auto san_move = RenderSAN(perft_move.move(), board_);
                observer_->OnPartialPerftResult(san_move, perft_move.count());
            }
        }

        auto shared_moves = node | std::views::filter([](const PerftMove& move){ return move.status() == PerftMoveStatus::Shared; });
        for (auto& perft_move : shared_moves)
        {
            UnmakeInfo unmake_info = board.Make(perft_move.move());

            perft_move.AddSearcher();
            FindNodeToHelp(perft_move.node(), board, depth - 1);
            perft_move.RemoveSearcher();

            board.Unmake(perft_move.move(), unmake_info);

            // If the node we just calculate at is done we make the move done.
            if (perft_move.node().done())
            {
                perft_move.MakeDone();

                if (is_root)
                {
                    auto san_move = RenderSAN(perft_move.move(), board_);
                    observer_->OnPartialPerftResult(san_move, perft_move.count());
                }
            }
        }
    }

    void Perft::SendResult()
    {
        auto end = std::chrono::steady_clock::now();
        auto duration = duration_cast<std::chrono::duration<double>>(end - start_);

        observer_->OnPerftCompleted(root_.count(), duration.count());
    }

    void Perft::RunThread()
    {
        mutex_.lock();

        Board board = board_;
        FindNodeToHelp(root_, board, depth_, true);

        if (root_.done())
        {
            SendResult();
        }

        mutex_.unlock();
    }

    void Perft::JoinThreads()
    {
        for (auto& thread : threads_)
        {
            if (thread.joinable())
            {
                if (thread.get_id() == std::this_thread::get_id())
                {
                    thread.detach();
                }
                else
                {
                    thread.join();
                }
            }
        }
    }
}