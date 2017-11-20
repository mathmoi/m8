#ifndef M8_COLUMN_HPP_
#define M8_COLUMN_HPP_

#include <cstdint>

namespace m8
{
    class Column
    {
    private:
        std::uint8_t value_;

    public:
        /// <summary>
        ///  Default constructor
        /// </summary>
        Column() {}

        /// <summary>
        ///  Constructor
        /// </summary>
        /// <param name="value">Value of the column</param>
        Column(std::uint8_t value)
            : value_(value)
        {}

        /// <summary>
        ///  Number of column on a chessboard.
        /// </summary>
        static const std::uint8_t NumColumnOnBoard = 8;

        /// <summary>
        ///  Return the value of the column.
        /// </summary>
        inline std::uint8_t value() const { return value_; }
        
        /// <summary>
        ///  Indicate if the current column is on the board.
        /// </summary>
        /// <returns>True if the column is valid.</returns>
        inline bool IsOnBoard() const { return value_ < NumColumnOnBoard; }

        /// <summary>
        ///  Modify the column to the next column. This can be used to iterate on all 
        ///  columns. The column must be valid before the move.
        /// </summary>
        /// <returns>A reference to the current object</returns>
        inline Column& MoveNext() { assert(IsOnBoard()); ++value_; return *this; }
        
        /// <summary>
        ///  Column a
        /// </summary>
        inline static Column A() { return Column(0); };

        /// <summary>
        ///  Column b
        /// </summary>
        inline static Column B() { return Column(1); };

        /// <summary>
        ///  Column c
        /// </summary>
        inline static Column C() { return Column(2); };

        /// <summary>
        ///  Column D
        /// </summary>
        inline static Column D() { return Column(3); };

        /// <summary>
        ///  Column e
        /// </summary>
        inline static Column E() { return Column(4); };

        /// <summary>
        ///  Column f
        /// </summary>
        inline static Column F() { return Column(5); };

        /// <summary>
        ///  Column g
        /// </summary>
        inline static Column G() { return Column(6); };

        /// <summary>
        ///  Column h
        /// </summary>
        inline static Column H() { return Column(7); };

        /// <summary>
        ///  Invalid column
        /// </summary>
        inline static Column Invalid() { return Column(255); };

        inline Column& operator += (std::int32_t rhs) { value_ += rhs; return *this; };

        friend bool operator==(Column lhs, Column rhs);
        friend bool operator<(Column lhs, Column rhs);
        friend bool operator>(Column lhs, Column rhs);
        friend bool operator<=(Column lhs, Column rhs);
        friend bool operator>=(Column lhs, Column rhs);
    };

    inline bool operator==(Column lhs, Column rhs) { return lhs.value_ == rhs.value_; }
    inline bool operator!=(Column lhs, Column rhs) { return !(lhs == rhs); }
    inline bool operator<(Column lhs, Column rhs) { return lhs.value_ < rhs.value_; }
    inline bool operator>(Column lhs, Column rhs) { return lhs.value_ > rhs.value_; }
    inline bool operator<=(Column lhs, Column rhs) { return lhs.value_ <= rhs.value_; }
    inline bool operator>=(Column lhs, Column rhs) { return lhs.value_ >= rhs.value_; }
}

#endif // M8_COLUMN_HPP_