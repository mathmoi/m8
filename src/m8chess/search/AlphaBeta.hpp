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
#include "../Types.hpp"
#include "../../m8common/ThreadPool.hpp"

#include "SearchResult.hpp"
#include "SearchObserver.hpp"
#include "PV.hpp"

namespace m8 {
	namespace search
	{
		/// Manage the seach for the engine.
		class AlphaBeta
		{
		public:
			/// Constructor.
			AlphaBeta(const Board& board,
				      SearchObserver* observer);

			// TODO : Remove depth from the Search method and replace by proper time management
			/// Start a search on a given position.
			SearchResult Search(DepthType depth);

			/// Stop the search
			void Stop();

		private:
			Board board_;
			bool continue_;
			Move best_move_; // TODO : Remove this hack by returning a proper PV.
			NodeCounterType nodes_counter_; // TODO : Replace this with a proper stats counter struct.

			SearchObserver* observer_;

			template<bool root, bool qsearch>
			EvalType Search(EvalType alpha, EvalType beta, DepthType depth, PV& pv);
		};

	}
}

#endif // M8_ALPHA_BETA_HPP_