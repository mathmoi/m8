ucm_set_runtime(STATIC)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  FIND_PACKAGE ( Threads REQUIRED )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g -Wall -pedantic -std=c++11")
endif()

# We add boost
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)    
set(Boost_USE_STATIC_RUNTIME     ON)
find_package(Boost REQUIRED COMPONENTS program_options
                                       filesystem
                                       log)
include_directories(${Boost_INCLUDE_DIRS})
     
ucm_add_dirs(../../src/m8common
             ../../src/m8chess
             ../../src/tests
             ../../src/m8/options
             TO sources_files
             RECURSIVE
             FILTER_POP 3)

ucm_add_target(NAME m8_tests 
               TYPE EXECUTABLE
               SOURCES ${sources_files})

target_link_libraries(m8_tests ${Boost_LIBRARIES}
                               ${CMAKE_THREAD_LIBS_INIT})