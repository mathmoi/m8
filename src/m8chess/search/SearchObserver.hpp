/// @file   SearchObserver.hpp
/// @author Mathieu Pagé
/// @date   January 2020
/// @brief  Contains a virtual base class (interface) for classes that can observe a 
///         search and receive "events".

#ifndef M8_SEARCH_SEARCH_OBSERVER_HPP_
#define M8_SEARCH_SEARCH_OBSERVER_HPP_

#include "../Types.hpp"
#include "../Move.hpp"
#include "PV.hpp"

namespace m8::search {

	/// Interface for classes that can observe the search
	class SearchObserver
	{
	public:
		/// Method called when the search starts.
		inline virtual void OnBeginSearch() {};

		/// Method called when a new best move is found at the root.
		inline virtual void OnNewBestMove(const PV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes) {};
		inline virtual void OnNewBestMove(const std::vector<std::string>& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes) {};

		/// Method called when an iteration is completed.
		inline virtual void OnIterationCompleted(const PV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes) {};
		inline virtual void OnIterationCompleted(const std::vector<std::string>& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes) {};

		/// Method when the search is completed.
		inline virtual void OnSearchCompleted(const PV& pv, double time) {};
		inline virtual void OnSearchCompleted(const std::vector<std::string>& pv, double time) {};
	};
}

#endif // M8_SEARCH_SEARCH_OBSERVER_HPP_
