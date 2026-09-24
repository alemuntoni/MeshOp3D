# MeshOp3D
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.24)
project(MeshOp3D-actions)

set(MOP_ACTIONS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/mop/*.h")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/mop/*.cpp")

add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES})
add_library(mop::actions ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${MOP_ACTIONS_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC mop::core)

target_sources(${PROJECT_NAME} PRIVATE ${HEADERS} ${SOURCES})
