/// @file   SearchCallbacks.hpp
/// @author Mathieu Pagé
/// @date   January 2020
/// @brief  Contains a struct used to pass search callbacks.

#ifndef M8_SEARCH_SEARCH_CALLBACKS_HPP_
#define M8_SEARCH_SEARCH_CALLBACKS_HPP_

#include <functional>

namespace m8::search {

	// TODO : I think this class will not be used.

	/// Struct containing callbacks used by the search to give feedback to the user about 
	/// the current search.
	struct SearchCallbacks
	{
	public:

		typedef std::function<void()> StartOfSearchCallback;
		typedef std::function<void(const SearchResult&)> SearchStartCallback;



		/// Callback called when the search starts
		StartOfSearchCallback start_of_search_callback_;

		/// Callback used when the search is completed
		SearchStartCallback search_completed_callback_;

		

	};

}

#endif // M8_SEARCH_SEARCH_CALLBACKS_HPP_
