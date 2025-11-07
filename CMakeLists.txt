cmake_minimum_required(VERSION 3.27)

set(PROJECT_NAME XEngine)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(${PROJECT_NAME})

# Engine side
add_subdirectory(XEngine)

# Client side
add_subdirectory(SandBox)