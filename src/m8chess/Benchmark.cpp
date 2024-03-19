/// @file Benchmark.cpp
/// @author Mathieu Pagé
/// @date March 2024
/// @brief Contains the implementations details of the Benchmark class.

#include <algorithm>

#include "m8common/options/Options.hpp"
#include "m8common/Output.hpp"

#include "m8chess/Benchmark.hpp"
#include "m8chess/TimeManager.hpp"

namespace m8
{
    std::array<std::pair<std::string, DepthType>, 64> Benchmark::positions = 
    {{
        {"8/3p4/p1bk3p/Pp6/1Kp1PpPp/2P2P1P/2P5/5B2 b",                     16},
        {"2r3k1/1p2q1pp/2b1pr2/p1pp4/6Q1/1P1PP1R1/P1PN2PP/5RK1 w",          9},
        {"2r3k1/pppR1pp1/4p3/4P1P1/5P2/1P4K1/P1P5/8 w",                    12},
        {"r2q1rk1/4bppp/p2p4/2pP4/3pP3/3Q4/PP1B1PPP/R3R1K1 w",              8},
        {"r1bq1r1k/1pp1n1pp/1p1p4/4p2Q/4Pp2/1BNP4/PPP2PPP/3R1RK1 w",        9},
        {"2kr1bnr/pbpq4/2n1pp2/3p3p/3P1P1B/2N2N1Q/PPP3PP/2KR1B1R w",        8},
        {"8/3k4/8/8/8/4B3/4KB2/2B5 w",                                     12},
        {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w",       8},
        {"5r1k/6p/1n2Q2p/4p/8/7P/PP4PK/R1B1q/ w",                          10},
        {"4r1k1/r1q2ppp/ppp2n2/4P3/5Rb1/1N1BQ3/PPP3PP/R5K1 w",              8},
        {"2K5/p7/7P/5pR1/8/5k2/r7/8 w",                                    13},
        {"2rqkb1r/ppp2p2/2npb1p1/1N1Nn2p/2P1PP2/8/PP2B1PP/R1BQK2R b",       8},
        {"8/2p4P/8/kr6/6R1/8/8/1K6 w",                                     12},
        {"r3r1k1/ppqb1ppp/8/4p1NQ/8/2P5/PP3PPP/R3R1K1 b",                   9},
        {"8/k/3p/p2P1p/P2P1P/8/8/K/ w",                                    29},
        {"3rn2k/ppb2rpp/2ppqp2/5N2/2P1P3/1P5Q/PB3PPP/3RR1K1 w",             9},
        {"rq3rk1/ppp2ppp/1bnpb3/3N2B1/3NP3/7P/PPPQ1PP1/2KR3R w",            8},
        {"r1bq1r1k/b1p1npp1/p2p3p/1p6/3PP3/1B2NN2/PP3PPP/R2Q1RK1 w",        8},
        {"r1bbk1nr/pp3p1p/2n5/1N4p1/2Np1B2/8/PPP2PPP/2KR1B1R w",            9},
        {"r2q1rk1/1ppnbppp/p2p1nb1/3Pp3/2P1P1P1/2N2N1P/PPB1QP2/R1B2RK1 b",  9},
        {"3r1rk1/p5pp/bpp1pp2/8/q1PP1P2/b3P3/P2NQRPP/1R2B1K1 b",            8},
        {"2q1rr1k/3bbnnp/p2p1pp1/2pPp3/PpP1P1P1/1P2BNNP/2BQ1PRK/7R b",      9},
        {"6k1/4pp1p/3p2p1/P1pPb3/R7/1r2P1PP/3B1P2/6K1 w",                  11},
        {"7k/3p2pp/4q3/8/4Q3/5Kp1/P6b/8 w",                                12},
        {"rnbqkb1r/p3pppp/1p6/2ppP3/3N4/2P5/PPP1QPPP/R1B1KB1R w",           9},
        {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w",                  10},
        {"8/8/8/5N2/8/p7/8/2NK3k w",                                       15},
        {"1nk1r1r1/pp2n1pp/4p3/q2pPp1N/b1pP1P2/B1P2R2/2P1B1PP/R2Q2K1 w",    9},
        {"4b3/p3kp2/6p1/3pP2p/2pP1P2/4K1P1/P3N2P/8 w",                     15},
        {"6k1/6p1/6Pp/ppp5/3pn2P/1P3K2/1PP2P2/3N4 b",                      15},
        {"r1b2rk1/2q1b1pp/p2ppn2/1p6/3QP3/1BN1B3/PPP3PP/R4RK1 w",           8},
        {"rnbqkb1r/p3pppp/1p6/2ppP3/3N4/2P5/PPP1QPPP/R1B1KB1R w",           9},
        {"8/1p3pp1/7p/5P1P/2k3P1/8/2K2P2/8 w",                             19},
        {"r1bq1rk1/pp2ppbp/2np2p1/2n5/P3PP2/N1P2N2/1PB3PP/R1B1QRK1 b",      8},
        {"rnb2r1k/pp2p2p/2pp2p1/q2P1p2/8/1Pb2NP1/PB2PPBP/R2Q1RK1 w",        9},
        {"8/8/8/8/5kp1/P7/8/1K1N4 w",                                      18},
        {"/k/rnn/8/8/8/5RBB/K/ w",                                         11},
        {"6k1/6p1/P6p/r1N5/5p2/7P/1b3PP1/4R1K1 w",                         11},
        {"4k2r/1pb2ppp/1p2p3/1R1p4/3P4/2r1PN2/P4PPP/1R4K1 b",               9},
        {"r1bq1rk1/ppp1nppp/4n3/3p3Q/3P4/1BP1B3/PP1N2PP/R4RK1 w",           8},
        {"3rr3/2pq2pk/p2p1pnp/8/2QBPP2/1P6/P5PP/4RRK1 b",                   9},
        {"r1bqk2r/pp2bppp/2p5/3pP3/P2Q1P2/2N1B3/1PP3PP/R4RK1 b",            9},
        {"r1bqkb1r/4npp1/p1p4p/1p1pP1B1/8/1B6/PPPN1PPP/R2Q1RK1 w",          9},
        {"8/8/3P3k/8/1p6/8/1P6/1K3n2 b",                                   19},
        {"8/8/1P6/5pr1/8/4R3/7k/2K5 w",                                    12},
        {"8/6pk/1p6/8/PP3p1p/5P2/4KP1q/3Q4 w",                             14},
        {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w",                              14},
        {"r4k2/pb2bp1r/1p1qp2p/3pNp2/3P1P2/2N3P1/PPP1Q2P/2KRR3 w",          9},
        {"2r1nrk1/p2q1ppp/bp1p4/n1pPp3/P1P1P3/2PBB1N1/4QPPP/R4RK1 w",       9},
        {"5k2/7R/4P2p/5K2/p1r2P1p/8/8/8 b",                                14},
        {"3rr1k1/pp3pp1/1qn2np1/8/3p4/PP1R1P2/2P1NQPP/R1B3K1 b",            8},
        {"3q2k1/pb3p1p/4pbp1/2r5/PpN2N2/1P2P2P/5PP1/Q2R2K1 b",              9},
        {"8/2p5/8/2kPKp1p/2p4P/2P5/3P4/8 w",                               18},
        {"8/R7/2q5/8/6k1/8/1P5p/K6R w",                                    11},
        {"r1q2rk1/2p1bppp/2Pp4/p6b/Q1PNp3/4B3/PP1R1PPP/2K4R w",             8},
        {"3r1k2/4npp1/1ppr3p/p6P/P2PPPP1/1NR5/5K2/2R5 w",                  10},
        {"2r2rk1/1bqnbpp1/1p1ppn1p/pP6/N1P1P3/P2B1N1P/1B2QPP1/R2R2K1 b",    8},
        {"r2qnrnk/p2b2b1/1p1p2pp/2pPpp2/1PP1P3/PRNBB3/3QNPPP/5RK1 w",       8},
        {"1r3k2/4q3/2Pp3b/3Bp3/2Q2p2/1p1P2P1/1P2KP2/3N4 w",                10},
        {"8/pp2r1k1/2p1p3/3pP2p/1P1P1P1P/P5KR/8/8 w",                      15},
        {"4rrk1/pp1n3p/3q2pQ/2p1pb2/2PP4/2P3N1/P2B2PP/4RRK1 b",            10},
        {"r3r1k1/2p2ppp/p1p1bn2/8/1q2P3/2NPQN2/PPP3PP/R4RK1 b",             8},
        {"3b4/5kp1/1p1p1p1p/pP1PpP1P/P1P1P3/3KN3/8/8 w",                   21},
        {"8/3p3B/5p2/5P2/p7/PP5b/k7/6K1 w",                                18}
    }};

    Benchmark::Benchmark(DepthType deltaDepth, std::uint32_t runs, std::uint32_t threads)
    : deltaDepth_(deltaDepth),
      runs_(runs),
      threads_count_(threads)
    {}

    void Benchmark::Run()
    {
        abort_ = false;
        results_.clear();
        results_.reserve(runs_);

        for (unsigned int i = 0; i < threads_count_; ++i)
        {
            threads_.emplace_back([this]() { RunBenchmarks(); });
        }

        for (auto& thread : threads_)
        {
            thread.join();
        }

        auto result = std::accumulate(results_.begin(), results_.begin() + runs_, BenchmarkResult());
        
        Output out;
        out << "Nodes: "            << result.nodes()                   << '\n'
            << "Time: "             << ToFSec(result.duration())        << '\n'
            << "Nodes per second: " << AddMetricSuffix(result.nps(), 2) << std::endl;
    }

    BenchmarkResult Benchmark::RunPosition(std::string_view fen, DepthType depth)
    {
        Board board(fen);
        auto time_manager = std::make_unique<TimeManager>(std::nullopt, std::nullopt, std::nullopt, std::nullopt, true);
        auto search = std::make_shared<search::Search>(board, std::move(time_manager), depth + deltaDepth_);
        transposition::TranspositionTable transposition_table(options::Options::get().tt_size * 1024 * 1024 / threads_count_ );
        transposition_table.Empty();
        search::IterativeDeepening iterative_deepening(transposition_table);

        Timer timer(TimerDirection::Up);
        timer.Start();
        auto search_result = iterative_deepening.Start(search);
        timer.Stop();

        return BenchmarkResult(timer.time_on_clock(), search_result.stats_.all_nodes());
    }

    BenchmarkResult Benchmark::RunBenchmark()
    {
        BenchmarkResult result;

        for (auto position : positions)
        {
            if (abort_)
            {
                break;
            }
            result += RunPosition(position.first, position.second);
        }

        return result;
    }

    void Benchmark::RunBenchmarks()
    {
        while (!abort_ && results_.size() < runs_)
        {
            auto result = RunBenchmark();
            if (!abort_)
            {
                std::lock_guard lock(mutex_);
                results_.push_back(result);
            }
        }

        // If we are done abort all the other threads
        abort_ = true;
    }
}