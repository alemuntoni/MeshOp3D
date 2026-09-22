# MeshOp3D
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.24)
project(MeshOp3D-app)

if (NOT TARGET vclib-3rd-qt)
    message(FATAL_ERROR "Qt6 not found. Please set Qt6_DIR to the Qt6Config.cmake file.")
endif()

find_package(Qt6 REQUIRED COMPONENTS Network)

# Load supported file formats (Single Source of Truth for OS file associations).
# Generates MOP_DESKTOP_MIME_TYPES, MOP_MIME_XML_ENTRIES, MOP_PLIST_EXTENSIONS.
include(cmake/formats.cmake)

#automoc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "*.h")
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")
file(GLOB_RECURSE FORMS CONFIGURE_DEPENDS "*.ui")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Forms Files" FILES ${FORMS})

add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${HEADERS} ${SOURCES} ${FORMS} assets.qrc)
target_link_libraries(${PROJECT_NAME} PRIVATE vclib mop::core Qt6::Network)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin COMPONENT Runtime
    BUNDLE DESTINATION . COMPONENT Runtime
)

if(WIN32)
    install(CODE "execute_process(COMMAND windeployqt \"\${CMAKE_INSTALL_PREFIX}/bin/${PROJECT_NAME}.exe\")" COMPONENT Runtime)
elseif(APPLE)
    install(CODE "execute_process(COMMAND macdeployqt \"\${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}.app\")" COMPONENT Runtime)
elseif(UNIX AND NOT APPLE)
    configure_file(assets/application.desktop.in "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.desktop" @ONLY)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.desktop" DESTINATION share/applications COMPONENT Runtime)
    install(FILES assets/mop3d-icon.png DESTINATION share/icons/hicolor/512x512/apps COMPONENT Runtime)
    configure_file(assets/mop3d-mimetypes.xml.in "${CMAKE_CURRENT_BINARY_DIR}/mop3d-mimetypes.xml" @ONLY)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/mop3d-mimetypes.xml" DESTINATION share/mime/packages COMPONENT Runtime)
    
    # Determine architecture for linuxdeploy
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86_64|amd64|AMD64)")
        set(LINUXDEPLOY_ARCH "x86_64")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "(aarch64|arm64|ARM64)")
        set(LINUXDEPLOY_ARCH "aarch64")
    else()
        message(WARNING "Unsupported architecture for AppImage: ${CMAKE_SYSTEM_PROCESSOR}, falling back to x86_64")
        set(LINUXDEPLOY_ARCH "x86_64")
    endif()

    find_program(QMAKE_EXECUTABLE NAMES qmake6 qmake HINTS "${Qt6_DIR}/bin" "${Qt6_DIR}/../../../bin" "${Qt6_DIR}/../../bin" REQUIRED)
    
    # Configure the CPack pre-build script
    configure_file(cmake/linuxdeploy_cpack.cmake.in "${CMAKE_BINARY_DIR}/linuxdeploy_cpack.cmake" @ONLY)
    
    set(CPACK_PRE_BUILD_SCRIPTS "${CMAKE_BINARY_DIR}/linuxdeploy_cpack.cmake")
endif()

# --- CPack Configuration ---
string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" SYS_ARCH)
if(SYS_ARCH STREQUAL "amd64")
    set(SYS_ARCH "x86_64")
endif()
set(CPACK_SYSTEM_NAME "${CMAKE_SYSTEM_NAME}-${SYS_ARCH}")

set(CPACK_PACKAGE_NAME "MeshOp3D")
set(CPACK_PACKAGE_VENDOR "VCLab")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "MeshOp3D is a mesh processing application developed by the Visual Computing Lab, ISTI - CNR.")
set(CPACK_PACKAGE_VERSION ${CMAKE_PROJECT_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${CMAKE_PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${CMAKE_PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${CMAKE_PROJECT_VERSION_PATCH})
if(NOT APPLE)
    set(CPACK_PACKAGE_ICON "mop3d-icon.png")
endif()
if(UNIX AND NOT APPLE)
    set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")
else()
    set(CPACK_PACKAGING_INSTALL_PREFIX "/")
endif()

# Package only the "Runtime" component to ignore vclib headers
set(CPACK_COMPONENTS_ALL "Runtime")

if(WIN32)
    set(CPACK_GENERATOR "ZIP")
elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
else()
    set(CPACK_GENERATOR "DEB;TGZ;AppImage")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt6core6, libqt6gui6, libqt6widgets6, libqt6network6, libgmp10, libmpfr6, libtbb12, libembree4")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Alessandro Muntoni")
endif()

include(CPack)
