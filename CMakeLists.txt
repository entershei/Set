cmake_minimum_required(VERSION 3.10)
project(Set)

set(CMAKE_CXX_STANDARD 17)

add_executable(Set main.cpp set.h)
add_executable(Set_testing
        main.cpp
        set.h
        test/test.cpp)

target_link_libraries(Set_testing  gtest -lpthread)


if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG -fsanitize=address,undefined")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17 -pedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")
    set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)
endif ()