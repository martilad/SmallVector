cmake_minimum_required(VERSION 3.13)
project(mpc_small_vector_martilad)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "-Werror -Wall -Wextra -O3")
set(CMAKE_CXX_FLAGS "-pedantic -Wall")

add_executable(martilad main.cpp small_vector.h)