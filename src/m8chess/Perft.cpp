/// @file   Perft.cpp
/// @author Mathieu Pag�
/// @date   Janurary 2018
/// @brief  Contains functionalities of the perft command.

#include <chrono>
#include <future>
#include <iostream> // TODO : Remove this include

#include "Perft.hpp"
#include "MoveGen.hpp"
#include "Checkmate.hpp"

namespace m8
{
    std::uint64_t Perft::RunPerft(int depth, Board& board, const MoveGen& move_gen)
    {
        std::uint64_t count = 0;

        MoveList moves;
        Move* last = move_gen.GenerateAllMoves(moves.data());

        for (Move* next = moves.data(); next < last; ++next)
        {
            UnmakeInfo unmake_info = board.Make(*next);

            if (!IsInCheck(OpposColor(board.side_to_move()), board, move_gen))
            {
                count += (depth == 1 ? 1 : RunPerft(depth - 1, board, move_gen));
            }

            board.Unmake(*next, unmake_info);
        }

        return count;
    }

    int Perft::AddLayer(PerftNode::Ptr node, Board& board, const MoveGen& move_gen)
    {
        int count = 0;

        UnmakeInfo unmake_info;
        if (node->move() != kNullMove)
        {
            unmake_info = board.Make(node->move());
        }


        if (node->children().size() > 0)
        {
            for (auto child : node->children())
            {
                count += AddLayer(child, board, move_gen);
            }
        }
        else
        {
            MoveList moves;
            Move* last = move_gen.GenerateAllMoves(moves.data());

            for (Move* next = moves.data(); next < last; ++next)
            {
                UnmakeInfo unmake_info = board.Make(*next);

                if (!IsInCheck(OpposColor(board.side_to_move()), board, move_gen))
                {
                    node->AddChild(std::make_shared<PerftNode>(node, *next, node->depth() - 1));
                    ++count;
                }

                board.Unmake(*next, unmake_info);
            }
        }

        if (node->move() != kNullMove)
        {
            board.Unmake(node->move(), unmake_info);
        }

        return count;
    }

    void Perft::CreateRootNodes()
    {

        MoveGen move_gen(board_);

        root_ = std::make_shared<PerftNode>(nullptr, kNullMove, depth_);
        int count;
        int layers = 0;
        do
        {
            count = AddLayer(root_, board_, move_gen);
            ++layers;
        } while (layers < depth_ && count < 100); // TODO : Constant should be a parameter
    }

    bool Perft::ReserveNode(PerftNode::Ptr node)
    {
        bool success = false;

        if (node->mutex().try_lock())
        {
            if (node->state() == PerftNodeState::Available)
            {
                node->set_state(PerftNodeState::WorkedOn);
                success = true;
            }
            node->mutex().unlock();
        }

        return success;
    }

    PerftNode::Ptr Perft::PickNode(PerftNode::Ptr node)
    {
        if (node->children().size() == 0 && node->state() == PerftNodeState::Available)
        {
            if (ReserveNode(node))
            {
                return node;
            }
        }
        else
        {
            for (auto child : node->children())
            {
                auto picked = PickNode(child);
                if (picked)
                {
                    return picked;
                }
            }
        }

        return nullptr;
    }

    void Perft::GetBoard(Board& board, PerftNode::Ptr node)
    {
        if (node->parent())
        {
            GetBoard(board, node->parent());
        }

        if (node->move() != kNullMove)
        {
            board.Make(node->move());
        }
    }

    void Perft::PropagateResult(PerftNode::Ptr node)
    {
        std::lock_guard<std::mutex> lock(node->mutex());

        node->AddChildDone();

        if (node->children_done() == node->children().size())
        {
            CompleteInternalNode(node);
        }
    }

    void Perft::CompleteInternalNode(PerftNode::Ptr node)
    {
        std::uint64_t sum = CalculateSumChildren(node);

        node->set_count(sum);
        node->set_state(PerftNodeState::Done);

        if (node->parent())
        {
            PropagateResult(node->parent());
        }

        if (node->parent() == root_)
        {
            SendPartialResult(node->move(), sum);
        }
    }

    std::uint64_t Perft::CalculateSumChildren(PerftNode::Ptr node)
    {
        std::uint64_t sum = 0;
        for (auto child : node->children())
        {
            sum += child->count().get();
        }
        return sum;
    }

    void Perft::ComputeNode(PerftNode::Ptr node)
    {
        Board board(board_);
        GetBoard(board, node);
        MoveGen move_gen(board);

        std::uint64_t count = (node->depth() > 0 ? RunPerft(node->depth(), board, move_gen) : 1);

        node->set_count(count);
        node->set_state(PerftNodeState::Done);

        if (node->parent())
        {
            PropagateResult(node->parent());
        }
    }

    void Perft::RunWorkerThread()
    {
        PerftNode::Ptr node = PickNode(root_);
        while (node)
        {
            ComputeNode(node);
            if (node->parent() == root_)
            {
                SendPartialResult(node->move(), node->count().get());
            }
            
            node = PickNode(root_);
        }
    }

    std::vector<std::future<void>> Perft::StartThreads()
    {
        std::vector<std::future<void>> futures;
        for (int i = 0; i < 4; ++i) // TODO: Number of threads should be an option.
        {
            futures.push_back(std::async(std::launch::async, &Perft::RunWorkerThread, this));
        }

        return futures; 
    }

    void Perft::JoinThreads(std::vector<std::future<void>>& futures)
    {
        for (auto& future : futures)
        {
            future.get();
        }
    }

    PerftResult Perft::CreateResult(const std::chrono::duration<double> &time_span)
    {
        PerftResult result;
        
        result.nodes = root_->count().get();
        result.seconds = time_span.count();

        return result;
    }

    PerftResult Perft::RunParallel()
    {
        using namespace std::chrono;

        CreateRootNodes();

        auto start = high_resolution_clock::now();

        auto futures = StartThreads();
        JoinThreads(futures);

        auto end = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(end - start);
        
        return CreateResult(time_span);
    }

    void Perft::SendPartialResult(Move move, std::uint64_t count)
    {
        std::lock_guard<std::mutex> lock(callback_mutex_);

        callback_(move, count);
    }
}