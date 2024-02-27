/// @file UCIInterface.cpp
/// @author Mathieu Pagé
/// @date February 2024
/// @brief Contains the UCIInterface class implementation

#include <boost/lexical_cast.hpp>

#include "m8chess/CoordinateNotation.hpp"
#include "m8chess/eval/Eval.hpp"

#include "UCIInterface.hpp"

namespace m8::uci
{
    void UCIInterface::SendId(const std::string& id_type, const std::string& id_value)
    {
        out_ <<"id " <<id_type <<' ' <<id_value <<std::endl;
    }

    void UCIInterface::SendUciok()
    {
        out_ <<"uciok" <<std::endl;
    }

    void UCIInterface::SendReadyok()
    {
        out_ <<"readyok" <<std::endl;
    }

    void UCIInterface::SendBestMove(const std::string& move, const std::optional<std::string>& ponder_move)
    {
        out_ <<"bestmove " <<move;
        if (ponder_move.has_value())
        {
            out_ <<" ponder " <<*ponder_move;
        }
        out_ <<std::endl;
    }

    void UCIInterface::SendInfo(std::optional<DepthType>                 depth,
                                std::optional<DepthType>                 sel_depth,
                                std::optional<std::chrono::milliseconds> time,
                                std::optional<NodeCounterType>           nodes,
                                std::optional<std::vector<std::string>>  pv,
                                std::optional<std::uint16_t>             multi_pv,
                                std::optional<EvalType>                  score,
                                std::optional<std::string>               current_move,
                                std::optional<std::uint16_t>             current_move_number,
                                std::optional<std::uint16_t>             hash_full,
                                std::optional<NodeCounterType>           nps,
                                std::optional<NodeCounterType>           tb_hits,
                                std::optional<std::uint16_t>             cpu_load,
                                std::optional<std::string>               string)
    {
        out_ <<"info";

        if (depth)               out_ <<" depth "               <<*depth;
        if (sel_depth)           out_ <<" seldepth "            <<*sel_depth;
        if (time)                out_ <<" time "                <<time->count();
        if (nodes)               out_ <<" nodes "               <<*nodes;
        if (pv)
        {
            out_ <<" pv ";
            for (auto move : *pv)
            {
                out_ <<move <<' ';
            }
        }
        if (multi_pv)            out_ <<" multi_pv "            <<*multi_pv;
        if (score)
        {
            out_ <<" score ";
            if (eval::IsMateEval(*score))
            {
                out_ <<"mate " <<eval::GetMateDistance(*score);
            }
            else
            {
                out_ <<"cp " <<*score;
            }
        }
        if (current_move)        out_ <<" current_move "        <<*current_move;
        if (current_move_number) out_ <<" current_move_number " <<*current_move_number;
        if (hash_full)           out_ <<" hash_full "           <<*hash_full;
        if (nps)                 out_ <<" nps "                 <<*nps;
        if (tb_hits)             out_ <<" tb_hits "             <<*tb_hits;
        if (cpu_load)            out_ <<" cpu_load "            <<*cpu_load;
        if (string)              out_ <<" string "              <<*string;

        out_ <<std::endl;
    }

    void UCIInterface::SendCheckOption(std::string_view name, bool default_value)
    {
        SendOption(name,
                   "check",
                   default_value ? "true" : "false");
    }

    void UCIInterface::SendSpinOption(std::string_view name, std::int64_t min, std::int64_t max, std::int64_t default_value)
    {
        auto default_string = boost::lexical_cast<std::string>(default_value);
        SendOption(name,
                   "spin",
                   default_string,
                   min,
                   max);
    }

    void UCIInterface::SendComboOption(std::string_view name, const std::vector<std::string>& values, std::string_view default_value)
    {
        SendOption(name,
                   "combo",
                   default_value,
                   std::nullopt,
                   std::nullopt,
                   values);
    }

    void UCIInterface::SendStringOption(std::string_view name, std::string_view default_value)
    {
        SendOption(name,
                   "string",
                   default_value);
    }

    void UCIInterface::SendButtonOption(std::string_view name)
    {
        SendOption(name,
                   "button");
    }

    void UCIInterface::SendOption(std::string_view name,
                                  std::string_view type,
                                  std::optional<std::string_view> default_value,
                                  std::optional<std::int64_t> min,
                                  std::optional<std::int64_t> max,
                                  std::optional<std::vector<std::string>> values)
    {
        out_ <<"option name " <<name <<" type " <<type;

        if (default_value)
        {
            out_ <<" default " <<*default_value;
        }

        if (min)
        {
            out_ <<" min " <<*min;
        }

        if (max)
        {
            out_ <<" max " <<*max;
        }

        if (values)
        {
            for (auto value : *values)
            {
                out_ <<" var " <<value;
            }
        }

        out_ <<std::endl;
    }
}