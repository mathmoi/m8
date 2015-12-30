/// @file   Utils.hpp
/// @author Mathieu Pagé
/// @date   April 2015
/// @brief  Contains generic tools used in m8.

#include <exception>
#include <string>
#include <sstream>
#include <algorithm>

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

    /**
    * Convert a string into a numeric type.
    *
    * @param str String to convert
    * @param failIfLeftoverChars Flag indicating if an exception must be raised
    *                            if there is leftover characters.
    */
    template<class T>
    inline T ConvertTo(std::string str, bool failIfLeftoverChars = true)
    {
        std::istringstream stream(str);
        T retour;
        char c;
        if (!(stream >> retour) || (failIfLeftoverChars && stream.get(c)))
            throw BadConvr("ConvertTo(\"" + str + "\")");
        return retour;
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