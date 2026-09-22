cmake_minimum_required(VERSION 3.24)
project(MeshOp3D-core)

### HLMP target
set(MOP_CORE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/mop/*.h")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/mop/*.cpp")

add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES})
add_library(mop::core ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${MOP_CORE_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC vclib::core)

target_sources(${PROJECT_NAME} PRIVATE ${HEADERS} ${SOURCES})
