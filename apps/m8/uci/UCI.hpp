/// @file uci.hpp
/// @author Mathieu Pagé
/// @date February 2024
/// @brief Contains the UCI class

#ifndef M8_UCI_HPP_
#define M8_UCI_HPP_

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "UCIEngine.hpp"

namespace m8::uci
{
    /// Class that manage the uci protocol
    class UCI
    {
    public:
        /// Run the UCI interface
        void Run();

    private:
        void HandleUci(const std::vector<std::string> params);
        void HandleIsready(const std::vector<std::string> params);
        void HandlePosition(const std::vector<std::string> params);
        void HandleGo(const std::vector<std::string> params);
        void HandleStop(const std::vector<std::string> params);
        void HandleSetOption(const std::vector<std::string> params);

        static const std::unordered_map<std::string, std::function<void(UCI*, const std::vector<std::string>&)>> HANDLERS;

        UCIEngine engine_;
    };
}

#endif // M8_UCI_HPP_