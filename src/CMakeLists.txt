# Define executables
set(DRAW_NAME draw)
set(DRAW_SOURCES draw.cpp)
add_executable(${DRAW_NAME} ${DRAW_SOURCES})

set(IO_DIJKSTRA_NAME io_dijkstra)
set(IO_DIJKSTRA_SOURCES io_dijkstra.cpp)
add_executable(${IO_DIJKSTRA_NAME} ${IO_DIJKSTRA_SOURCES})

set(RANDOM_DIJKSTRA_NAME random_dijkstra)
set(RANDOM_DIJKSTRA_SOURCES random_dijkstra.cpp)
add_executable(${RANDOM_DIJKSTRA_NAME} ${RANDOM_DIJKSTRA_SOURCES})

set(IO_MST_NAME io_mst)
set(IO_MST_SOURCES io_mst.cpp)
add_executable(${IO_MST_NAME} ${IO_MST_SOURCES})

# Link common libraries
foreach(EXEC ${DRAW_NAME} ${IO_DIJKSTRA_NAME} ${RANDOM_DIJKSTRA_NAME} ${IO_MST_NAME})
    target_include_directories(${EXEC} PRIVATE ${OGDF_INCLUDE_DIRS})
    target_link_libraries(${EXEC} OGDF)

    set_target_properties(${EXEC}
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/bin"
    )
endforeach()
