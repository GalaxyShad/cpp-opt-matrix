cmake_minimum_required(VERSION 3.28)
project(cpp_opt_matrix)

set(CMAKE_CXX_STANDARD 23)

add_executable(cpp_opt_matrix main.cpp
        ICuteVector.h
        CuteHashedVector.h
        CuteMatrix.h
        CuteStdVector.h)
