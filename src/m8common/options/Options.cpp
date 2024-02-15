/// @file   Options.cpp
/// @author Mathieu Pagé
/// @date   November 2022
/// @brief  Contains methods related to the engine options

#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/program_options.hpp>

#include "m8common/logging.hpp"
#include "m8common/Utils.hpp"

#include "m8common/options/Options.hpp"

namespace po = boost::program_options;
namespace pt = boost::property_tree;

namespace m8::options
{
    void Options::CreateModifiableOptions()
    {
        modifiable_options.emplace("perft-threads", 
            std::make_unique<TypedModifiableOption<std::int32_t>>("perft-threads",
                                                    "Numbers of parallel threads to use for the perft command.",
                                                    this->perft_threads));
        
        modifiable_options.emplace("max-log-severity", 
            std::make_unique<TypedModifiableOption<m8::severity_level>>("max-log-severity",
                                                    "Define the maximum log severity level (fatal, error, warning, info, output, input, debug, trace).",
                                                    this->max_log_severity));

        modifiable_options.emplace("display-auto", 
            std::make_unique<TypedModifiableOption<bool>>("display-auto",
                                                    "Indicate if the board should be displayed after each moves.",
                                                    this->display_auto));

        modifiable_options.emplace("display-eval", 
            std::make_unique<TypedModifiableOption<bool>>("display-eval",
                                                    "Indicate if the evaluation should be displayed with the board.",
                                                    this->display_eval));

        modifiable_options.emplace("use-san", 
            std::make_unique<TypedModifiableOption<bool>>("use-san",
                                                    "Indicate if the engine should use the Standard Algebraic Notation.",
                                                    this->use_san));
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

    void ReadEvalOptions(pt::ptree& tree, EvalOptions& options)
    {
        TryReadOption<int>(tree, "eval.pawn",   options.pawn);
        TryReadOption<int>(tree, "eval.knight", options.knight);
        TryReadOption<int>(tree, "eval.bishop", options.bishop);
        TryReadOption<int>(tree, "eval.rook",   options.rook);
        TryReadOption<int>(tree, "eval.queen",  options.queen);
    }

    std::vector<PsqtZone> ReadPsqtZones(pt::ptree& tree)
    {
        std::vector<PsqtZone> list;

        for (auto& zone_tree : tree)
        {
            PsqtZone zone;
            zone.name = zone_tree.second.get<std::string>("name");
            auto zone_str = zone_tree.second.get<std::string>("zone");
            zone.zone = ConvertTo<Bb>(zone_str, true);
            zone.value = zone_tree.second.get<std::int32_t>("value");
            list.push_back(zone);
        }

        return list;
    }

    void ReadPsqtOptions(pt::ptree& tree, std::unordered_map<PieceType, std::vector<PsqtZone>>& psqt)
    {
        auto psqt_tree = tree.get_child_optional("psqt");
        if (psqt_tree.is_initialized())
        {
            for (auto& psqt_piece_tree : psqt_tree.get())
            {
                auto piece_type = GetPieceTypeFromName(psqt_piece_tree.first);
                if (IsPieceType(piece_type))
                {
                    psqt.insert(std::make_pair(piece_type, ReadPsqtZones(psqt_piece_tree.second)));
                }
                else
                {
                    M8_WARNING << "Unknow piece type in config file (" << psqt_piece_tree.first << ")";
                }
            }
        }
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

        if (TryReadOption<std::string>(tree, "display-auto", temp))
        {
            options.display_auto = boost::lexical_cast<bool>(temp);
        }

        if (TryReadOption<std::string>(tree, "display-eval", temp))
        {
            options.display_eval = boost::lexical_cast<bool>(temp);
        }

        if (TryReadOption<std::string>(tree, "min-display-depth", temp))
        {
            options.min_display_depth = boost::lexical_cast<DepthType>(temp);
        }

        if (TryReadOption<std::string>(tree, "use-san", temp))
        {
            options.use_san = boost::lexical_cast<bool>(temp);
        }

        if (TryReadOption<std::string>(tree, "tt-size", temp))
        {
            options.tt_size = boost::lexical_cast<size_t>(temp);
        }
        
        ReadEvalOptions(tree, options.eval);
        ReadPsqtOptions(tree, options.eval.psqt_zones);
    }

    po::options_description GenerateOptionsDescriptions()
    {
        auto& options = Options::get();

        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("max-log-severity", po::value<m8::severity_level>(&options.max_log_severity),
             "Define the maximum log severity level (fatal, error, warning, info, output, input, debug, trace).")
            ("tt-size", po::value<size_t>(&options.tt_size),
             "Transposition table size in megabytes (must be a power of two).");

        return desc;
    }

    bool ReadOptionsFromCommandLine(int argc, char** argv, std::ostream& out)
    {
        bool stop_execution = false;

        auto desc = GenerateOptionsDescriptions();
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            out << desc << std::endl;
            stop_execution = true;
        }

        return stop_execution;
    }

} // namespace m8::options