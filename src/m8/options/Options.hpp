/// @file   Options.hpp
/// @author Mathieu Pagé
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
#include "../../m8common/logging.hpp"
#include "../../m8chess/Sq.hpp"
#include "../../m8chess/Piece.hpp"

namespace m8
{
    /// Options for perft calculations.
    struct PerftOptions
    {
        /// Numbers of threads to use to compute perft.
        int threads = 8;

        /// Minimum numbers of chunk to create to share between threads.
        int min_works_items = 100;

        /// Delete the copy constructor
        PerftOptions(PerftOptions const&) = delete;

        /// Delete the copy operator
        void operator=(PerftOptions const&) = delete;

        friend struct Options;

    private:
        /// Private constructor;
        inline PerftOptions() {};
    };

    /// Options for perft calculations.
    struct PsqtPieceOptions
    {
        /// Bonnus/malus for a piece on a given column. Only 4 values are necessary
        /// because we can use symetry to get the values of the four right-most columns.
        std::array<int, kNumColmnOnBoard / 2> columns = { 0, 0, 0, 0 };

        /// Bonnus/malus for a piece on a given row.
        std::array<int, kNumRowOnBoard> rows = { 0, 0, 0, 0, 0, 0, 0, 0 };

        /// Bonnus/malus for a piece on a corner square or a square off the corner.
        std::array<int, 2> corner = { 0, 0 };

        /// Bonnus/malus for a piece on a center square, a square off the center or a 
        /// square off off the center.
        std::array<int, 3> center = { 0, 0, 0 };

        /// Delete the copy constructor
        PsqtPieceOptions(PsqtPieceOptions const&) = delete;

        /// Delete the copy operator
        void operator=(PsqtPieceOptions const&) = delete;

        friend class std::array<PsqtPieceOptions, kMaxPieceType + 1>;

    private:
        /// Private constructor;
        inline PsqtPieceOptions() {};
    };

    struct Options
    {
        typedef std::map<std::string, std::shared_ptr<Option>> Storage;

        /// Get the only instance of the class (Singleton pattern)
        inline static Options& get()
        {
            static Options options;
            return options;
        };

        /// Options for perft calculation
        PerftOptions perft;

        /// Maximum log severity level.
        severity_level max_log_severity = severity_level::trace;

        /// Value used to generate the piece-square table.
        std::array<PsqtPieceOptions, kMaxPieceType + 1> psqt;

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
        void AddOption(const std::string& name, const std::string& desc, T& ref);

        Storage options_;

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
