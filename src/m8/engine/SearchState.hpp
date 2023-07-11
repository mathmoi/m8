/// @file	SearchState.hpp
/// @author Mathieu Pagé
/// @date	January 2020
/// @brief	Contains the SearchState class. Controlling the engine behavior when it's 
///         searching for a move to play

#ifndef M8_ENGINE_THINKING_STATE_HPP_
#define M8_ENGINE_THINKING_STATE_HPP_

#include <thread>

#include "../../m8chess/search/Search.hpp"
#include "../../m8chess/Types.hpp"

#include "EngineState.hpp"

namespace m8::engine {

	/// Class controlling the engine behavior in the thinking state.
	class SearchState : public EngineState, public search::ISearchObserver<search::PV> // TODO : This state should be named SearchingState/PonderingState
	{
	public:

		/// Constructor from a previous state
		SearchState(Engine* engine);

		/// Return the name of the state
        inline virtual const std::string state_name() const { return "SearchState"; }

		/// Method that is run when this state becomes the new current state
		virtual void BeginState();

		/// Set the engine to play neither color.
		virtual void Force();

		/// Method called when a new best move is found at the root.
		void OnNewBestMove(const search::PV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes);

		/// Method called when an iteration is completed.
		void OnIterationCompleted(const search::PV& pv, EvalType eval, DepthType depth, double time, NodeCounterType nodes);

		/// Method when the search is completed.
		void OnSearchCompleted(const search::PV& pv, double time);
				
	private:
		void SwitchToWaitingState();
		void SwitchToObservingState();
		bool StopSearch();

		std::vector<std::string> RenderPVMoves(const search::PV& pv);
	};
}

#endif // M8_ENGINE_THINKING_STATE_HPP_