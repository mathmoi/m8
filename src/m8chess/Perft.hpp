/// @file   Perft.hpp
/// @author Mathieu Pag�
/// @date   Janurary 2018
/// @brief  Contains functionalities of the perft command.

#ifndef M8_PERFT_HPP_
#define M8_PERFT_HPP_

#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <future>
#include <vector>
#include <chrono>

#include <boost/optional.hpp>

#include "Move.hpp"
#include "MoveGen.hpp"
#include "Board.hpp"

namespace m8
{
    /// Result of a perft test
    struct PerftResult
    {
        /// Number of lead nodes visited
        std::uint64_t nodes;

        /// Time in seconds 
        double seconds;
    };

    /// State of a perft node
    enum class PerftNodeState
    {
        /// The node is in an initial state.
        Available,

        /// A thread is working on this node.
        WorkedOn,

        /// Node that as been computed
        Done
    };

    class PerftNode
    {
    public:
        typedef std::shared_ptr<PerftNode> Ptr;
        typedef std::vector<PerftNode::Ptr> Nodes;

        PerftNode(PerftNode::Ptr parent, Move move, int depth)
            : parent_(parent), move_(move), state_(PerftNodeState::Available), depth_(depth), children_(),
              children_done_(0)
        {};

        inline Move move() const { return move_; }
        inline PerftNodeState state() const { return state_; }
        inline void set_state(PerftNodeState state) { state_ = state; };
        inline std::mutex& mutex() { return mutex_; };
        inline int depth() const { return depth_; };
        inline const Nodes& children() const { return children_; }
        inline void AddChild(PerftNode::Ptr child) { children_.push_back(child); }
        inline boost::optional<std::uint64_t> count() const { return count_; }
        inline void set_count(std::uint64_t count) { count_ = count; }
        inline unsigned int children_done() const { return children_done_; }
        inline void AddChildDone() { ++children_done_; }
        inline PerftNode::Ptr parent() const { return parent_; }

    private:
        PerftNode::Ptr parent_;
        Move move_;
        PerftNodeState state_;
        boost::optional<std::uint64_t> count_;
        int depth_;
        Nodes children_;
        unsigned int children_done_;

        std::mutex mutex_;
    };
    
    /// Represents a performance test.
    class Perft
    {
    public:
        typedef std::function<void(Move, std::uint64_t)> PartialResultCallback;
        typedef std::function<void(std::uint64_t, double)> ResultCallback;

        /// Constructor
        inline Perft(int depth, const Board& board, PartialResultCallback partial_result_callback, ResultCallback result_callback)
            : depth_(depth), abort_(false), board_(board), partial_result_callback_(partial_result_callback), result_callback_(result_callback), running_threads_(0)
        {}

        /// Destructor.
        ~Perft();
        
        /// Run the test in prallel.
        void RunParallel();

        /// Stop the current perft operation.
        void Abort();

    private:
        int depth_;
        bool abort_;
        Board board_;
        PartialResultCallback partial_result_callback_;
        ResultCallback result_callback_;
        std::mutex perft_mutex_;
        int running_threads_;
        std::chrono::high_resolution_clock::time_point start_;
        PerftNode::Ptr root_;
        std::vector<std::thread> threads_;

        void CreateRootNodes();
        int AddLayer(PerftNode::Ptr node, Board& board, const MoveGen& move_gen);

        std::uint64_t RunPerft(int depth, Board& board, const MoveGen& move_gen);
        
        PerftNode::Ptr PickNode(PerftNode::Ptr root);
        void ComputeNode(PerftNode::Ptr node);
        void StartThreads();
        void JoinThreads();
        bool ReserveNode(PerftNode::Ptr node);
        void GetBoard(Board& board, PerftNode::Ptr node);
        void PropagateResultParent(PerftNode::Ptr node);
        void CompleteInternalNode(PerftNode::Ptr node);
        std::uint64_t CalculateSumChildren(PerftNode::Ptr node);
        void RunWorkerThread();
        void IncrementRunningThreads();
        bool DecrementRunningThreads();
        void SendResult();
    };
}

#endif // M8_PERFT_HPP_