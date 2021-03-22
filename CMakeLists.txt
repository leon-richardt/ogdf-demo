cmake_minimum_required(VERSION 3.8)

project(ogdf-demo)

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/lib/ogdf")

add_subdirectory(src)
