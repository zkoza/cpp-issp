cmake_minimum_required(VERSION 3.10)
project (lista)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS Off)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

add_executable(lista1 lista1.cpp)
add_executable(lista2 lista2.cpp)
add_executable(bst bst.cpp)



