cmake_minimum_required(VERSION 3.23)
project(paralela)

set(CMAKE_CXX_STANDARD 14)

add_executable(paralela
        Pthreads/matrix-vector.cpp
        mpi/main.cpp
        mpi/mult.cpp
        mpi/oddEvenSort.cpp
        mpi/point_to_point.cpp)
