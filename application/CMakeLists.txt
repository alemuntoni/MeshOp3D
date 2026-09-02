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

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${FORMS} assets.qrc)
target_link_libraries(${PROJECT_NAME} PRIVATE vclib hlmp::core)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin COMPONENT Runtime
    BUNDLE DESTINATION . COMPONENT Runtime
)
