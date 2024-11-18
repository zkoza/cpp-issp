cmake_minimum_required(VERSION 3.10)
project (prog_dyn)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS Off)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

add_executable(
                newton_simple
                newton-function/newton_simple.cpp
              )
add_executable(
                newton_counted
                newton-function/newton_counted.cpp
              )
add_executable(
                newton_class_vector
                newton-class-vector/newton.h
                newton-class-vector/newton.cpp
                newton-class-vector/newton_main.cpp
              )

add_executable(
                newton_class_map
                newton-class-map/newton.h
                newton-class-map/newton.cpp
                newton-class-map/newton_main.cpp
              )

add_executable(
                klejnoty
                klejnoty/klejnoty.h
                klejnoty/klejnoty.cpp
                klejnoty/main.cpp
              )


