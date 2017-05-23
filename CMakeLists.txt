cmake_minimum_required(VERSION 3.0)

project(GRAPH
  VERSION 0.1.0
  LANGUAGES CXX
)

# Find GLPK

find_path(GLPK_INCLUDE_DIRS glpk.h)
find_library(GLPK_LIBRARIES glpk)

add_definitions(-Wall -Wextra -g -O3 -std=c++11)

#
# library
#

add_library(discgraph0
  lib/graph/Cover.cc
  lib/graph/Graph.cc
  lib/graph/Metrics.cc
  lib/graph/Problem.cc
  lib/graph/Random.cc
)

target_include_directories(discgraph0
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${GLPK_INCLUDE_DIRS}"
)

target_link_libraries(discgraph0
  "${GLPK_LIBRARIES}"
)

#
# executables
#

macro(add_graph_executable NAME)
  add_executable("${NAME}" "bin/${NAME}.cc")
  target_include_directories("${NAME}"
    PRIVATE
      "${CMAKE_CURRENT_SOURCE_DIR}/include"
  )
  target_link_libraries("${NAME}" discgraph0)
endmacro()

add_graph_executable(graph_features)
add_graph_executable(xp_random)
add_graph_executable(xp_uniform)
add_graph_executable(xp_exact)
add_graph_executable(xp_approx)
