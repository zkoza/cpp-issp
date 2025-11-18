cmake_minimum_required(VERSION 3.10)
project (concept_error_messages)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS Off)

add_executable(unconstrained unconstrained.cpp)
add_executable(constrained constrained.cpp)



