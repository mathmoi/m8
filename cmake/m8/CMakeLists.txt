ucm_set_runtime(STATIC)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    FIND_PACKAGE ( Threads REQUIRED )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-unknown-pragmas -pedantic")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_SCL_SECURE_NO_WARNINGS")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DM8_MAX_LOG_SEVERITY=M8_SEVERITY_TRACE")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DM8_MAX_LOG_SEVERITY=M8_SEVERITY_INPUT")

# We add boost
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)    
#set(Boost_USE_STATIC_RUNTIME     ON)
find_package(Boost REQUIRED COMPONENTS program_options
                                       log
                                       filesystem)
include_directories(${Boost_INCLUDE_DIRS})
add_compile_definitions(BOOST_BIND_GLOBAL_PLACEHOLDERS)
     
ucm_add_dirs(../../src/m8common
             ../../src/m8chess
             ../../src/m8
             TO sources_files
             RECURSIVE
             FILTER_POP 3)

ucm_add_target(NAME m8 
               TYPE EXECUTABLE
               SOURCES ${sources_files})

configure_file(../../resources/m8.json
               ./m8.json
               COPYONLY)

target_link_libraries(m8 ${Boost_LIBRARIES}
                         ${CMAKE_THREAD_LIBS_INIT})

set_property(TARGET m8 PROPERTY CXX_STANDARD 20)

find_package(Git)
add_custom_target(version
  ${CMAKE_COMMAND} -D SRC=${CMAKE_SOURCE_DIR}/../src/m8/version.hpp.in
                   -D DST=${CMAKE_BINARY_DIR}/../src/m8/version.hpp
                   -D GIT_EXECUTABLE=${GIT_EXECUTABLE}
                   -P ${CMAKE_SOURCE_DIR}/GenerateVersionHeader.cmake
  )
add_dependencies(m8 version)
