/// @file   Options.cpp
/// @author Mathieu Pagé
/// @date   January 2017
/// @brief  Contains the declarations of the Options class that handles m8 command line 
///         parameters and m8.ini file.

#include <boost/program_options.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "Options.hpp"

#define OPTIONS_ADD_OPTIONS_PO(name, desc, type, default_value)     (boost::replace_all_copy(std::string(#name), "_", "-").c_str(), po::value<type>(), desc)
#define OPTIONS_READ_VALUES_PO(name, desc, type, default_value)     if (vm.count(boost::replace_all_copy(std::string(#name), "_", "-"))) {name##_.set_value(vm.at(boost::replace_all_copy(std::string(#name), "_", "-")).as<type>());}
#define OPTIONS_OUTPUT_FILE(name, desc, type, default_value)        <<"# " <<desc <<"\n#" <<boost::replace_all_copy(std::string(#name), "_", "-") <<'=' <<default_value <<"\n\n"

namespace po = boost::program_options;

namespace m8
{
    po::options_description Options::GenerateOptionsDescriptions()
    {
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            OPTIONS_DEFINITIONS(OPTIONS_ADD_OPTIONS_PO)
            ;

        return desc;
    }

    bool Options::ReadOptions(int argc, char** argv, std::ostream& out)
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
        else
        {
            OPTIONS_DEFINITIONS(OPTIONS_READ_VALUES_PO)
        }


        return stop_execution;
    }

    void Options::ReadOptions(std::istream& file)
    {
        auto desc = GenerateOptionsDescriptions();
        po::variables_map vm;
        po::store(po::parse_config_file(file, desc), vm);
        po::notify(vm);
        
        OPTIONS_DEFINITIONS(OPTIONS_READ_VALUES_PO)
    }

    void Options::CreateOptionsFile(std::ostream& out)
    {
        out OPTIONS_DEFINITIONS(OPTIONS_OUTPUT_FILE);
    }
}