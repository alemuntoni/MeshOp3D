cmake_minimum_required(VERSION 3.24)
project(HLMP-application)

if (NOT TARGET vclib-3rd-qt)
    message(FATAL_ERROR "Qt6 not found. Please set Qt6_DIR to the Qt6Config.cmake file.")
endif()

#automoc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "*.h")
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")
file(GLOB_RECURSE FORMS CONFIGURE_DEPENDS "*.ui")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Forms Files" FILES ${FORMS})

target_include_directories(vclib-render PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${FORMS})
target_link_libraries(${PROJECT_NAME} PRIVATE vclib hlmp::core)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(${PROJECT_NAME} PRIVATE
    HLMP_ASSETS_DIR="${HLMP_ASSETS_DIR}")
