/// @file	EngineCallbacks.hpp
/// @author Mathieu Pagé
/// @date	January 2020
/// @brief	Contains the EngineCallbacks struct, that contains a list of callbacks 
///         function used by the Engine class to notify it's creator of events.

#ifndef M8_ENGINE_ENGINE_CALLBACKS_HPP_
#define M8_ENGINE_ENGINE_CALLBACKS_HPP_

#include "../../m8chess/Types.hpp"

namespace m8::engine {

	// TODO : Could we replace this or part of this witha SearchObserver base class (interface) that could be passed around?

	/// Contains a list of callbacks used by the engine class and the different
	/// engine states classes.
	struct EngineCallbacks
	{
		typedef std::function<void(std::string, std::uint64_t)> PartialPerftResultCallback;
		typedef std::function<void(std::uint64_t, double)> PerftResultCallback;
		typedef std::function<void(std::string, double)> SearchResultCallback;
		typedef std::function<void()> SearchStartCallback;
		typedef std::function<void(std::string, EvalType, DepthType depth, double, NodeCounterType)> NewBestMoveCallback;
		typedef std::function<void(std::string, EvalType, DepthType depth, double, NodeCounterType)> IterationCompletedCallback;

		PartialPerftResultCallback partial_perft_result_callback;
		PerftResultCallback		   perft_result_callback;
		SearchResultCallback	   search_result_callback;
		SearchStartCallback        search_start_callback;
		NewBestMoveCallback		   new_best_move_callback;
		IterationCompletedCallback iteration_completed_callback;
	};
}

#endif // M8_ENGINE_ENGINE_CALLBACKS_HPP_