/// @file Perft.cpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023

#include <algorithm>

#include "../m8/options/Options.hpp"

#include "Checkmate.hpp"
#include "MoveGen.hpp"
#include "SAN.hpp"

#include "Perft.hpp"

namespace m8
{
    void PerftMove::CheckSharedDone()
    {
        assert(status_ == PerftMoveStatus::Shared);
        if (node_->done())
        {
            count_ = node_->count();
            node_.reset();
            status_ = PerftMoveStatus::Done;
        }
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

    void Perft::PerftMoveRecursive(PerftMove& move, Board& board, int depth)
    {
        move.MakeSearching();

        mutex_.unlock();
        auto count = RecursivePerft(board, depth);
        mutex_.lock();

        move.MakeDone(count);
    }

    void Perft::ParallelPerft(PerftNode& node,
                              Board& board,
                              int depth,
                              PerftMoveStatus node_type,
                              bool is_root,
                              std::function<void(PerftMove&, Board&, int)> recurse)
    {
        auto moves = node | std::views::filter([node_type](const PerftMove& move){ return move.status() == node_type; });
        for (auto& move : moves)
        {
            if (abort_)
            {
                break;
            }

            UnmakeInfo unmake_info = board.Make(move.move());

            if (!IsInCheck(OpposColor(board.side_to_move()), board))
            {
                if (depth == 1)
                {
                    move.MakeDone(1);
                }
                else if (kMinParallelDepth < depth)
                {
                    recurse(move, board, depth - 1);
                }
                else
                {
                    PerftMoveRecursive(move, board, depth - 1);
                }
            }
            else
            {
                move.MakeDone(0);
            }

            board.Unmake(move.move(), unmake_info);

            if (is_root && !abort_ && move.status() == PerftMoveStatus::Done)
            {
                auto san_move = RenderSAN(move.move(), board_);
                observer_->OnPartialPerftResult(san_move, move.count());
            }
        }
    }

    void Perft::ContributeAtSharedNode(PerftNode& node, Board& board, int depth)
    {
        ParallelPerft(node, board, depth, PerftMoveStatus::New, false,
            [this](PerftMove& move, Board& board, int depth)
            {
                PerftMoveRecursive(move, board, depth);
            });
    }

    void Perft::FindNodeToContribute(PerftNode& node, Board& board, int depth, bool is_root = false)
    {
        // At the current node we first look for a New child that we can transform in a 
        // Shared child and start work on it.
        ParallelPerft(node, board, depth, PerftMoveStatus::New, is_root,
            [this](PerftMove& move, Board& board, int depth)
            {
                move.MakeShared(std::make_unique<PerftNode>(board));
                ContributeAtSharedNode(move.node(), board, depth);
                move.CheckSharedDone();
            });

        // When there is no more New children, we look for a shared child to contribute to
        ParallelPerft(node, board, depth, PerftMoveStatus::Shared, is_root,
            [this](PerftMove& move, Board& board, int depth)
            {
                FindNodeToContribute(move.node(), board, depth);
                move.CheckSharedDone();
            });
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

        if (!root_.done())
        {
            Board board = board_;
            FindNodeToContribute(root_, board, depth_, true);

            if (root_.done() && !abort_)
            {
                SendResult();
            }
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