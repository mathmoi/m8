add_executable(m8-magic-finder magic-finder.cpp)
target_link_libraries(m8-magic-finder m8chess m8common)
add_dependencies(m8-magic-finder m8chess m8common)