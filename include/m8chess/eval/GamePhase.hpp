/// @file GamePhase.hpp
/// @author Mathieu Pagé
/// @date March 2024
/// Contains the definition of the GamePhase enum type.

#ifndef M8_EVAL_GAME_PHASE_HPP_
#define M8_EVAL_GAME_PHASE_HPP_

#include <cstdint>

#include "m8chess/Piece.hpp"

namespace m8::eval
{
    typedef std::uint32_t GamePhaseEstimate;

    const GamePhaseEstimate kGamePhasePawnWeight   = 0;
    const GamePhaseEstimate kGamePhaseKnightWeight = 1;
    const GamePhaseEstimate kGamePhaseBishopWeight = 1;
    const GamePhaseEstimate kGamePhaseRookWeight   = 2;
    const GamePhaseEstimate kGamePhaseQueenWeight  = 4;
    const GamePhaseEstimate kGamePhaseKingWeight   = 0;

    const GamePhaseEstimate kGamePhaseEstimateMax = kGamePhasePawnWeight   * 8
                                                  + kGamePhaseKnightWeight * 4
                                                  + kGamePhaseBishopWeight * 4
                                                  + kGamePhaseRookWeight   * 4
                                                  + kGamePhaseQueenWeight  * 2
                                                  + kGamePhaseKingWeight   * 2;


    inline const std::array<GamePhaseEstimate, kMaxPiece + 1> kPiecePhaseEstimate =
    {
        0,
        kGamePhasePawnWeight,
        kGamePhaseKnightWeight,
        kGamePhaseKingWeight,
        kGamePhaseQueenWeight,
        kGamePhaseBishopWeight,
        kGamePhaseRookWeight,
        0,
        0,
        kGamePhasePawnWeight,
        kGamePhaseKnightWeight,
        kGamePhaseKingWeight,
        kGamePhaseQueenWeight,
        kGamePhaseBishopWeight,
        kGamePhaseRookWeight
    };
    

    /// Phases of the game in regards to the evaluation
    enum class GamePhase
    {
        MiddleGame,
        EndGame,

        // Number of game phases
        Count
    };
}

#endif // M8_EVAL_GAME_PHASE_HPP_