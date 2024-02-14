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
file(GLOB_RECURSE M8_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(m8 ${M8_SOURCES})

# Add link libraries
target_link_libraries(m8 m8chess
                         m8common
                         Boost::program_options
                         Boost::filesystem
                         Boost::log
                         ${CMAKE_THREAD_LIBS_INIT})
add_dependencies(m8 m8chess m8common)

# Add definitions
target_compile_definitions(m8 PRIVATE
    $<$<CONFIG:Debug>:M8_MAX_LOG_SEVERITY=M8_SEVERITY_TRACE>
    $<$<CONFIG:RelWithDebInfo>:M8_MAX_LOG_SEVERITY=M8_SEVERITY_TRACE>
    $<$<CONFIG:Release>:M8_MAX_LOG_SEVERITY=M8_SEVERITY_INPUT>    
    $<$<CONFIG:MinSizeRel>:M8_MAX_LOG_SEVERITY=M8_SEVERITY_INPUT>)

# Generating the version.hpp file
find_package(Git)
add_custom_target(version
  ${CMAKE_COMMAND} -D SRC=${CMAKE_CURRENT_SOURCE_DIR}/version.hpp.in
                   -D DST=${CMAKE_CURRENT_SOURCE_DIR}/version.hpp
                   -D GIT_EXECUTABLE=${GIT_EXECUTABLE}
                   -P ${CMAKE_SOURCE_DIR}/cmake/GenerateVersionHeader.cmake)
add_dependencies(m8 version)

# Add the config files
configure_file(${CMAKE_SOURCE_DIR}/resources/m8.json
               ${CMAKE_CURRENT_BINARY_DIR}/m8.json
               COPYONLY)