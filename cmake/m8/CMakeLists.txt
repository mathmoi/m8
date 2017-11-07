# enable c++11 for gcc
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")
endif()

# We set maximum warnings on GCC
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
endif()

# We add boost
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})  

file(GLOB_RECURSE common_header_files
     RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
     ${CMAKE_CURRENT_SOURCE_DIR}/../../src/m8common/*.hpp)

add_executable(m8
           ../../src/m8/m8.cpp
           ../../src/m8/Engine.cpp
           ../../src/m8/Engine.hpp
           ../../src/m8/ShellIntrf.cpp
           ../../src/m8/ShellIntrf.hpp
           ../../src/m8/m8Intrf.cpp
           ../../src/m8/m8Intrf.hpp
		   ../../src/m8chess/Board.hpp
           ../../src/m8chess/Board.cpp
           ../../src/m8common/Bb.cpp
           ${common_header_files})
