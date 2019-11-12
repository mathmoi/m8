/// @file   AlphaBeta.hpp
/// @author Mathieu Pagé
/// @date   Decembre 2018
/// @brief  Contains the AlphaBeta class. This class is responsible for the search
///         algorithm of the chess engine.

#ifndef M8_ALPHA_BETA_HPP_
#define M8_ALPHA_BETA_HPP_

#include <vector>
#include <chrono>

#include "../Board.hpp"
#include "../../m8common/ThreadPool.hpp"

namespace m8 {
	namespace search
	{
		/// Manage the seach for the engine.
		class AlphaBeta
		{
		public:

			/// Constructor
			AlphaBeta(const Board& board);

			/// Start a search on a given position.
			void ExecuteMinimax(std::uint32_t depth);

		private:
			Board board_;

			/// Callback function for the end of the search.
			///
			/// The first parameter is the evaluation.
			std::function<void(std::uint32_t)> search_finished_callback_;

			/// Callback function for the end of an iteration.
			///
			/// Parameter 1 : depth
			/// Parameter 2 : Time since the begining of the search
			/// Parameter 3 : Evaluation
			std::function<void(std::int32_t,
				               std::chrono::duration<double>,
				               std::uint32_t)> iteration_finished_callback_;

		};

	}
}

#endif // M8_ALPHA_BETA_HPP_