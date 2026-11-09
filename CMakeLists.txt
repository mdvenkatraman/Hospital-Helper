cmake_minimum_required(VERSION 3.14)
project(HospitalHelper LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(HED_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/Header Files"
    "${CMAKE_CURRENT_SOURCE_DIR}/Header Files/R Tree Header Files"
    "${CMAKE_CURRENT_SOURCE_DIR}/Header Files/KD Tree Header Files"
)

add_executable(HospitalHelper
    main.cpp
    RTree.cpp
    KD-Tree.cpp
    Parser.cpp
)

target_include_directories(HospitalHelper PRIVATE ${HED_DIRS})
target_compile_options(HospitalHelper PRIVATE -Wall -Wextra -Wpedantic)

add_executable(HospitalHelperTests
    tests.cpp
)

target_include_directories(HospitalHelperTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/Catch2
    ${HED_DIRS}
)

target_compile_definitions(HospitalHelperTests PRIVATE CATCH_CONFIG_MAIN)
target_compile_options(HospitalHelperTests PRIVATE -Wall -Wextra -Wpedantic)