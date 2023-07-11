/// @file	PerftState.hpp
/// @author Mathieu Pag�
/// @date	Decembre 2019
/// @brief	Contains the PerftState class. Controlling the engine behavior when it's 
///         executing a perft test.

#ifndef M8_ENGINE_PERFT_STATE_HPP_
#define M8_ENGINE_PERFT_STATE_HPP_

#include <functional>

#include "../../m8chess/Perft.hpp"
#include "../../m8chess/SAN.hpp"

#include "EngineState.hpp"
#include "IPerftObserver.hpp"

namespace m8::engine {

	/// Class controlling the engine behavior in the waiting state.
	class PerftState : public EngineState,
	                   public IPerftObserver
	{
	public:

		/// Constructor from a previous state
		PerftState(Engine* engine,
			       int depth,
				   IPerftObserver* observer)
			: EngineState(engine),
			  perft_(depth,
				     engine->board_,
					 this),
			  observer_(observer)

		{}

		/// Return the name of the state
        inline const std::string state_name() const { return "PerftState"; }

		/// Method that is run before a state is replaced by a new state
		void BeginState();

		/// Stops the current operation.
		void Stop();

		/// Method called everytime a partial perf result is ready
		/// 
		/// @param move  The move for which the result is available 
		/// @param count The number of nodes
		void OnPartialPerftResult(const std::string& move, std::uint64_t count);

		/// Method called at the end of the perft test
		/// 
		/// @param count The number of nodes
		/// @param time  The time used to complete the test
		void OnPerftCompleted(std::uint64_t count, double time);

	private:
		m8::Perft perft_;

		IPerftObserver* observer_;

		inline void partial_result_handler(Move move, std::uint64_t count) {};

		void HandleResult(std::uint64_t count, double seconds);
	};
}

#endif // M8_ENGINE_PERFT_STATE_HPP_