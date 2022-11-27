/// @file   Perft.cpp
/// @author Mathieu Pag�
/// @date   Janurary 2018
/// @brief  Contains functionalities of the perft command.

#include <chrono>
#include <future>

#include "../m8/options/Options.hpp"
#include "Perft.hpp"
#include "MoveGen.hpp"
#include "Checkmate.hpp"
#include "../m8common/logging.hpp"

namespace m8
{
    Perft::~Perft()
    {
        JoinThreads();
    }

    std::uint64_t Perft::RunPerft(int depth, Board& board)
    {
        std::uint64_t count = 0;

        MoveList moves;
        Move* last = GenerateAllMoves(board, moves.data());

        for (Move* next = moves.data(); next < last && !abort_; ++next)
        {
            UnmakeInfo unmake_info = board.Make(*next);

            if (!IsInCheck(OpposColor(board.side_to_move()), board))
            {
                count += (depth == 1 ? 1 : RunPerft(depth - 1, board));
            }

            board.Unmake(*next, unmake_info);
        }

        return count;
    }

    int Perft::AddLayer(PerftNode::Ptr node, Board& board)
    {
        int count = 0;

        UnmakeInfo unmake_info = 0;
        if (node->move() != kNullMove)
        {
            unmake_info = board.Make(node->move());
        }

        if (node->children().size() > 0)
        {
            for (auto child : node->children())
            {
                count += AddLayer(child, board);
            }
        }
        else
        {
            MoveList moves;
            Move* last = GenerateAllMoves(board, moves.data());

            for (Move* next = moves.data(); next < last; ++next)
            {
                UnmakeInfo unmake_info = board.Make(*next);

                if (!IsInCheck(OpposColor(board.side_to_move()), board))
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
        root_ = std::make_shared<PerftNode>(nullptr, kNullMove, depth_);
        int count;
        int layers = 0;
        do
        {
            count = AddLayer(root_, board_);
            ++layers;
        } while (layers < depth_ && count < options::Options::get().perft.min_works_items);
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

    void Perft::PropagateResultParent(PerftNode::Ptr node)
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
            PropagateResultParent(node->parent());
        }

        if (node->parent() == root_ && !abort_)
        {
            partial_result_callback_(node->move(), sum);
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

        std::uint64_t count = (node->depth() > 0 ? RunPerft(node->depth(), board) : 1);

        node->set_count(count);
        node->set_state(PerftNodeState::Done);

        if (node->parent())
        {
            PropagateResultParent(node->parent());
        }
    }

    void Perft::IncrementRunningThreads()
    {
        std::lock_guard<std::mutex> lock(perft_mutex_);
        ++running_threads_;
    }

    bool Perft::DecrementRunningThreads()
    {
        std::lock_guard<std::mutex> lock(perft_mutex_);
        --running_threads_;
        return running_threads_ == 0;
    }

    void Perft::SendResult()
    {
        using namespace std::chrono;
        auto end = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(end - start_);

        result_callback_(root_->count().get(), time_span.count());
    }

    void Perft::RunWorkerThread()
    {
        M8_LOG_SCOPE_THREAD();
        M8_DEBUG << "Thread started";

        IncrementRunningThreads();

        PerftNode::Ptr node = PickNode(root_);
        while (node)
        {
            ComputeNode(node);
            if (node->parent() == root_ && !abort_)
            {
                partial_result_callback_(node->move(), node->count().get());
            }
            
            node = PickNode(root_);
        }

        bool last_thread = DecrementRunningThreads();
        if (last_thread && !abort_)
        {
            SendResult();
        }
        
        M8_DEBUG << "Thread finished";
    }

    void Perft::StartThreads()
    {
        int threads = options::Options::get().perft.threads;

        for (int i = 0; i < threads; ++i)
        {
            threads_.push_back(std::thread(&Perft::RunWorkerThread, this));
        }
    }

    void Perft::JoinThreads()
    {
        M8_DEBUG << "Perft::JoinThreads";

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

    void Perft::RunParallel()
    {
        CreateRootNodes();

        start_ = std::chrono::high_resolution_clock::now();

        StartThreads();
    }

    void Perft::Abort()
    {
        abort_ = true;
        JoinThreads();
    }
}