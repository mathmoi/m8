/// @file   PV.hpp
/// @author Mathieu Pagé
/// @date   January 2020
/// @brief  Contains the PV class used to gather the principal variation during a search.

#ifndef M8_PV_HPP_
#define M8_PV_HPP_

#include <array>

#include "../Move.hpp"

namespace m8::search
{
	/// Maximum size of a PV.
	const std::size_t MAX_PV_SIZE = 512;

	/// Represent a principal variation 
	class PV 
	{
	public:

		/// Constructor.
		PV()
			: count_(0)
		{}

		/// return the number of elements actually in the PV.
		inline std::size_t count() const { return count_; }

		/// Replace the PV with a new one based a single move and another PV.
		inline void Replace(Move move, const PV& other)
		{
			assert(count_ + other.count_ + 1 < MAX_PV_SIZE);

			moves_[0] = move;
			for (size_t x = 0; x < other.count_; ++x)
			{
				moves_[x + 1] = other.moves_[x];
			}
			count_ = other.count_ + 1;
		}

		/// Allow access to the moves composing the PV.
		inline Move operator[](size_t pos) const 
		{
			assert(pos < count_);
			return moves_[pos];
		}

		/// Returns the fist move of the PV.
		inline Move first() const
		{
			assert(0 < count_);
			return moves_[0];
		}

		/// Remove all moves from the PV.
		inline void Clear()
		{
			count_ = 0;
		}
		
	private:
		std::array<Move, MAX_PV_SIZE> moves_;
		std::size_t count_;
		
	};
}

#endif // M8_PV_HPP_