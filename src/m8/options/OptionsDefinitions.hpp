/// @file   OptionsDefinitions.hpp
/// @author Mathieu Pag�
/// @date   January 2017
/// @brief  Contains the definitions of m8 options.

#ifndef M8_OPTIONS_DEFINITIONS_HPP_
#define M8_OPTIONS_DEFINITIONS_HPP_

#define M8_OPTIONS_DEFINITIONS(macro)                                       \
    macro("perft-threads",                                                  \
          "Numbers of parallel threads to use for the perft command.",      \
          int,                                                              \
          perft().threads,                                                  \
          perft().set_threads)                                              \
    macro("perft-min-works-items",                                          \
          "Minimum number of work item the perft load is separated in.",    \
          int,                                                              \
          perft().min_works_items,                                          \
          perft().set_min_works_items)                                      \
    macro("max-log-severity",                                               \
          "Define the maximum log severity level (fatal, error, warning, info, output, input, debug, trace).", \
          m8::severity_level,                                               \
          max_log_severity,                                                 \
          set_max_log_severity)                                             \

                                   
#endif // M8_OPTIONS_DEFINITIONS_HPP_

