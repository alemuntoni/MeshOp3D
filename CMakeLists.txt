cmake_minimum_required(VERSION 3.24)

project(HighLevelMeshProcessing)

set(CMAKE_CXX_STANDARD 20)

set(VCLIB_BUILD_MODULE_EXTERNAL ON)
set(VCLIB_BUILD_MODULE_RENDER ON)
set(VCLIB_BUILD_EXAMPLES OFF)
set(VCLIB_BUILD_TESTS OFF)

add_subdirectory(vclib)

set(HLMP_ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")

add_subdirectory(core)
add_subdirectory(gui)
