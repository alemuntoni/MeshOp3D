cmake_minimum_required(VERSION 3.24)

project(MeshProcessing)

set(CMAKE_CXX_STANDARD 20)

set(VCLIB_BUILD_MODULE_EXTERNAL ON)
set(VCLIB_BUILD_MODULE_PROCESSING ON)
set(VCLIB_BUILD_MODULE_RENDER ON)
set(VCLIB_BUILD_EXAMPLES OFF)
set(VCLIB_BUILD_TESTS OFF)

add_subdirectory(vclib)

if (NOT TARGET vclib-3rd-qt)
    message(FATAL_ERROR "Qt6 not found. Please set Qt6_DIR to the Qt6Config.cmake file.")
endif()

add_subdirectory(mesh_processing)
