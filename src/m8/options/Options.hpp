/// @file   Options.hpp
/// @author Mathieu Pagé
/// @date   January 2017
/// @brief  Contains the declarations of the Options class that handles m8 command line 
///         parameters and m8.ini file.

#ifndef M8_OPTIONS_HPP_
#define M8_OPTIONS_HPP_

#include <iostream>

#include <boost/program_options.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "OptionsDefinitions.hpp"
#include "Option.hpp"

#define M8_OPTIONS_PRIVATE_ATTRIBUTES(name, desc, type, default_value)            TypedOption<type> name##_;
#define M8_OPTIONS_DEFAULT_VALUES_INITIALISATION(name, desc, type, default_value) name##_ = default_value;
#define M8_OPTIONS_ACCESSORS(name, desc, type, default_value)                     TypedOption<type> name() const { return name##_; };
#define M8_OPTIONS_INITIALISATION(name, desc, type, default_value)                name##_(boost::replace_all_copy(std::string(#name), "_", "-"), desc, default_value),
#define M8_OPTIONS_CREATE_MAP(name, desc, type, default_value)                    options_.emplace(boost::replace_all_copy(std::string(#name), "_", "-"), name##_);

namespace m8
{
    class Options
    {
    public:
        typedef std::map<std::string, Option&> Storage;

        /// Get the only instance of the class (Singleton pattern)
        inline static Options& get();

        /// Delete the copy constructor
        Options(Options const&) = delete;

        /// Delete the copy operator
        void operator=(Options const&) = delete;

        // Create the get functions for all the options
        M8_OPTIONS_DEFINITIONS(M8_OPTIONS_ACCESSORS)

        /// Give read only access to the options map.
        inline const Storage& map() const { return options_; }

        /// Read the options from the command line
        ///
        /// @param argc  Number of arguments on the command line. This information is 
        ///              passed to the main function.
        /// @param argv  Values of the arguments.
        /// @param out   Output stream the function can use to display informations to 
        ///              the user.
        /// @returns     A boolean value indicating if we must stop the execution. This 
        ///              will be set to true if the users asked to display to availables
        ///              options.
        bool ReadOptions(int argc, char** argv, std::ostream& out);

        /// Read the options from a file
        ///
        /// @param file  Stream opened on a file containings options.
        void ReadOptions(std::istream& file);

        /// Create a new options file on the provided stream.
        void CreateOptionsFile(std::ostream& out);

    private:
        /// Private constructor to prevent initialisation outside the class (Singleton 
        /// pattern).
        Options()
            : M8_OPTIONS_DEFINITIONS(M8_OPTIONS_INITIALISATION) options_()
        {
            M8_OPTIONS_DEFINITIONS(M8_OPTIONS_CREATE_MAP)
        }

        // Generate the privates attributes that will contains the options values.
        M8_OPTIONS_DEFINITIONS(M8_OPTIONS_PRIVATE_ATTRIBUTES)

        Storage options_;

        /// Generate the program_options options descriptions object
        static boost::program_options::options_description GenerateOptionsDescriptions();
    };

    inline Options& Options::get()
    {
        static Options options;
        return options;
    }
}

#endif // M8_OPTIONS_HPP_
