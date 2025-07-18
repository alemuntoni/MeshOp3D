cmake_minimum_required(VERSION 3.24)
project(hlmp-core)

### HLMP target
set(HLMP_CORE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/vclib/*.h")

add_library(${PROJECT_NAME} INTERFACE ${HEADERS})
add_library(hlmp::core ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} INTERFACE
    ${HLMP_CORE_INCLUDE_DIR})

# todo - change
target_compile_definitions(${PROJECT_NAME} INTERFACE VCLIB_PROCESSING_MODULE)

target_link_libraries(${PROJECT_NAME} INTERFACE vclib::core)

target_sources(${PROJECT_NAME} PRIVATE ${HEADERS})
