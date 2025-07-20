cmake_minimum_required(VERSION 3.24)
project(HLMP-core)

### HLMP target
set(HLMP_CORE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/hlmp/*.h")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/hlmp/*.cpp")

add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES})
add_library(hlmp::core ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${HLMP_CORE_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC vclib::core)

target_sources(${PROJECT_NAME} PRIVATE ${HEADERS} ${SOURCES})
