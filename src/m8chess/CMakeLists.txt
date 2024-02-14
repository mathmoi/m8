file(GLOB_RECURSE M8_CHESS_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE M8_CHESS_HEADERS "${CMAKE_SOURCE_DIR}/include/m8chess")

add_library(m8chess STATIC ${M8_CHESS_SOURCES} ${M8_CHESS_HEADERS})

target_include_directories(m8chess PUBLIC "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(m8chess PRIVATE m8common)
add_dependencies(m8chess m8common)