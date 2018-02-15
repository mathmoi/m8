/// @file   PsqtZoneValue.hpp
/// @author Mathieu Pagé
/// @date   Feburary 2017
/// @brief  Contains a class that represent a zone of a piece-square table in a 
///         configuration file.

#ifndef M8_PSQT_ZONE_VALUE_HPP_
#define M8_PSQT_ZONE_VALUE_HPP_

#include <string>
#include <cstdint>

#include <boost/property_tree/ptree.hpp>
#include <boost/lexical_cast.hpp>

#include "../../m8common/Bb.hpp"
#include "../../m8common/Utils.hpp"

namespace m8
{
    /// Classe representing the value of a zone in a piece's piece-square table. For 
    /// exemple an instance of this class could contains the bonus (or malus) for a piece
    /// on a give row. The zone is defined as a bitboard.
    class PsqtZoneValue
    {
    public:

        /// Constructor. Build an instance from data read in a ptree usually comming from
        /// a configuration file.
        inline PsqtZoneValue(boost::property_tree::ptree& tree)
        {
            name_ = tree.get<std::string>("name");
            auto zone_str = tree.get<std::string>("zone");
            zone_ = ConvertTo<Bb>(zone_str, true);
            value_ = tree.get<std::int32_t>("value");
        };

        /// Accessor for the name of the zone.
        inline const std::string& name() const { return name_; };

        /// Accessor for the bitboard that define the zone.
        inline Bb zone() const { return zone_; };

        /// Accessor for the bonus or malus associated with the zone.
        inline std::int32_t value() const { return value_; };


    private: 
        std::string name_;
        Bb zone_;
        std::int32_t value_;
    };
}

#endif // M8_PSQT_ZONE_VALUE_HPP_