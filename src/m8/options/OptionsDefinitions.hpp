/// @file   OptionsDefinitions.hpp
/// @author Mathieu Pagé
/// @date   January 2017
/// @brief  Contains the definitions of m8 options.

#ifndef M8_OPTIONS_DEFINITIONS_HPP_
#define M8_OPTIONS_DEFINITIONS_HPP_

#define M8_OPTIONS_DEFINITIONS(macro)                                       \
    macro("perft-threads",                                                  \
          "Numbers of parallel threads to use for the perft command.",      \
          perft.threads,                                                    \
          int)                                                              \
    macro("perft-min-works-items",                                          \
          "Minimum number of work item the perft load is separated in.",    \
          perft.min_works_items,                                            \
          int)                                                              \
    macro("max-log-severity",                                               \
          "Define the maximum log severity level (fatal, error, warning, info, output, input, debug, trace).", \
          max_log_severity,                                                 \
          m8::severity_level)                                               \
                                   
#endif // M8_OPTIONS_DEFINITIONS_HPP_

