cmake_minimum_required(VERSION 3.27)

project(m8 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wall -Wextra -Wno-unused-parameter -Wno-unknown-pragmas -pedantic)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -pedantic)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W4 /D_SCL_SECURE_NO_WARNINGS /DBOOST_USE_WINAPI_VERSION=0x600)
endif()

# We find boost
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED COMPONENTS program_options filesystem log)
include_directories(${Boost_INCLUDE_DIRS})

# If we are usging g++ we find Threads
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    find_package(Threads REQUIRED)
endif()

add_subdirectory(src)
add_subdirectory(apps)
add_subdirectory(tests)
add_subdirectory(Doxygen)