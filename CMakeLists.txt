cmake_minimum_required(VERSION 3.10)
project(PathPlanning)

include_directories(/usr/include/python3.8 include)

add_executable(dfs src/DepthFirstSearch/dfs.cpp src/Nodes.cpp)
target_link_libraries(dfs libpython3.8.so)

add_executable(bfs src/BreadthFirstSearch/bfs.cpp src/Nodes.cpp)
target_link_libraries(bfs libpython3.8.so)

add_executable(dji src/Djikstra/Djikstra.cpp src/Nodes.cpp)
target_link_libraries(dji libpython3.8.so)

add_executable(astar src/A*/Astar.cpp)
target_link_libraries(astar libpython3.8.so)

add_executable(dstar src/D*/Dstar.cpp)
target_link_libraries(dstar libpython3.8.so)

add_executable(rrt src/RRT/RRT.cpp src/Random_tree.cpp)
target_link_libraries(rrt libpython3.8.so)

add_executable(rrtstar src/RRT*/RRT*.cpp src/Random_tree.cpp)
target_link_libraries(rrtstar libpython3.8.so)
