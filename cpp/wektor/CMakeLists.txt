cmake_minimum_required(VERSION 3.5)

project(wektor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(wektor0 vector0.cpp)
add_executable(wektor1 vector1.cpp)
add_executable(wektor2 vector2.cpp)
add_executable(wektor3 vector3.cpp)
add_executable(wektor4 vector4.cpp)
add_executable(wektor5 vector5.cpp)
add_executable(wektor6 vector6.cpp)
add_executable(wektor7 main7.cpp vector7.cpp vector7.h)
add_executable(wektor8 main8.cpp vector8.cpp vector8.h)
add_executable(wektor9 main9.cpp vector9.cpp vector9.h)
add_executable(wektor10 main10.cpp vector10.cpp vector10.h)

