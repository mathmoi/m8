/// @file Perft_tests.cpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @copyright Copyright (c) 2023 Mathieu Pagé
/// @date July 2023

#include <condition_variable>
#include <optional>

#include "catch.hpp"

#include "../m8chess/Perft.hpp"

using namespace m8;

class TestPerftObserver : public engine::IPerftObserver
{
public:
    TestPerftObserver()
    : is_completed(false)
    {}

    void OnPerftCompleted(std::uint64_t count, double time)
    {
        std::lock_guard guard(mutex_);

        count_ = count;
        is_completed = true;
        condition_variable_.notify_all();
    }

    void WaitForCompletion()
    {
        std::unique_lock lock(mutex_);
        condition_variable_.wait(lock, [this]{ return is_completed; });
    }

    std::uint64_t count() const
    {
        assert(count_.has_value());
        return count_.value();
    }

private:
    std::optional<std::uint64_t> count_;

    bool is_completed;
    std::mutex mutex_;
    std::condition_variable condition_variable_;
};

TEST_CASE("Perft test of differents positions at different depth")
{
    const std::string kKiwipetePosition("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    const std::string kCPWPosition3("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -");
    const std::string kCPWPosition4("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    const std::string kCPWPosition5("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    const std::string kCPWPosition6("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");

    std::string fen;
    int depth;
    std::uint64_t expected;

    SECTION("Starting position, depth 1") { fen = kStartingPositionFEN; depth = 1; expected = 20; }
    SECTION("Starting position, depth 2") { fen = kStartingPositionFEN; depth = 2; expected = 400; }
    SECTION("Starting position, depth 3") { fen = kStartingPositionFEN; depth = 3; expected = 8902; }
    SECTION("Starting position, depth 4") { fen = kStartingPositionFEN; depth = 4; expected = 197281; }
    SECTION("Starting position, depth 5") { fen = kStartingPositionFEN; depth = 5; expected = 4865609; }
#ifdef NDEBUG
    SECTION("Starting position, depth 6") { fen = kStartingPositionFEN; depth = 6; expected = 119060324; }
#endif

    SECTION("Kiwipete position, depth 1") { fen = kKiwipetePosition; depth = 1; expected = 48; }
    SECTION("Kiwipete position, depth 2") { fen = kKiwipetePosition; depth = 2; expected = 2039; }
    SECTION("Kiwipete position, depth 3") { fen = kKiwipetePosition; depth = 3; expected = 97862; }
    SECTION("Kiwipete position, depth 4") { fen = kKiwipetePosition; depth = 4; expected = 4085603; }
#ifdef NDEBUG
    SECTION("Kiwipete position, depth 5") { fen = kKiwipetePosition; depth = 5; expected = 193690690; }
#endif

    SECTION("Chess Programming Wiki position 3, depth 1") { fen = kCPWPosition3; depth = 1; expected = 14; }
    SECTION("Chess Programming Wiki position 3, depth 2") { fen = kCPWPosition3; depth = 2; expected = 191; }
    SECTION("Chess Programming Wiki position 3, depth 3") { fen = kCPWPosition3; depth = 3; expected = 2812; }
    SECTION("Chess Programming Wiki position 3, depth 4") { fen = kCPWPosition3; depth = 4; expected = 43238; }
    SECTION("Chess Programming Wiki position 3, depth 5") { fen = kCPWPosition3; depth = 5; expected = 674624; }
#ifndef NDEBUG
    SECTION("Chess Programming Wiki position 3, depth 6") { fen = kCPWPosition3; depth = 6; expected = 11030083; }
#endif

    SECTION("Chess Programming Wiki position 4, depth 1") { fen = kCPWPosition4; depth = 1; expected = 6; }
    SECTION("Chess Programming Wiki position 4, depth 2") { fen = kCPWPosition4; depth = 2; expected = 264; }
    SECTION("Chess Programming Wiki position 4, depth 3") { fen = kCPWPosition4; depth = 3; expected = 9467; }
    SECTION("Chess Programming Wiki position 4, depth 4") { fen = kCPWPosition4; depth = 4; expected = 422333; }
#ifdef NDEBUG
    SECTION("Chess Programming Wiki position 4, depth 5") { fen = kCPWPosition4; depth = 5; expected = 15833292; }
#endif

    SECTION("Chess Programming Wiki position 5, depth 1") { fen = kCPWPosition5; depth = 1; expected = 44; }
    SECTION("Chess Programming Wiki position 5, depth 2") { fen = kCPWPosition5; depth = 2; expected = 1486; }
    SECTION("Chess Programming Wiki position 5, depth 3") { fen = kCPWPosition5; depth = 3; expected = 62379; }
    SECTION("Chess Programming Wiki position 5, depth 4") { fen = kCPWPosition5; depth = 4; expected = 2103487; }
#ifdef NDEBUG
    SECTION("Chess Programming Wiki position 5, depth 5") { fen = kCPWPosition5; depth = 5; expected = 89941194; }
#endif

    SECTION("Chess Programming Wiki position 6, depth 1") { fen = kCPWPosition6; depth = 1; expected = 46; }
    SECTION("Chess Programming Wiki position 6, depth 2") { fen = kCPWPosition6; depth = 2; expected = 2079; }
    SECTION("Chess Programming Wiki position 6, depth 3") { fen = kCPWPosition6; depth = 3; expected = 89890; }
    SECTION("Chess Programming Wiki position 6, depth 4") { fen = kCPWPosition6; depth = 4; expected = 3894594; }
#ifdef NDEBUG
    SECTION("Chess Programming Wiki position 6, depth 5") { fen = kCPWPosition6; depth = 5; expected = 164075551; }
#endif

    Board board(fen);
    TestPerftObserver observer;

    Perft perft(depth, board, &observer);
    perft.Start();
    observer.WaitForCompletion();

    REQUIRE(expected == observer.count());
}