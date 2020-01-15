/// @file	ThinkingState.hpp
/// @author Mathieu Pagé
/// @date	January 2020
/// @brief	Contains the ThinkingState class. Controlling the engine behavior when it's 
///         searching for a move to play

#ifndef M8_ENGINE_THINKING_STATE_HPP_
#define M8_ENGINE_THINKING_STATE_HPP_

#include <thread>

#include "../../m8chess/search/Search.hpp"

#include "EngineState.hpp"

namespace m8::engine {

	/// Class controlling the engine behavior in the waiting state.
	class ThinkingState : public EngineState
	{
	public:

		/// Constructor from a previous state
		ThinkingState(EngineState* source);

		/// Method that is run when this state becomes the new current state
		virtual void BeginState();

		/// Set the engine to play neither color.
		virtual void Force();
				
	private:
		search::Search search_;
		bool searching_;

		std::mutex state_mutex_;

		void HandleSearchResult(search::SearchResult result);
		void SwitchToWaitingState();
		void SwitchToObservingState();
		void SendResultToUser(const std::string& move, m8::search::SearchResult& result);
		void MakeEngineMove(m8::search::SearchResult& result);
		bool StopSearch();
	};
}

#endif // M8_ENGINE_THINKING_STATE_HPP_