cmake_minimum_required(VERSION 3.8)

project(ogdf-demo)
set(CMAKE_CXX_STANDARD 11)

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/lib/ogdf")

add_subdirectory(src)
