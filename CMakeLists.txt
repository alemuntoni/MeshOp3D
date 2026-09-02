cmake_minimum_required(VERSION 3.24)

project(HighLevelMeshProcessing VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)

set(VCLIB_BUILD_MODULE_EXTERNAL ON)
set(VCLIB_BUILD_MODULE_RENDER ON)

# not needed dependencies
set(VCLIB_ALLOW_DOWNLOAD_GLFW OFF)
set(VCLIB_ALLOW_DOWNLOAD_IMGUI OFF)

add_subdirectory(vclib)


add_subdirectory(core)
add_subdirectory(application)

# --- CPack Configuration ---
set(CPACK_PACKAGE_NAME "HighLevelMeshProcessing")
set(CPACK_PACKAGE_VENDOR "VCLab")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "High Level Mesh Processing Application")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

# Package only the "Runtime" component to ignore vclib headers
set(CPACK_COMPONENTS_ALL "Runtime")
set(CPACK_DEB_COMPONENT_INSTALL ON)
set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)

if(WIN32)
    set(CPACK_GENERATOR "ZIP")
elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
else()
    set(CPACK_GENERATOR "DEB;TGZ")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt6core6, libqt6gui6, libqt6widgets6, libgmp10, libmpfr6, libtbb12, libembree4")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Alessandro Muntoni")
endif()

include(CPack)
