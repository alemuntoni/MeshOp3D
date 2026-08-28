cmake_minimum_required(VERSION 3.24)

project(HighLevelMeshProcessing)

set(CMAKE_CXX_STANDARD 20)

set(VCLIB_BUILD_MODULE_EXTERNAL ON)
set(VCLIB_BUILD_MODULE_RENDER ON)

# not needed dependencies
set(VCLIB_ALLOW_DOWNLOAD_GLFW OFF)
set(VCLIB_ALLOW_DOWNLOAD_IMGUI OFF)

add_subdirectory(vclib)

set(HLMP_ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")

add_subdirectory(core)
add_subdirectory(application)
