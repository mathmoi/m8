/// @file UCIInterface.hpp
/// @author Mathieu Pagé
/// @date February 2024
/// @brief Contains the UCIInterface class

#ifndef M8_UCI_UCI_INTERFACE_HPP_
#define M8_UCI_UCI_INTERFACE_HPP_

#include <chrono>
#include <optional>
#include <string> // TODO: Est-ce qu'on peut tout remplacer par des string_view?
#include <string_view>

#include "m8common/Output.hpp"

#include "m8chess/Types.hpp"
#include "m8chess/search/PV.hpp"

#include "UCIOptionType.hpp"

namespace m8::uci
{
    /// This class represent the GUI in a UCI protocol and provides methods that allow
    /// communication from the engine to the GUI.
    class UCIInterface
    {
    public:
        /// Send the id command to the gui.
        void SendId(const std::string& id_type, const std::string& id_value);

        /// Send the uciok command to the gui.
        void SendUciok();

        /// Send the readyok command to the gui.
        void SendReadyok();

        /// Send the bestmove command to the gui.
        void SendBestMove(const std::string& move, const std::optional<std::string>& ponder_move);

        /// Send the info command to the gui
        /// 
        /// @param depth search depth in plies
        /// @param sel_depth selective search depth in plies
        /// @param time the time searched in ms
        /// @param nodes x nodes searched
        /// @param pv the best line found
        /// @param multi_pv this for the multi pv mode, identify the pv sent
        /// @param score the score from the engine's point of view
        /// @param current_move currently searching this move
        /// @param current_move_number urrently searching move number x
        /// @param hash_full the hash is x permill full
        /// @param nps  nodes per second searched
        /// @param tb_hits x positions where found in the endgame table bases
        /// @param cpu_load the cpu usage of the engine is x permill
        /// @param string any string str which will be displayed be the engine
        void SendInfo(std::optional<DepthType>                 depth,
                      std::optional<DepthType>                 sel_depth,
                      std::optional<std::chrono::milliseconds> time,
                      std::optional<NodeCounterType>           nodes,
                      std::optional<std::vector<std::string>>  pv,
                      std::optional<std::uint16_t>             multi_pv,
                      std::optional<EvalType>                  score,
                      std::optional<std::string>               current_move,
                      std::optional<std::uint16_t>             current_move_number,
                      std::optional<std::uint16_t>             hash_full,
                      std::optional<NodeCounterType>           nps,
                      std::optional<NodeCounterType>           tb_hits,
                      std::optional<std::uint16_t>             cpu_load,
                      std::optional<std::string>               string);
        
        /// Sends an option of type Check.
        /// 
        /// @param name Name of the option
        /// @param default_value Default value
        void SendCheckOption(std::string_view name, bool default_value);

        /// Sends an option of type Spin.
        /// 
        /// @param name Name of the option
        /// @param min  Minum value
        /// @param max  Maximum value
        /// @param default_value Default value
        void SendSpinOption(std::string_view name, std::int64_t min, std::int64_t max, std::int64_t default_value);

        /// Sends an option of type Combo.
        /// 
        /// @param name Name of the option
        /// @param values List of possible values
        /// @param default_value Default value
        void SendComboOption(std::string_view name, const std::vector<std::string>& values, std::string_view default_value);

        /// Sends an option of type String
        /// 
        /// @param name Name of the option
        /// @param default_value Default value
        void SendStringOption(std::string_view name, std::string_view default_value);

        /// Sends an option of type Button
        /// 
        /// @param name Name of the option
        void SendButtonOption(std::string_view name);
    
    private:
        m8::Output out_;

        void SendOption(std::string_view name,
                        std::string_view type,
                        std::optional<std::string_view>         default_value = std::nullopt,
                        std::optional<std::int64_t>             min           = std::nullopt,
                        std::optional<std::int64_t>             max           = std::nullopt,
                        std::optional<std::vector<std::string>> values        = std::nullopt);
    };
}

#endif // M8_UCI_UCI_INTERFACE_HPP_