cmake_minimum_required(VERSION 3.24)

project(HighLevelMeshProcessing)

set(CMAKE_CXX_STANDARD 20)

set(VCLIB_BUILD_MODULE_EXTERNAL ON)
set(VCLIB_BUILD_MODULE_RENDER ON)

add_subdirectory(vclib)

set(HLMP_ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")

add_subdirectory(core)
add_subdirectory(application)
