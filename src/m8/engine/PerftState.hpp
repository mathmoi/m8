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

namespace m8::engine {

	/// Class controlling the engine behavior in the waiting state.
	class PerftState : public EngineState
	{
	public:

		/// Constructor from a previous state
		PerftState(Engine* engine,
			       int depth)
			: EngineState(engine),
			  perft_(depth,
				     engine->board_,
				     [this](Move move, std::uint64_t count) { this->engine_->callbacks_.partial_perft_result_callback(m8::RenderSAN(move, this->engine_->board_), count); },
				     std::bind(&PerftState::HandleResult, this, std::placeholders::_1, std::placeholders::_2))
		{}

		/// Return the name of the state
        inline virtual const std::string state_name() const { return "PerftState"; }

		/// Method that is run before a state is replaced by a new state
		virtual void BeginState();

		/// Stops the current operation.
		virtual void Stop();

	private:
		m8::Perft perft_;

		inline void partial_result_handler(Move move, std::uint64_t count) {};

		void HandleResult(std::uint64_t count, double seconds);
	};
}

#endif // M8_ENGINE_PERFT_STATE_HPP_