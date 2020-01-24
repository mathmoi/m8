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

#include "SearchResult.hpp"

namespace m8 {
	namespace search
	{
		/// Manage the seach for the engine.
		class AlphaBeta
		{
		public:

			/// Constructor.
			AlphaBeta(const Board& board);

			// TODO : Remove depth from the Search method and replace by proper time management
			/// Start a search on a given position.
			SearchResult Search(std::uint32_t depth);

			/// Stop the search
			void Stop();

		private:
			Board board_;
			bool continue_;
			Move best_move_; // TODO : Remove this hack by returning a proper PV.

			template<bool root, bool qsearch>
			eval::EvalType Search(eval::EvalType alpha, eval::EvalType beta, std::int_fast16_t depth);
		};

	}
}

#endif // M8_ALPHA_BETA_HPP_