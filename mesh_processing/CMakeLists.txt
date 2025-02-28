#automoc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "*.h")
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")
file(GLOB_RECURSE FORMS CONFIGURE_DEPENDS "*.ui")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Forms Files" FILES ${FORMS})

add_executable(MeshProcessing ${HEADERS} ${SOURCES} ${FORMS})
target_link_libraries(MeshProcessing PRIVATE vclib)
target_include_directories(MeshProcessing PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(MeshProcessing PRIVATE
    MESH_PROCESSING_ASSETS_DIR="${MESH_PROCESSING_ASSETS_DIR}")
