cmake_minimum_required(VERSION 2.8...3.24)

include(cotire.cmake)
include(ucm.cmake)

project(m8)

add_subdirectory(tests)
add_subdirectory(m8)
add_subdirectory(magic-finder)