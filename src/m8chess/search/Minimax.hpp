/// @file   Minimax.hpp
/// @author Mathieu Pagé
/// @date   Novembre 2019
/// @brief  Contains the Minimax class. This class implements the naive minimax algorithm.

#ifndef M8_MINIMAX_HPP_
#define M8_MINIMAX_HPP_

#include "../Board.hpp"
#include "SearchResult.hpp"

namespace m8 { namespace search {

	/// Implements the minimax search algorithm
	class Minimax
	{
	public:

		/// Constructor
		inline Minimax(const Board& board)
			: board_(board), continue_(true)
		{};

		/// Start a search on a given position.
		SearchResult Search(std::uint32_t depth);

		/// Stop the search
		void Stop();

	private:
		Board board_;
		bool continue_;

		eval::EvalType SearchInternal(std::uint32_t depth);

	};
}}

#endif // M8_MINIMAX_HPP_