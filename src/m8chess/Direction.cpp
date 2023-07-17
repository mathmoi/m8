/// @file   Direction.cpp
/// @author Mathieu Pag�
/// @date   Janurary 2018
/// @brief  Contains types and method about directions on a chess board.

#include "Direction.hpp"
#include "Sq.hpp"

namespace m8
{
    std::array<Direction, 240> kDirectionArray;

    void InitializeDirection()
    {
        kDirectionArray.fill(Direction::None);

        Sq from = kA1;
        for (Sq to = kB1; IsSqOnBoard(to) && to <= kH1; to = MoveSqRight(to))
        {
            kDirectionArray[CalculateOx88diff(from, to) + kDeltaDirectionArray] = Direction::Horizontal;
            kDirectionArray[CalculateOx88diff(to, from) + kDeltaDirectionArray] = Direction::Horizontal;
        }

        from = kA1;
        for (Sq to = kA2; IsSqOnBoard(to) && to <= kA8; to = MoveSqUp(to))
        {
            kDirectionArray[CalculateOx88diff(from, to) + kDeltaDirectionArray] = Direction::Vertical;
            kDirectionArray[CalculateOx88diff(to, from) + kDeltaDirectionArray] = Direction::Vertical;
        }

        from = kA1;
        for (Sq to = kB2; IsSqOnBoard(to) && to <= kH8; to = MoveSqUpRight(to))
        {
            kDirectionArray[CalculateOx88diff(from, to) + kDeltaDirectionArray] = Direction::Diagonal;
            kDirectionArray[CalculateOx88diff(to, from) + kDeltaDirectionArray] = Direction::Diagonal;
        }

        from = kA8;
        for (Sq to = kB7; IsSqOnBoard(to) && to >= kH1; to = MoveSqDownRight(to))
        {
            kDirectionArray[CalculateOx88diff(from, to) + kDeltaDirectionArray] = Direction::AntiDiagonal;
            kDirectionArray[CalculateOx88diff(to, from) + kDeltaDirectionArray] = Direction::AntiDiagonal;
        }
    }
}
