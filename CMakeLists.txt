cmake_minimum_required(VERSION 3.24)

project(MeshOp3D VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)

set(VCLIB_BUILD_MODULE_EXTERNAL ON)
set(VCLIB_BUILD_MODULE_RENDER ON)

# not needed dependencies
set(VCLIB_ALLOW_DOWNLOAD_GLFW OFF)
set(VCLIB_ALLOW_DOWNLOAD_IMGUI OFF)

add_subdirectory(vclib EXCLUDE_FROM_ALL)


add_subdirectory(core)
add_subdirectory(application)


