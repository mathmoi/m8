/// @file   Options.hpp
/// @author Mathieu Pagé
/// @date   January 2017
/// @brief  Contains the declarations of the classes that hold the engine options

#ifndef M8_OPTIONS_OPTIONS_HPP_
#define M8_OPTIONS_OPTIONS_HPP_

#include <string>
#include <cstdint>
#include <unordered_map>
#include <vector>

#include <boost/program_options.hpp>

#include "m8chess/Piece.hpp"
#include "../../m8common/Bb.hpp"
#include "../../m8common/logging.hpp"
#include "m8chess/Types.hpp"

#include "ModifiableOption.hpp"
#include "PiecesValues.hpp"
#include "PhasePieceSquareTable.hpp"

namespace m8::options
{
    /// Class containing all m8 options
    struct Options
    {
    public:
        typedef std::map<std::string, std::unique_ptr<ModifiableOption>> ModifiableOptions;

        static Options& get()
        {
            static Options instance;
            if (instance.modifiable_options.size() == 0)
            {
                instance.CreateModifiableOptions();
            }
            return instance;
        }

        /// Options of the perft command
        std::int32_t perft_threads = 16;

        /// Max log severity
        severity_level max_log_severity;

        /// Map of a list of ModifiableOption that allows to list or modify options at 
        /// runtime.
        ModifiableOptions modifiable_options;

        /// Values of the pieces for the middle game.
        PiecesValues pieces_values_middle_game;

        /// Values of the pieces for the end game.
        PiecesValues pieces_values_end_game;

        /// Values for the piece-square table
        PhasePieceSquareTable piece_square_table;

        /// Transposition table size in megabytes.
        size_t tt_size;

        /// Indicate if we are playing a chess960 (aka Fisher Random) game.
        bool chess960;

    private:
        /// Consctructor
        Options()
        : max_log_severity(severity_level::none),
          chess960(false)
        {}

        void CreateModifiableOptions();
    
    public:
        Options(Options const&)        = delete;
        void operator=(Options const&) = delete;
    };

    /// Read the properties from a configuration file.
    void ReadOptionsFromFile(const std::string filename);

    /// Generate the global options descriptions so that the options can be read from the
    /// command line.
    /// 
    /// @return po::options_description 
    boost::program_options::options_description GenerateGlobalOptionsDescriptions();
}

#endif // M8_OPTIONS_OPTIONS_HPP_