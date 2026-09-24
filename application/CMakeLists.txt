# MeshOp3D
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.24)
project(MeshOp3D-app)

# Public-facing name used for the installed binary, bundle, and package.
# Change only this line on future renames.
set(MOP_APP_NAME "MeshOp3D")

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
target_link_libraries(${PROJECT_NAME} PRIVATE vclib mop::core mop::actions Qt6::Network)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}  # for generated mop_config.h
)

# Generate mop_config.h from the cmake-computed constants (e.g. MIME type list).
# Using configure_file avoids the semicolon-splitting issue that breaks
# target_compile_definitions when the value contains list-separator characters.
configure_file(
    cmake/mop_config.h.in
    "${CMAKE_CURRENT_BINARY_DIR}/mop_config.h"
    @ONLY
)

if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INSTALL_RPATH "@executable_path/../Frameworks"
    )
endif()

# The installed binary is always named ${MOP_APP_NAME} (/${MOP_APP_NAME}.exe/${MOP_APP_NAME}.app)
# regardless of the CMake project name (${PROJECT_NAME}).
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${MOP_APP_NAME}")

if(WIN32)
    # On Windows the executable lives directly in the install root,
    # not in a bin/ subdirectory (which is the Linux/macOS convention).
    install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION . COMPONENT Runtime)
else()
    install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin COMPONENT Runtime
        BUNDLE DESTINATION . COMPONENT Runtime
    )
endif()

if(WIN32)
    # Embed the .ico icon into the .exe (visible in Explorer/taskbar/Alt+Tab).
    # A .rc file is required by MSVC; CMake compiles it automatically via rc.exe.
    target_sources(${PROJECT_NAME} PRIVATE assets/mop3d-icon.rc)

    # Deploy Qt runtime DLLs next to the installed executable.
    find_program(WINDEPLOYQT_EXECUTABLE NAMES windeployqt
        HINTS "${Qt6_DIR}/bin" "${Qt6_DIR}/../../../bin" "${Qt6_DIR}/../../bin"
        REQUIRED)
    install(CODE "execute_process(COMMAND \"${WINDEPLOYQT_EXECUTABLE}\" \
        \"\${CMAKE_INSTALL_PREFIX}/${MOP_APP_NAME}.exe\")" COMPONENT Runtime)

    # Install the .ico alongside the executable (needed for the DefaultIcon registry entry).
    install(FILES assets/mop3d-icon.ico DESTINATION . COMPONENT Runtime)
elseif(APPLE)
    # The bundle and its internal executable will be named ${MOP_APP_NAME} (not ${PROJECT_NAME}).
    set(MOP_BUNDLE_EXECUTABLE "${MOP_APP_NAME}")

    # Step 1: expand @MOP_PLIST_EXTENSIONS@, @MOP_BUNDLE_EXECUTABLE@, etc.
    configure_file(
        assets/Info.plist.in
        "${CMAKE_CURRENT_BINARY_DIR}/Info.plist"
        @ONLY
    )
    # Step 2: point the bundle to the already-expanded plist (no further substitution)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_BINARY_DIR}/Info.plist"
    )

    # Bundle the .icns icon into ${MOP_APP_NAME}.app/Contents/Resources/
    set(MACOS_ICON "${CMAKE_CURRENT_SOURCE_DIR}/assets/mop3d-icon.icns")
    set_source_files_properties(${MACOS_ICON} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources"
    )
    target_sources(${PROJECT_NAME} PRIVATE ${MACOS_ICON})

    find_program(MACDEPLOYQT_EXECUTABLE NAMES macdeployqt HINTS "${Qt6_DIR}/bin" "${Qt6_DIR}/../../../bin" "${Qt6_DIR}/../../bin" REQUIRED)
    install(CODE "execute_process(COMMAND \"${MACDEPLOYQT_EXECUTABLE}\" \"\${CMAKE_INSTALL_PREFIX}/${MOP_APP_NAME}.app\")" COMPONENT Runtime)
elseif(UNIX AND NOT APPLE)
    configure_file(assets/application.desktop.in "${CMAKE_CURRENT_BINARY_DIR}/${MOP_APP_NAME}.desktop" @ONLY)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${MOP_APP_NAME}.desktop" DESTINATION share/applications COMPONENT Runtime)
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

set(CPACK_PACKAGE_NAME "${MOP_APP_NAME}")
set(CPACK_PACKAGE_VENDOR "VCLab")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${MOP_APP_NAME} is a mesh processing application developed by the Visual Computing Lab, ISTI - CNR.")
set(CPACK_RESOURCE_FILE_LICENSE      "${CMAKE_SOURCE_DIR}/LICENSE")
# Install to "C:\Program Files\MeshOp3D" (no version suffix).
# CPack's default is "${CPACK_PACKAGE_NAME} ${CPACK_PACKAGE_VERSION}".
set(CPACK_PACKAGE_INSTALL_DIRECTORY  "${MOP_APP_NAME}")
set(CPACK_PACKAGE_VERSION ${CMAKE_PROJECT_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${CMAKE_PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${CMAKE_PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${CMAKE_PROJECT_VERSION_PATCH})
if(NOT WIN32 AND NOT APPLE)
    # Linux only: used by DEB/TGZ packaging.
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
    set(CPACK_GENERATOR "ZIP;NSIS")
    set(CPACK_NSIS_DISPLAY_NAME              "${MOP_APP_NAME}")
    set(CPACK_NSIS_INSTALLED_ICON_NAME       "${MOP_APP_NAME}.exe")
    set(CPACK_NSIS_MUI_ICON                  "${CMAKE_CURRENT_SOURCE_DIR}/assets/mop3d-icon.ico")
    set(CPACK_NSIS_MUI_UNIICON               "${CMAKE_CURRENT_SOURCE_DIR}/assets/mop3d-icon.ico")
    set(CPACK_NSIS_URL_INFO_ABOUT            "https://github.com/cnr-isti-vclab/MeshOp3D")
    set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS    "${MOP_NSIS_REGISTRY_ENTRIES}")
    set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS  "${MOP_NSIS_UNREGISTRY_ENTRIES}")
elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
else()
    set(CPACK_GENERATOR "DEB;TGZ;AppImage")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt6core6, libqt6gui6, libqt6widgets6, libqt6network6, libgmp10, libmpfr6, libtbb12, libembree4")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Alessandro Muntoni")
endif()

include(CPack)
