/// @file   OptionsDefinitions.hpp
/// @author Mathieu Pagé
/// @date   January 2017
/// @brief  Contains the definitions of m8 options.

#ifndef M8_OPTIONS_DEFINITIONS_HPP_
#define M8_OPTIONS_DEFINITIONS_HPP_

#define M8_OPTIONS_DEFINITIONS(macro)                                    \
    macro(parallel_perft_threads,                                        \
          "Numbers of parallel threads to use for the perft command.",   \
          int,                                                           \
          8)                                                             \
    macro(parallel_perft_min_work_items,                                 \
          "Minimum number of work item the perft load is separated in.", \
          int,                                                           \
          100)                                                           \
    macro(ini,                                                           \
          "Generate a m8.ini file if one can not be found.",             \
          bool,                                                          \
          false)                                                         \
    macro(max_log_severity,                                              \
          "Define the maximum log severity level (0: Fatal, 1: Error, 2: Warning, 3: Info, 4: Output, 5: Input, 6: Debug, 7: Trace).", \
          int,                                                           \
          0)                                                             \
                                   
#endif // M8_OPTIONS_DEFINITIONS_HPP_

