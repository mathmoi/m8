/// @file   AlphaBeta.hpp
/// @author Mathieu Pagé
/// @date   Decembre 2018
/// @brief  Contains the AlphaBeta class. This class is responsible for the search
///         algorithm of the chess engine.

#include <chrono>

#include "../eval/Eval.hpp"

#include "AlphaBeta.hpp"

namespace m8 { namespace search {

	AlphaBeta::AlphaBeta(const Board& board)
		: board_(board)
	{}

	void AlphaBeta::ExecuteMinimax(std::uint32_t depth)
	{
		for (auto x = 0; x < 15; ++x)
		{
			iteration_finished_callback_(x, std::chrono::duration<double>(x), x);

			using namespace std::chrono_literals;
			std::this_thread::sleep_for(1s);
		}

		auto eval = eval::Evaluate(board_);
		search_finished_callback_(eval);
	}
}}