# Create the target
file(GLOB_RECURSE M8_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(m8 ${M8_SOURCES})

# compile statically
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")

# Add link libraries
target_link_libraries(m8 m8common
                         m8chess
                         Boost::program_options
                         Boost::filesystem
                         Boost::log
                         ${CMAKE_THREAD_LIBS_INIT})
add_dependencies(m8 m8chess m8common)

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