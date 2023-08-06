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

#include "../../m8chess/Piece.hpp"
#include "../../m8common/Bb.hpp"
#include "../../m8common/logging.hpp"
#include "../../m8chess/Types.hpp"

#include "ModifiableOption.hpp"

namespace m8::options
{
    struct PsqtZone
    {
        /// Human readable name for the zone
        std::string name;

        /// Bitboard representing all the squares of the zone
        Bb zone;

        /// Value (bonus or malus) applied to the zone
        std::int32_t value;
    };

    /// Class containing options related to the perft command
    struct PerftOptions
    {
        /// Numbers of parallel threads to use for the perft command.
        std::int32_t threads = 16;

        /// Minimum number of work item the perft load is separated in.
        std::int32_t min_works_items = 100;
    };

    /// Class containing options related to the evaluation function
    struct EvalOptions
    {
        /// Value of a pawn
        std::int32_t pawn = 100;

        /// Value of a knight
        std::int32_t knight = 320;

        /// Value of a bishop
        std::int32_t bishop = 333;

        /// Value of a rook
        std::int32_t rook = 510;

        /// Value of a queen
        std::int32_t queen = 880;

        /// Zones values used to generate the piece/square table
        std::unordered_map<PieceType, std::vector<PsqtZone>> psqt_zones;

        /// Return the value of a give piece_type.
        inline int get_value_piece_type(PieceType piece_type) const
        {
            assert(IsPieceType(piece_type));

            int value;

            switch (piece_type)
            {
            case kPawn:
                value = pawn;
                break;
            case kKnight:
                value = knight;
                break;
            case kBishop:
                value = bishop;
                break;
            case kRook:
                value = rook;
                break;
            case kQueen:
                value = queen;
                break;
            default:
                value = 0;
                break;
            }

            return value;
        }
    };

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
        PerftOptions perft;

        /// Max log severity
        severity_level max_log_severity;

        /// Map of a list of ModifiableOption that allows to list or modify options at 
        /// runtime.
        ModifiableOptions modifiable_options;

        /// Parameter indicating if the evaluation should be displayed
        bool display_eval = true;

        /// Parameter indicating if the board should be displayed automatically
        bool display_auto = true;

        /// Minimum depth of the displayed engine thinking
        DepthType min_display_depth = 4;

        /// Indicate if the engine should use the Standatd Algebraic Notation. If it is
        /// false, the Coordinate Algebraic Notation is used instead.
        bool use_san;

        /// Options related to the evaluation function
        EvalOptions eval;        

    private:
        /// Consctructor
        Options()
        : max_log_severity(severity_level::none)
        {}

        void CreateModifiableOptions();
    
    public:
        Options(Options const&)        = delete;
        void operator=(Options const&) = delete;
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

#endif // M8_OPTIONS_OPTIONS_HPP_