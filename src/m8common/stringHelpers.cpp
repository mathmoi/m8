/// @file stringHelpers.cpp
/// @author Mathieu Pagé
/// @date February 2024
/// @brief Contains implementations of string helpers

#include <sstream>

#include "m8common/stringHelpers.hpp"

namespace m8
{
    std::vector<std::string> Split(const std::string& str)
    {
        std::vector<std::string> result;
        std::istringstream iss(str);
        std::string token;
        while (iss >> token) {
            result.push_back(token);
        }
        return result;
    }
}