cmake_minimum_required(VERSION 3.24)
project(MeshOp3D-core)

### HLMP target
set(M3D_CORE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/m3d/*.h")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/m3d/*.cpp")

add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES})
add_library(m3d::core ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${M3D_CORE_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC vclib::core)

target_sources(${PROJECT_NAME} PRIVATE ${HEADERS} ${SOURCES})
