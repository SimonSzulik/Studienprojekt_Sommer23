cmake_minimum_required(VERSION 3.25)
project(project)

# Find X11 Libraries
find_package(X11 REQUIRED)

message(STATUS "X11_FOUND = ${X11_FOUND}")
message(STATUS "X11_INCLUDE_DIR = ${X11_INCLUDE_DIR}")
message(STATUS "X11_LIBRARIES = ${X11_LIBRARIES}")

# Include X11 Header files
include_directories(${X11_INCLUDE_DIR})

# Include LEDA Header files
include_directories(./lib/LEDA-7/incl)

link_directories(${X11_LIBRARIES})

set(CMAKE_CXX_STANDARD 17)

add_executable(cycle_canceling src/Cycle_Canceling.cpp)
add_executable(successive_shortest_path src/Succesive_Shortest_Path.cpp)

target_link_libraries(cycle_canceling ${CMAKE_SOURCE_DIR}/lib/LEDA-7/libleda.a ${X11_LIBRARIES} -lXft)
target_link_libraries(successive_shortest_path ${CMAKE_SOURCE_DIR}/lib/LEDA-7/libleda.a ${X11_LIBRARIES} -lXft)
