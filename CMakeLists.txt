cmake_minimum_required(VERSION 3.12)

project(astar CXX C)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include_directories(src)
file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR} src/*.c??)
file(GLOB_RECURSE HEADER_FILES {CMAKE_CURRENT_SOURCE_DIR} src/*.h)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES} src/grid.debug)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)