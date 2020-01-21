/// @file   Utils.hpp
/// @author Mathieu Pagé
/// @date   April 2015
/// @brief  Contains generic tools used in m8.

#ifndef M8_UTILS_HPP_
#define M8_UTILS_HPP_

#include <exception>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <iomanip>

namespace m8
{
    /**
    * Exception class used by ConvertTo.
    */
    class BadConvr : public std::runtime_error
    {
    public:
        BadConvr(const std::string& s)
            : std::runtime_error(s)
        {};
    };


    /// Convert a string into a numeric type.
    ///
    /// @param str String to convert.
    /// @param hex Indicate if the input string is in hexadecimal.
    /// @param failIfLeftoverChars Flag indicating if an exception must be raised if there
    ///                            is leftover characters.
    template<class T>
    inline T ConvertTo(std::string str, bool hex = false, bool failIfLeftoverChars = true)
    {
        std::istringstream stream(str);
        T retour;
        char c;
        if (hex) stream >> std::hex;
        if (!(stream >> retour) || (failIfLeftoverChars && stream.get(c)))
            throw BadConvr("ConvertTo(\"" + str + "\")");
        return retour;
    }

    template<class T>
    std::string ToString(T value)
    {
        std::ostringstream out;
        out << value;
        return out.str();
    }

    /// Convert a number to string using the bigest metric suffixe possible.
    ///
    /// @param number    Number to convert to a string.
    /// @param precision Number of decimal digit to use in output.
    inline std::string AddMetricSuffix(std::uint64_t number, int precision)
    {
        static const std::array<std::string, 7> suffixes = { {"", "k", "M", "G", "T", "P", "E"} };
        auto index = static_cast<std::size_t>((number > 0 ? std::log10(number) / 3 : 0));

        std::ostringstream out;
        out << std::setiosflags(std::ios::fixed) << std::setprecision(precision) << (number / std::pow(1000, index)) << suffixes[index];
        return out.str();
    }

    /// Verify if an elements is in a container.
    ///
    /// @param first Initial position of the sequence to search.
    /// @param last  final position of the sequence to search.
    /// @param val   Searched value.
    /// @return True if the elementa can be found in the sequence, false otherwise.
    template <class InputIterator, class T>
    inline bool Contains(InputIterator first, InputIterator last, const T& val)
    {
        return std::find(first, last, val) != last;
    }
}

#endif