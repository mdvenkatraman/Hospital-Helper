cmake_minimum_required(VERSION 3.14)
project(HospitalHelper LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG         v3.6.0
)
FetchContent_MakeAvailable(Catch2)

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

target_include_directories(HospitalHelperTests PRIVATE ${Catch2_SOURCE_DIR}/single_include ${HED_DIRS})

target_compile_definitions(HospitalHelperTests PRIVATE CATCH_CONFIG_MAIN)
target_compile_options(HospitalHelperTests PRIVATE -Wall -Wextra -Wpedantic)