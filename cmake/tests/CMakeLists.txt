# enable c++11 for gcc
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

# We set maximum warnings on GCC
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
endif()

file(GLOB_RECURSE common_header_files
     RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
     ${CMAKE_CURRENT_SOURCE_DIR}/../../src/m8common/*.hpp)

add_executable(m8_tests
	            ../../src/tests/tests_main.cpp
               ../../src/tests/Sq_tests.cpp
               ../../src/tests/Bb_tests.cpp
	            ../../src/tests/Piece_tests.cpp
               ../../src/tests/Board_tests.cpp
               ../../src/tests/Move_tests.cpp
               ../../src/tests/MoveGen_tests.cpp
               ../../src/tests/catch.hpp
               ../../src/m8chess/Board.cpp
               ../../src/m8common/Bb.cpp
               ../../src/m8chess/MoveGen.cpp
               ${common_header_files})
