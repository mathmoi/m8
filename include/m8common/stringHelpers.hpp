/// @file stringHelpers.hpp
/// @author Mathieu Pag�
/// @date Novembre 2017
/// @brief File  containing helper methods to work with strings.

#ifndef M8_STRING_HELPERS_HPP_
#define M8_STRING_HELPERS_HPP_

#include <string>

namespace m8
{
    /// Join strings together using a separator.
    ///
    /// @param first     Iterator pointing on the first string to join.
    /// @param last      Iterator pointing past the last string to join.
    /// @param separator Separator to insert between each strings.
    /// @returns         A concatened string.
    template<class Iterator> std::string join(Iterator first, Iterator last, std::string separator)
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
}

#endif // M8_STRING_HELPERS_HPP_