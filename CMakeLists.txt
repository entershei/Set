cmake_minimum_required(VERSION 3.10)
project(Set)

set(CMAKE_CXX_STANDARD 17)

add_executable(Set main.cpp set.h)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG -fsanitize=address,undefined")