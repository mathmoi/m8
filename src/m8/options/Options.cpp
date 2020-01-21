/// @file   Options.cpp
/// @author Mathieu Pag�
/// @date   January 2017
/// @brief  Contains the declarations of the Options class that handles m8 command line 
///         parameters and m8.ini file.

#include <functional>

#include <boost/program_options.hpp>
#include <boost/algorithm/string/replace.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "../../m8common/logging.hpp"
#include "Options.hpp"

namespace po = boost::program_options;
namespace pt = boost::property_tree;

#define M8_DESC_OPTIONS(name, desc, type, getter, setter)       (name, po::value<type>()->notifier([&options](type v) { options.setter(v); }), desc)
#define M8_ADD_OPTIONS(name, desc, type, getter, setter)        AddOption<type>(name, desc, [this](type v) { this->setter(v); }, [this]() { return this->getter(); });

namespace m8
{
    template<typename T>
    void TryReadOption(pt::ptree& tree, const std::string key, std::function<void(T)> setter)
    {
        auto optional = tree.get_optional<T>(key);
        if (optional.is_initialized())
        {
            setter(optional.get());
        }
    }

    void ReadPerftOptions(pt::ptree& tree, PerftOptions& options)
    {
        auto perft_tree = tree.get_child_optional("perft");
        if (perft_tree.is_initialized())
        {
            TryReadOption<int>(perft_tree.get(), "threads", [&options](int v) {options.set_threads(v); });
            TryReadOption<int>(perft_tree.get(), "min-works-items", [&options](int v) {options.set_min_works_items(v); });
        }
    }

    EvalOptions ReadEvalOptions(pt::ptree& tree)
    {
        EvalOptions options;

        TryReadOption<int>(tree, "eval.pawn", [&options](int v) {options.set_pawn(v); });
        TryReadOption<int>(tree, "eval.knight", [&options](int v) {options.set_knight(v); });
        TryReadOption<int>(tree, "eval.bishop", [&options](int v) {options.set_bishop(v); });
        TryReadOption<int>(tree, "eval.rook", [&options](int v) {options.set_rook(v); });
        TryReadOption<int>(tree, "eval.queen", [&options](int v) {options.set_queen(v); });

        return options;
    }

    template<typename T>
    void ReadArray(pt::ptree& tree, T& an_array)
    {
        std::size_t index = 0;

        for (auto& value_tree : tree)
        {
            if (index < an_array.size())
            {
                an_array[index++] = value_tree.second.get_value<typename T::value_type>();
            }
        }
    }

    template<typename T>
    void ReadArray(pt::ptree& tree, const std::string& name, T& an_array)
    {
        auto array_tree = tree.get_child_optional(name);
        if (array_tree.is_initialized())
        {
            ReadArray(array_tree.get(), an_array);
        }
    }

    std::vector<PsqtZoneValue> ReadPsqtZones(pt::ptree& tree)
    {
        std::vector<PsqtZoneValue> list;

        for (auto& zone_tree : tree)
        {
            list.emplace_back(zone_tree.second);
        }

        return list;
    }

    std::map<PieceType, std::vector<PsqtZoneValue>> ReadPsqtOptions(pt::ptree& tree)
    {
        std::map<PieceType, std::vector<PsqtZoneValue>> map;

        auto psqt_tree = tree.get_child_optional("psqt");
        if (psqt_tree.is_initialized())
        {
            for (auto& psqt_piece_tree : psqt_tree.get())
            {
                auto piece_type = GetPieceTypeFromName(psqt_piece_tree.first);
                if (IsPieceType(piece_type))
                {
                    map.insert(std::make_pair(piece_type, ReadPsqtZones(psqt_piece_tree.second)));
                }
                else
                {
                    M8_WARNING << "Unknow piece type in config file (" << psqt_piece_tree.first << ")";
                }
            }
        }

        return map;
    }

    void ReadOptionsFromFile(const std::string filename)
    {
       pt::ptree tree;
       pt::read_json(filename, tree);

       Options& options = Options::get();

       TryReadOption<std::string>(tree, "max-log-severity", [&options](std::string v) { options.set_max_log_severity(boost::lexical_cast<severity_level>(v)); });
       TryReadOption<std::string>(tree, "display-auto", [&options](std::string v) { options.set_display_auto(boost::lexical_cast<bool>(v)); });
       TryReadOption<std::string>(tree, "display-eval", [&options](std::string v) { options.set_display_eval(boost::lexical_cast<bool>(v)); });

       ReadPerftOptions(tree, options.perft());
       options.set_psqt(ReadPsqtOptions(tree));
       options.set_eval(ReadEvalOptions(tree));
    }


    po::options_description GenerateOptionsDescriptions()
    {
        auto& options = Options::get();

        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            M8_OPTIONS_DEFINITIONS(M8_DESC_OPTIONS);

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

    template<typename T>
    void Options::AddOption(const std::string& name, const std::string& desc, typename TypedOption<T>::setter_type setter, typename TypedOption<T>::getter_type getter)
    {
        options_.insert(Storage::value_type(name, std::make_shared<TypedOption<T>>(name, desc, setter, getter)));
    }

    Options::Options()
        : options_()
    {
        M8_OPTIONS_DEFINITIONS(M8_ADD_OPTIONS);
    }
}