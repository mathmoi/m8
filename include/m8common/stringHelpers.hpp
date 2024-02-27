/// @file stringHelpers.hpp
/// @author Mathieu Pag�
/// @date Novembre 2017
/// @brief File  containing helper methods to work with strings.

#ifndef M8_STRING_HELPERS_HPP_
#define M8_STRING_HELPERS_HPP_

#include <string>
#include <vector>

namespace m8
{
    /// Join strings together using a separator.
    ///
    /// @param first     Iterator pointing on the first string to join.
    /// @param last      Iterator pointing past the last string to join.
    /// @param separator Separator to insert between each strings.
    /// @returns         A concatened string.
    template<class Iterator> std::string Join(Iterator first, Iterator last, std::string separator)
    {
        std::string result("");
        for (auto it = first; it < last; ++it)
        {
            if (first < it)
            {
                result += separator;
            }

            result += *it;
        }

        return result;
    }

    /// Split a string on whitespaces.
    /// 
    /// @param str String to split
    /// @return a vector containing all the "words" in the string.
    std::vector<std::string> Split(const std::string& str);
}

#endif // M8_STRING_HELPERS_HPP_