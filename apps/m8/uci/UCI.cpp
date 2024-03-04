/// @file uci.cpp
/// @author Mathieu Pagé
/// @date February 2024
/// @brief Contains the UCI class implementation

#include <chrono>
#include <optional>
#include <regex>
#include <string>

#include <boost/lexical_cast.hpp>

#include "m8common/logging.hpp"
#include "m8common/stringHelpers.hpp"

#include "UCI.hpp"

namespace m8::uci
{
    const std::unordered_map<std::string, std::function<void(UCI*, const std::vector<std::string>&)>> UCI::HANDLERS =
    {
        {"uci",       &UCI::HandleUci},
        {"isready",   &UCI::HandleIsready},
        {"position",  &UCI::HandlePosition},
        {"go",        &UCI::HandleGo},
        {"stop",      &UCI::HandleStop},
        {"setoption", &UCI::HandleSetOption}
    };

    void UCI::Run()
    {
        // On déactive les buffers sur les IO
        std::cout.setf(std::ios::unitbuf);
        std::cin.rdbuf()->pubsetbuf(0, 0);

        std::string input;

        std::vector<std::string> token_list;
        do
        {
            if (std::getline(std::cin, input))
            {
                M8_INPUT << input;

                if (input.length() > 0)
                {
                    token_list = Split(input);

                    auto handler = HANDLERS.find(token_list.front());
                    if (handler != HANDLERS.end())
                    {
                        handler->second(this, token_list);
                    }
                }
            }
        } while(std::cin && 0 < token_list.size() && token_list.front() != "quit");
    }

    void UCI::HandleUci(const std::vector<std::string> params)
    {
        engine_.HandleUCI();
    }

    void UCI::HandleIsready(const std::vector<std::string> params)
    {
        engine_.HandleIsready();
    }

    void UCI::HandlePosition(const std::vector<std::string> params)
    {
        const std::string startpos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

        auto next = params.begin() + 2;
        std::string fen = startpos;
        if (params[1] == "fen")
        {
            fen = Join(params.begin() + 2,
                       params.begin() + 8,
                       " ");
            next = params.begin() + 8;
        }

        std::vector<std::string> moves;
        if (next < params.end() && *next == "moves")
        {
            ++next;
            moves.resize(params.end() - next);
            std::copy(next, params.end(), moves.begin());
        }

        engine_.HandlePosition(fen, moves);
    }

    std::chrono::milliseconds ReadTime(std::vector<std::string>::const_iterator& it)
    {
        ++it;
        std::int32_t value = boost::lexical_cast<std::int32_t>(*it);
        ++it;
        return std::chrono::milliseconds(value);
    }

    template<typename T>
    T ReadValue(std::vector<std::string>::const_iterator& it)
    {
        ++it;
        return boost::lexical_cast<T>(*(it++));
    }

    std::vector<std::string> ReadMoves(std::vector<std::string>::const_iterator& it,
                                       std::vector<std::string>::const_iterator end)
    {
        const std::regex regex_move("^([a-h][1-8]){2}[qrbn]?$");

        std::vector<std::string> moves;
        ++it;
        while (it < end && std::regex_match(*it, regex_move))
        {
            moves.push_back(*it);
            ++it;
        }
        return moves;
    }

    void UCI::HandleGo(const std::vector<std::string> params)
    {
        std::vector<std::string> search_moves;
        bool ponder = false;
        std::optional<std::chrono::milliseconds> wtime;
        std::optional<std::chrono::milliseconds> btime;
        std::optional<std::chrono::milliseconds> winc;
        std::optional<std::chrono::milliseconds> binc;
        std::optional<std::uint32_t> moves_to_go;
        std::optional<std::uint32_t> depth;
        std::optional<std::uint64_t> nodes;
        std::optional<std::uint32_t> mate;
        std::optional<std::chrono::milliseconds> move_time;
        bool infinite = false;

        auto it = params.begin() + 1;
        while (it < params.end())
        {
            if (                     *it == "searchmoves") { search_moves = ReadMoves(it, params.end()); }
            if (it < params.end() && *it == "ponder")      { ponder       = true; ++it; }
            if (it < params.end() && *it == "wtime")       { wtime        = ReadTime(it); }
            if (it < params.end() && *it == "btime")       { btime        = ReadTime(it); }
            if (it < params.end() && *it == "winc")        { winc         = ReadTime(it); }
            if (it < params.end() && *it == "binc")        { binc         = ReadTime(it); }
            if (it < params.end() && *it == "movestogo")   { moves_to_go  = ReadValue<std::uint32_t>(it); }
            if (it < params.end() && *it == "depth")       { depth        = ReadValue<std::uint32_t>(it); }
            if (it < params.end() && *it == "nodes")       { nodes        = ReadValue<std::uint64_t>(it); }
            if (it < params.end() && *it == "mate")        { mate         = ReadValue<std::uint32_t>(it); }
            if (it < params.end() && *it == "movetime")    { move_time    = ReadTime(it); }
            if (it < params.end() && *it == "infinite")    { infinite     = true; ++it; }
        }

        engine_.HandleGo(search_moves,
                         ponder,
                         wtime,
                         btime,
                         winc,
                         binc,
                         moves_to_go,
                         depth,
                         nodes,
                         mate,
                         move_time,
                         infinite);
    }

    void UCI::HandleStop(const std::vector<std::string> params)
    {
        engine_.HandleStop();
    }

    void UCI::HandleSetOption(const std::vector<std::string> params)
    {
        std::string_view name = params[2];
        std::optional<std::string_view> value = std::nullopt;
        if (params.size() >= 5)
        {
            value = params[4];
        }

        engine_.HandleSetOption(name, value);
    }
}
