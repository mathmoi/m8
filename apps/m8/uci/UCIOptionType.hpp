/// @file UCIOptionType.hpp
/// @author Mathieu Pagé
/// @date February 2024
/// @brief Contains the UCIOptionType enum

#ifndef M8_UCI_UCI_OPTION_TYPE_HPP_
#define M8_UCI_UCI_OPTION_TYPE_HPP_

namespace m8::uci
{
    /// Type of an engine option in the UCI protocol.
    enum UCIOptionType
    {
        /// A checkbox that can either be true or false.
        Check,

        /// A spin wheel that can be an integer in a certain range.
        Spin,

        /// A combo box that can have different predefined strings as a value.
        Combo,

        /// A button that can be pressed to send a command to the engine.
        Button,

        /// A text field that has a string as a value.
        String
    };
}

#endif // M8_UCI_UCI_OPTION_TYPE_HPP_