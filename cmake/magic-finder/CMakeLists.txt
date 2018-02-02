ucm_set_runtime(STATIC)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  FIND_PACKAGE ( Threads REQUIRED )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g -Wall -pedantic -std=c++11")
endif()

# We add boost
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})  
     
ucm_add_dirs(../../src/magic-finder
             TO sources_files
             RECURSIVE
             FILTER_POP 3)
ucm_add_files(../../src/m8common/Bb.cpp
              ../../src/m8common/Bb.hpp
              TO sources_files
              FILTER_POP 3)

ucm_add_target(NAME m8-magic-finder
               TYPE EXECUTABLE
               SOURCES ${sources_files})