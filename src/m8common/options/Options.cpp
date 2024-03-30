/// @file   Options.cpp
/// @author Mathieu Pagé
/// @date   November 2022
/// @brief  Contains methods related to the engine options

#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/lexical_cast.hpp>

#include "m8common/logging.hpp"
#include "m8common/Utils.hpp"

#include "m8common/options/Options.hpp"

namespace po = boost::program_options;
namespace pt = boost::property_tree;

namespace m8::options
{
    void Options::CreateModifiableOptions()
    {
        modifiable_options.emplace("Hash",
            std::make_unique<TypedModifiableOption<size_t>>("Hash",
                                                    "Define the hashtable size in Mb.",
                                                    this->tt_size));

        modifiable_options.emplace("UCI_Chess960",
            std::make_unique<TypedModifiableOption<bool>>("UCI_Chess960",
                                                    "Indicate if we play a Chess960 game.",
                                                    this->chess960));
                                                    
        modifiable_options.emplace("max-log-severity", 
            std::make_unique<TypedModifiableOption<m8::severity_level>>("max-log-severity",
                                                    "Define the maximum log severity level (fatal, error, warning, info, output, input, debug, trace).",
                                                    this->max_log_severity));
    }

    template<typename T>
    bool TryReadOption(pt::ptree& tree, const std::string key, T& storage)
    {
        auto optional = tree.get_optional<T>(key);
        if (optional.is_initialized())
        {
            storage = optional.get();
            return true;
        }

        return false;
    }

    // void ReadEvalOptions(pt::ptree& tree, EvalOptions& options)
    // {
    //     TryReadOption<int>(tree, "eval.pawn",   options.pawn);
    //     TryReadOption<int>(tree, "eval.knight", options.knight);
    //     TryReadOption<int>(tree, "eval.bishop", options.bishop);
    //     TryReadOption<int>(tree, "eval.rook",   options.rook);
    //     TryReadOption<int>(tree, "eval.queen",  options.queen);
    // }

    // std::vector<PsqtZone> ReadPsqtZones(pt::ptree& tree)
    // {
    //     std::vector<PsqtZone> list;

    //     for (auto& zone_tree : tree)
    //     {
    //         PsqtZone zone;
    //         zone.name = zone_tree.second.get<std::string>("name");
    //         auto zone_str = zone_tree.second.get<std::string>("zone");
    //         zone.zone = ConvertTo<Bb>(zone_str, true);
    //         zone.value = zone_tree.second.get<std::int32_t>("value");
    //         list.push_back(zone);
    //     }

    //     return list;
    // }

    // void ReadPsqtOptions(pt::ptree& tree, std::unordered_map<PieceType, std::vector<PsqtZone>>& psqt)
    // {
    //     auto psqt_tree = tree.get_child_optional("psqt");
    //     if (psqt_tree.is_initialized())
    //     {
    //         for (auto& psqt_piece_tree : psqt_tree.get())
    //         {
    //             auto piece_type = GetPieceTypeFromName(psqt_piece_tree.first);
    //             if (IsPieceType(piece_type))
    //             {
    //                 psqt.insert(std::make_pair(piece_type, ReadPsqtZones(psqt_piece_tree.second)));
    //             }
    //             else
    //             {
    //                 M8_WARNING << "Unknow piece type in config file (" << psqt_piece_tree.first << ")";
    //             }
    //         }
    //     }
    // }

    void ReadPiecesValues(pt::ptree& tree, PiecesValues& values)
    {
        TryReadOption<std::int16_t>(tree, "pawn",   values.pawn);
        TryReadOption<std::int16_t>(tree, "knight", values.knight);
        TryReadOption<std::int16_t>(tree, "bishop", values.bishop);
        TryReadOption<std::int16_t>(tree, "rook",   values.rook);
        TryReadOption<std::int16_t>(tree, "queen",  values.queen);
        TryReadOption<std::int16_t>(tree, "king",   values.king);
    }

    void ReadPiecesValues(pt::ptree& tree, Options& options)
    {
        ReadPiecesValues(tree.get_child_optional("pieces-values-middle-game").get(),
                         options.pieces_values_middle_game);
        ReadPiecesValues(tree.get_child_optional("pieces-values-end-game").get(),
                         options.pieces_values_end_game);
    }

    void ReadPieceSquareTable(pt::ptree& tree, PieceSquareTable::SinglePieceSqTable& single_piece_square_table)
    {
        int index = 0;
        for (const auto& item: tree)
        {
            single_piece_square_table[index++] = item.second.get_value<std::int16_t>();
        }
    }

    void ReadPieceSquareTable(pt::ptree& tree, PieceSquareTable& piece_square_table)
    {
        ReadPieceSquareTable(tree.get_child("pawn"),   piece_square_table.pawn);
        ReadPieceSquareTable(tree.get_child("knight"), piece_square_table.knight);
        ReadPieceSquareTable(tree.get_child("bishop"), piece_square_table.bishop);
        ReadPieceSquareTable(tree.get_child("rook"),   piece_square_table.rook);
        ReadPieceSquareTable(tree.get_child("queen"),  piece_square_table.queen);
        ReadPieceSquareTable(tree.get_child("king"),   piece_square_table.king);
    }

    void ReadPieceSquareTable(pt::ptree& tree, Options& options)
    {
        ReadPieceSquareTable(tree.get_child("psqt.middle-game"), options.piece_square_table.middle_game);
        ReadPieceSquareTable(tree.get_child("psqt.end-game"),    options.piece_square_table.end_game);
    }

    void ReadOptionsFromFile(const std::string filename)
    {   
        pt::ptree tree;
        pt::read_json(filename, tree);

        Options& options = Options::get();

        std::string temp;
        if (TryReadOption<std::string>(tree, "max-log-severity", temp))
        {
            options.max_log_severity = boost::lexical_cast<severity_level>(temp);
        }

        if (TryReadOption<std::string>(tree, "perft-threads", temp))
        {
            options.perft_threads = boost::lexical_cast<std::uint32_t>(temp);
        }

        if (TryReadOption<std::string>(tree, "tt-size", temp))
        {
            options.tt_size = boost::lexical_cast<size_t>(temp);
        }

        ReadPiecesValues(tree, options);
        ReadPieceSquareTable(tree, options);
    }

    po::options_description GenerateGlobalOptionsDescriptions()
    {
        auto& options = Options::get();

        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "Produce this help message. Use \"m8 <command> --help\" to see the allowed options for a specific command.")
            ("max-log-severity", po::value<m8::severity_level>(&options.max_log_severity),
             "Define the maximum log severity level (fatal, error, warning, info, output, input, debug, trace).")
            ("tt-size", po::value<size_t>(&options.tt_size),
             "Transposition table size in megabytes (must be a power of two).");
            
        return desc;
    }
} // namespace m8::options