/// @file   SearchObserver.hpp
/// @author Mathieu Pagé
/// @date   January 2020
/// @brief  Contains a virtual base class (interface) for classes that can observe a 
///         search and receive "events".

#ifndef M8_SEARCH_SEARCH_OBSERVER_HPP_
#define M8_SEARCH_SEARCH_OBSERVER_HPP_

#include "../Types.hpp"
#include "../Move.hpp"

namespace m8::search {

	/// Interface for classes that can observe the search
	class SearchObserver
	{
	public:
		/// Method called when the search starts.
		inline virtual void OnBeginSearch() {};

		/// Method called when a new best move is found at the root.
		inline virtual void OnNewBestMove(Move move, EvalType eval, DepthType depth, double time, NodeCounterType nodes) {};
		inline virtual void OnNewBestMove(std::string move, EvalType eval, DepthType depth, double time, NodeCounterType nodes) {};

		/// Method called when an iteration is completed.
		inline virtual void OnIterationCompleted(Move move, EvalType eval, DepthType depth, double time, NodeCounterType nodes) {};
		inline virtual void OnIterationCompleted(std::string move, EvalType eval, DepthType depth, double time, NodeCounterType nodes) {};

		/// Method when the search is completed.
		inline virtual void OnSearchCompleted(Move move, double time) {};
		inline virtual void OnSearchCompleted(std::string move, double time) {};
	};
}

#endif // M8_SEARCH_SEARCH_OBSERVER_HPP_
