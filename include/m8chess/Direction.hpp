/// @file   Direction.hpp
/// @author Mathieu Pag�
/// @date   Janurary 2018
/// @brief  Contains types and method about directions on a chess board.

#ifndef M8_DIRECTION_HPP_
#define M8_DIRECTION_HPP_

#include <cassert>
#include <cstdint>
#include <array>

#include "m8common/0x88.hpp"

#include "Sq.hpp"

namespace m8
{
    /// Delta to apply to 0x88 difference before indexing the array.
    const size_t kDeltaDirectionArray = 120;

    /// Represent a direction on a chess board.
    ///
    /// Diagonal are the same direction as the a1-h8 diagonal. Anti-diagonal are parallel
    /// to the a8-h1 diagonal.
    enum class Direction : std::uint8_t
    {
        Horizontal,
        Vertical,
        Diagonal,
        AntiDiagonal,
        None
    };

    /// Array containing the directions of the relation of two squares indexed by 0x88 
    /// difference.
    extern std::array<Direction, 240> kDirectionArray;

    /// Get the direction of the relation between two squares.
    inline Direction GetDirection(Sq from, Sq to)
    {
        assert(IsSqOnBoard(from));
        assert(IsSqOnBoard(to));

        return kDirectionArray[CalculateOx88diff(from, to) + kDeltaDirectionArray];
    }

    /// Initialize precalculations for the type Direction. Should be called once at the
    /// start of the application.
    void InitializeDirection();
}

#endif // M8_DIRECTION_HPP_