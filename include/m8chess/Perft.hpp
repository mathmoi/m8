/// @file Perft.hpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023

#ifndef M8_PERFT_HPP_
#define M8_PERFT_HPP_

#include <algorithm>
#include <cstdint>
#include <functional>
#include <mutex>
#include <numeric>
#include <ranges>
#include <thread>
#include <vector>

#include "IPerftObserver.hpp"

#include "Board.hpp"

namespace m8
{
    /// Status of a move in a perft node
    enum class PerftMoveStatus
    {
        /// Indicate that the move need to be searched.
        New,

        /// Indicate that a thread is already searching this move.
        Searching,

        /// Indicate that this move is shared. A PerftNode corresponding to the position
        /// resulting from this move exists.
        Shared,

        /// The Move has already been searched and the number of leaf nodes is known.
        Done
    };

    // Forward declaration
    class PerftNode;

    /// Represents a move in a perft node
    class PerftMove
    {
    public:
        /// Constructor
        ///
        /// @param move Chess move represented by this PerftMove
        PerftMove(Move move)
        : move_(move),
          status_(PerftMoveStatus::New),
          node_(nullptr)
        {}

        /// Returns the status  of the move.
        PerftMoveStatus status() const { return status_; }

        /// Returns the move.
        Move move() const { return move_; }

        /// Return the perft node following this move
        PerftNode& node() const
        {
            assert(status_ == PerftMoveStatus::Shared);
            return *node_;
        }

        /// Return the nodes count for this move.
        std::uint64_t count() const
        {
            assert(status_ == PerftMoveStatus::Done);
            return count_;
        }

        /// Change this node to a shared node.
        void MakeShared(std::unique_ptr<PerftNode> node)
        {
            assert(status_ == PerftMoveStatus::New);
            status_ = PerftMoveStatus::Shared;
            node_ = std::move(node);
        }

        /// Change the status of the node to Searching.
        void MakeSearching()
        {
            assert(status_ == PerftMoveStatus::New);
            status_ = PerftMoveStatus::Searching;
        }

        /// Change the status of the node to done.
        void CheckSharedDone();

        /// Change the status of the node to done.
        void MakeDone(std::uint64_t count)
        {
            assert(status_ == PerftMoveStatus::Searching || status_ == PerftMoveStatus::New);
            status_ = PerftMoveStatus::Done;
            count_ = count;
        }

    private:
        Move                       move_;
        PerftMoveStatus            status_;
        std::unique_ptr<PerftNode> node_;
        std::uint64_t              count_;
    };

    /// Represent a shared node during a perft test
    class PerftNode
    {
    public:
        /// Constructor
        ///
        /// @param board position at the current node
        PerftNode(Board& board);

        /// Returns an iterator to the first move.
        inline std::vector<PerftMove>::iterator begin() { return moves_.begin(); }

        /// Returns an iterator one position past the last move.
        inline std::vector<PerftMove>::iterator end() { return moves_.end(); }

        /// Indicates if all the moves of this node are done.
        inline bool done() const
        {
            return std::ranges::all_of(moves_,
                                       [](const PerftMove& move){ return move.status() == PerftMoveStatus::Done; });
        }

        /// Returns the sum of the count of all moves at this node.
        inline std::uint64_t count() const
        {
            auto counts = moves_
                            | std::views::transform([](const PerftMove& move){ return move.count(); })
                            | std::views::common;
            return std::accumulate(counts.begin(), counts.end(), UINT64_C(0));
        }


    private:
        std::vector<PerftMove> moves_;

        void GenerateMoves(Board& board);
    };

    /// Classes responsible for running a performance test.
    class Perft
    {
    public:
        /// Constructor
        inline Perft(int depth, const Board& board, IPerftObserver* observer)
            : depth_(depth),
              board_(board),
              root_(board_),
              observer_(observer)
        {}

        /// Destructor.
        ~Perft() { JoinThreads(); };

        /// Run the test in prallel.
        void Run();

    private:
        const int kMinParallelDepth = 3;

        int depth_;
        Board board_;

        std::chrono::steady_clock::time_point start_;

        PerftNode root_;

        std::vector<std::thread> threads_;
        std::mutex mutex_;

        IPerftObserver* observer_;

        std::uint64_t RecursivePerft(Board& board, int depth);
        void ContributeAtSharedNode(PerftNode& node, Board& board, int depth);
        void PerftMoveRecursive(PerftMove& move, Board& board, int depth);
        void ParallelPerft(PerftNode& node,
                           Board& board,
                           int depth,
                           PerftMoveStatus node_type,
                           bool is_root,
                           std::function<void(PerftMove&, Board&, int)> recurse);
        void FindNodeToContribute(PerftNode& node, Board& board, int depth, bool is_root);
        void StartThreads();
        void JoinThreads();
        void RunThread();
        void SendResult();
    };
}

#endif // M8_PERFT_HPP_