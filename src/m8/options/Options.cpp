/// @file   Options.cpp
/// @author Mathieu Pagé
/// @date   January 2017
/// @brief  Contains the declarations of the Options class that handles m8 command line 
///         parameters and m8.ini file.

#include <boost/program_options.hpp>
#include <boost/algorithm/string/replace.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "../../m8common/logging.hpp"
#include "Options.hpp"

namespace po = boost::program_options;
namespace pt = boost::property_tree;

#define M8_DESC_OPTIONS(name, desc, ref, type)      (name, po::value<type>(&options.ref), desc)
#define M8_ADD_OPTIONS(name, desc, ref, type)       AddOption(name, desc, this->##ref);

namespace m8
{
    template<typename T1, typename T2>
    void TryReadOption(pt::ptree& tree, const std::string key, T2& ref)
    {
        auto optional = tree.get_optional<T1>(key);
        if (optional.is_initialized())
        {
            ref = static_cast<T2>(optional.get());
        }
    }

    void ReadPerftOptions(pt::ptree& tree, PerftOptions& options)
    {
        auto perft_tree = tree.get_child_optional("perft");
        if (perft_tree.is_initialized())
        {
            TryReadOption<int>(perft_tree.get(), "threads", options.threads);
            TryReadOption<int>(perft_tree.get(), "min-works-items", options.min_works_items);
        }
    }

    template<typename T>
    void ReadArray(pt::ptree& tree, T& an_array)
    {
        std::size_t index = 0;

        for (auto& value_tree : tree)
        {
            if (index < an_array.size())
            {
                an_array[index++] = value_tree.second.get_value<T::value_type>();
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

    void ReadPsqtPieceOptions(pt::ptree& tree, PsqtPieceOptions& psqt_options)
    {
        ReadArray(tree, "columns", psqt_options.columns);
        ReadArray(tree, "rows", psqt_options.rows);
        ReadArray(tree, "corners", psqt_options.corner);
        ReadArray(tree, "center", psqt_options.center);
    }

    void ReadPsqtOptions(pt::ptree& tree, std::array<PsqtPieceOptions, kMaxPieceType + 1>& psqt_pieces)
    {
        auto psqt_tree = tree.get_child_optional("psqt");
        if (psqt_tree.is_initialized())
        {
            for (auto& psqt_piece_tree : psqt_tree.get())
            {
                auto piece_type = GetPieceTypeFromName(psqt_piece_tree.first);
                if (IsPieceType(piece_type))
                {
                    ReadPsqtPieceOptions(psqt_piece_tree.second, psqt_pieces[piece_type]);
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

       TryReadOption<int>(tree, "max-log-severity", options.max_log_severity);

       ReadPerftOptions(tree, options.perft);
       ReadPsqtOptions(tree, options.psqt);
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
    void Options::AddOption(const std::string& name, const std::string& desc, T& ref)
    {
        options_.insert(Storage::value_type(name, std::make_shared<TypedOption<T>>(name, desc, ref)));
    }

    Options::Options()
        : options_()
    {
        M8_OPTIONS_DEFINITIONS(M8_ADD_OPTIONS);
    }
}