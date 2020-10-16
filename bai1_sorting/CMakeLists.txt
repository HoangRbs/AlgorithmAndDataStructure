cmake_minimum_required(VERSION 3.10)

# set the project name
project(sorting VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(includes)

add_executable(sorts_algorithm src/main.cpp)

target_link_libraries(sorts_algorithm PUBLIC Timer)
