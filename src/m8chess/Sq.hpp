/// @file   Sq.hpp
/// @author Mathieu Pagé
/// @date   Feburary 2015
/// @brief  Contains the definition of types and utilities used to manipulates 
///         chess board coordinates.

#ifndef M8_SQ_HPP_
#define M8_SQ_HPP_

#include <cstdint>
#include <cassert>

#include "Color.hpp"
#include "Column.hpp"
#include "Row.hpp"

namespace m8
{
    /// Type that represent the index of a diagonal (or anti-diagonal) on a chess board.
    typedef std::int8_t Diagonal; // TODO : Something

    /// <summary>
    ///  Squares on a chess board
    /// </summary>
    class Sq
    {
    private:
        std::uint8_t value_;

    public:
        /// <summary>
        ///  Default constructor
        /// </summary>
        Sq() {}

        /// <summary>
        ///  Constructor
        /// </summary>
        /// <param name="value">Value of the square</param>
        Sq(std::uint8_t value)
            : value_(value)
        {};

        /// <summary>
        ///  Create a new square from a column and a row
        /// </summary>
        /// <param name="column">Column</param>
        /// <param name="row">Row</param>
        Sq(Column column, Row row)
        {
            // A : The column and row are valid
            assert(column.IsOnBoard());
            assert(row.IsOnBoard());

            value_ = (row.value() << 3) + column.value();
        }

        /// <summary>
        ///  Number of squares on a chess board
        /// </summary>
        static const std::uint8_t NumSqOnBoard = 64;

        /// <summary>
        ///  Indicate if the current row is on the board.
        /// </summary>
        /// <returns>True if the row is valid.</returns>
        inline bool IsOnBoard() const { return value_ < NumSqOnBoard; }

        /// <summary>
        ///  Return the value of the square.
        /// </summary>
        inline std::uint8_t value() const { return value_; }
        
        /// <summary>
        ///  Get the column of the square
        /// </summary>
        inline Column column() const
        {
            // A : The square is valid
            assert(IsOnBoard());

            return Column(value_ & 7);
        }

        /// <summary>
        ///  Get the row of the square
        /// </summary>
        inline Row row() const
        {
            // A : The square is valid
            assert(IsOnBoard());

            return Row(value_ >> 3);
        }

        /// <summary>
        ///  Get the index of the diagonals the square is on.
        /// </summary>
        inline std::uint8_t diag() const
        {
            // A : The square is valid
            assert(IsOnBoard());

            return 7 - column().value() + row().value();
        }

        /// <summary>
        ///  Get the index of the anti-diagonals the square is on.
        /// </summary>
        inline std::uint8_t anti_diag()
        {
            // A : The square is valid
            assert(IsOnBoard());

            return column().value() + row().value();
        }

        /// <summary>
        ///  Return the next squares in a sequence of all squares. Can be used to iterate
        ///  over all squares.
        /// </summary>
        inline Sq MoveNext() const { assert(IsOnBoard()); return value_ + 1; };

        /// <summary>
        ///  Return the square, one squares up from the current square.
        /// </summary>
        inline Sq MoveUp() const;

        /// <summary>
        ///  Return the square, one squares down from the current square.
        /// </summary>
        inline Sq MoveDown() const;

        /// <summary>
        ///  Return the square, one squares left from the current square.
        /// </summary>
        inline Sq MoveLeft() const;

        /// <summary>
        ///  Return the square, one squares right from the current square.
        /// </summary>
        inline Sq MoveRight() const;

        inline static Sq A1() { return Sq(0); };
        inline static Sq B1() { return Sq(1); };
        inline static Sq C1() { return Sq(2); };
        inline static Sq D1() { return Sq(3); };
        inline static Sq E1() { return Sq(4); };
        inline static Sq F1() { return Sq(5); };
        inline static Sq G1() { return Sq(6); };
        inline static Sq H1() { return Sq(7); };

        inline static Sq A2() { return Sq(8); };
        inline static Sq B2() { return Sq(9); };
        inline static Sq C2() { return Sq(10); };
        inline static Sq D2() { return Sq(11); };
        inline static Sq E2() { return Sq(12); };
        inline static Sq F2() { return Sq(13); };
        inline static Sq G2() { return Sq(14); };
        inline static Sq H2() { return Sq(15); };

        inline static Sq A3() { return Sq(16); };
        inline static Sq B3() { return Sq(17); };
        inline static Sq C3() { return Sq(18); };
        inline static Sq D3() { return Sq(19); };
        inline static Sq E3() { return Sq(20); };
        inline static Sq F3() { return Sq(21); };
        inline static Sq G3() { return Sq(22); };
        inline static Sq H3() { return Sq(23); };

        inline static Sq A4() { return Sq(24); };
        inline static Sq B4() { return Sq(25); };
        inline static Sq C4() { return Sq(26); };
        inline static Sq D4() { return Sq(27); };
        inline static Sq E4() { return Sq(28); };
        inline static Sq F4() { return Sq(29); };
        inline static Sq G4() { return Sq(30); };
        inline static Sq H4() { return Sq(31); };

        inline static Sq A5() { return Sq(32); };
        inline static Sq B5() { return Sq(33); };
        inline static Sq C5() { return Sq(34); };
        inline static Sq D5() { return Sq(35); };
        inline static Sq E5() { return Sq(36); };
        inline static Sq F5() { return Sq(37); };
        inline static Sq G5() { return Sq(38); };
        inline static Sq H5() { return Sq(39); };

        inline static Sq A6() { return Sq(40); };
        inline static Sq B6() { return Sq(41); };
        inline static Sq C6() { return Sq(42); };
        inline static Sq D6() { return Sq(43); };
        inline static Sq E6() { return Sq(44); };
        inline static Sq F6() { return Sq(45); };
        inline static Sq G6() { return Sq(46); };
        inline static Sq H6() { return Sq(47); };

        inline static Sq A7() { return Sq(48); };
        inline static Sq B7() { return Sq(49); };
        inline static Sq C7() { return Sq(50); };
        inline static Sq D7() { return Sq(51); };
        inline static Sq E7() { return Sq(52); };
        inline static Sq F7() { return Sq(53); };
        inline static Sq G7() { return Sq(54); };
        inline static Sq H7() { return Sq(55); };

        inline static Sq A8() { return Sq(56); };
        inline static Sq B8() { return Sq(57); };
        inline static Sq C8() { return Sq(58); };
        inline static Sq D8() { return Sq(59); };
        inline static Sq E8() { return Sq(60); };
        inline static Sq F8() { return Sq(61); };
        inline static Sq G8() { return Sq(62); };
        inline static Sq H8() { return Sq(63); };

        inline static Sq Invalid() { return Sq(255); };

        friend bool operator==(Sq lhs, Sq rhs);
    };

    inline bool operator==(Sq lhs, Sq rhs) { return lhs.value_ == rhs.value_; }
    inline bool operator!=(Sq lhs, Sq rhs) { return !(lhs == rhs); }

    inline Sq Sq::MoveUp() const
    {
        assert(IsOnBoard());
        return value_ + 8;
    }

    inline Sq Sq::MoveDown() const
    {
        assert(IsOnBoard());
        return value_ - 8;
    }

    inline Sq Sq::MoveLeft() const
    {
        assert(IsOnBoard());
        return value_ - 1;
    }

    inline Sq Sq::MoveRight() const
    {
        assert(IsOnBoard());
        return value_ + 1;
    }
}

#endif // M8_SQ_HPP_