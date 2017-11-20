#ifndef M8_ROW_HPP_
#define M8_ROW_HPP_

#include <cstdint>
#include <cassert>

#include "Color.hpp"

namespace m8
{
    class Row
    {
    private:
        std::uint8_t value_;

    public:
        /// <summary>
        ///  Default constructor
        /// </summary>
        Row() {}

        /// <summary>
        ///  Constructor
        /// </summary>
        /// <param name="value">Value of the row</param>
        Row(std::uint8_t value)
            : value_(value)
        {};

        /// <summary>
        ///  Number of rows on a chessboard.
        /// </summary>
        static const std::uint8_t NumRowOnBoard = 8;

        /// <summary>
        ///  Return the value of the row.
        /// </summary>
        inline std::uint8_t value() const { return value_; }

        /// <summary>
        ///  Returns the number of the row.
        /// </summary>
        /// <returns></returns>
        inline std::uint8_t number() const { assert(IsOnBoard()); return value_ + 1; }

        /// <summary>
        ///  Indicate if the current row is on the board.
        /// </summary>
        /// <returns>True if the row is valid.</returns>
        inline bool IsOnBoard() const { return value_ < NumRowOnBoard; }

        /// <summary>
        ///  Modify the column to the next row. This can be used to iterate on all rows.
        ///  The row must be valid before the move.
        /// </summary>
        /// <returns>A reference to the current object</returns>
        inline Row& MoveNext() { assert(IsOnBoard()); ++value_; return *this; }

        /// <summary>
        ///  Modify the row to the previous row. This can be used to iterate on all rows.
        ///  The row must be valid before the move.
        /// </summary>
        /// <returns>A reference to the current object</returns>
        inline Row& MovePrevious() { assert(IsOnBoard()); --value_; return *this; }
        
        /// <summary>
        ///  Returns a row relative to each color. For exemple, the first row for black
        ///  is the 8th row while to first row for white is the 1st row.
        /// </summary>
        /// <param name="color">Color</param>
        /// <param name="row">Row</param>
        /// <returns></returns>
        inline Row color_wise(Color color) const {
            return Row((value_ * OpposColor(color)) + ((7 - value_) * color));
        }

        /// <summary>
        ///  Row 1.
        /// </summary>
        inline static Row _1() { return Row(0); };

        /// <summary>
        ///  Row 2.
        /// </summary>
        inline static Row _2() { return Row(1); };

        /// <summary>
        ///  Row 3.
        /// </summary>
        inline static Row _3() { return Row(2); };

        /// <summary>
        ///  Row 4.
        /// </summary>
        inline static Row _4() { return Row(3); };

        /// <summary>
        ///  Row 5.
        /// </summary>
        inline static Row _5() { return Row(4); };

        /// <summary>
        ///  Row 6.
        /// </summary>
        inline static Row _6() { return Row(5); };

        /// <summary>
        ///  Row 7.
        /// </summary>
        inline static Row _7() { return Row(6); };

        /// <summary>
        ///  Row 8.
        /// </summary>
        inline static Row _8() { return Row(7); };

        /// <summary>
        ///  Invalid row
        /// </summary>
        inline static Row Invalid() { return Row(255); };

        inline Row& operator += (std::int32_t rhs) { value_ += rhs; return *this; };

        friend bool operator==(Row lhs, Row rhs);
        friend bool operator<(Row lhs, Row rhs);
        friend bool operator>(Row lhs, Row rhs);
        friend bool operator<=(Row lhs, Row rhs);
        friend bool operator>=(Row lhs, Row rhs);
    };

    inline bool operator==(Row lhs, Row rhs) { return lhs.value_ == rhs.value_; }
    inline bool operator!=(Row lhs, Row rhs) { return !(lhs == rhs); }
    inline bool operator<(Row lhs, Row rhs) { return lhs.value_ < rhs.value_; }
    inline bool operator>(Row lhs, Row rhs) { return lhs.value_ > rhs.value_; }
    inline bool operator<=(Row lhs, Row rhs) { return lhs.value_ <= rhs.value_; }
    inline bool operator>=(Row lhs, Row rhs) { return lhs.value_ >= rhs.value_; }
}

#endif // M8_ROW_HPP_