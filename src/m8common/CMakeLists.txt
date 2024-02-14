file(GLOB_RECURSE M8_COMMON_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE M8_COMMON_HEADERS "${CMAKE_SOURCE_DIR}/include/m8common")

add_library(m8common STATIC ${M8_COMMON_SOURCES} ${M8_COMMON_HEADERS})

target_include_directories(m8common PUBLIC "${CMAKE_SOURCE_DIR}/include")