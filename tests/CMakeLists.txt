# Get Catch2
include(FetchContent)
FetchContent_Declare(Catch2
                     GIT_REPOSITORY https://github.com/catchorg/Catch2.git
                     GIT_TAG v3.4.0)
FetchContent_MakeAvailable(Catch2)

# We find boost
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED COMPONENTS program_options filesystem log)
include_directories(${Boost_INCLUDE_DIRS})

# If we are usging g++ we find Threads
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    find_package(Threads REQUIRED)
endif()

# Create the target
file(GLOB_RECURSE M8_TESTS_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(m8-tests ${M8_TESTS_SOURCES})

# Link libraries
target_link_libraries(m8-tests PRIVATE Catch2::Catch2
                                       m8chess
                                       m8common
                                       Boost::program_options
                                       Boost::filesystem
                                       Boost::log
                                       ${CMAKE_THREAD_LIBS_INIT})
add_dependencies(m8-tests m8chess m8common)

# Configure tests
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(m8-tests)