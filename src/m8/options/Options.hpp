/// @file   Options.hpp
/// @author Mathieu Pag�
/// @date   January 2017
/// @brief  Contains the declarations of the Options class that handles m8 command line 
///         parameters and m8.ini file.

#ifndef M8_OPTIONS_HPP_
#define M8_OPTIONS_HPP_

#include <iostream>
#include <array>
#include <memory>

#include <boost/program_options.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "OptionsDefinitions.hpp"
#include "Option.hpp"
#include "PsqtZoneValue.hpp"
#include "PerftOptions.hpp"
#include "EvalOptions.hpp"
#include "../../m8common/logging.hpp"
#include "../../m8chess/Sq.hpp"
#include "../../m8chess/Piece.hpp"

namespace m8
{
    struct Options
    {
        typedef std::map<std::string, std::shared_ptr<Option>> Storage;
        typedef std::map<PieceType, std::vector<PsqtZoneValue>> PsqtType;

        /// Get the only instance of the class (Singleton pattern)
        inline static Options& get()
        {
            static Options options;
            return options;
        };

        /// Options for perft calculation.
        inline PerftOptions& perft() { return perft_; }

        /// Value used to generate the piece-square table.
        inline const PsqtType psqt() const { return psqt_; }

        /// Set the values used to generate the piece-square table.
        inline void set_psqt(PsqtType value) { psqt_ = value; }

        /// Value used by the evaluation function.
        inline const EvalOptions& eval() const { return eval_; }

        /// Set the value used by the evaluation function.
        inline void set_eval(const EvalOptions& value) { eval_ = value; }
        
        /// Set the maximum log severity.
        inline void set_max_log_severity(severity_level value) { max_log_severity_ = value; }

        /// Maximum log severity level.
        inline severity_level max_log_severity() const { return max_log_severity_; }
        
        /// Give access to the options that can be modified through the user interface.
        inline const Storage& modifiable_options_map() const { return options_; }

        /// Delete the copy constructor
        Options(Options const&) = delete;

        /// Delete the copy operator
        void operator=(Options const&) = delete;

    private:
        /// Private constructor;
        Options();

        template<typename T>
        void AddOption(const std::string& name, const std::string& desc, typename TypedOption<T>::setter_type setter, typename TypedOption<T>::getter_type getter);

        Storage options_;

        severity_level max_log_severity_ = severity_level::trace;
        PerftOptions perft_;
        PsqtType psqt_;
        EvalOptions eval_;

    };

    /// Read the properties from a configuration file.
    void ReadOptionsFromFile(const std::string filename);

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
    bool ReadOptionsFromCommandLine(int argc, char** argv, std::ostream& out);
}

#endif // M8_OPTIONS_HPP_
