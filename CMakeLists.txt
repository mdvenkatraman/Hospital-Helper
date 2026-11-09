cmake_minimum_required(VERSION 3.14)
project(HospitalHelper CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    "Header Files"
    "Header Files/R Tree Header Files"
    "Header Files/KD Tree Header Files"
    )

add_executable(app main.cpp RTree.cpp "Header Files/KD Tree Header Files/KD-Tree.cpp" parser.cpp tests.cpp)