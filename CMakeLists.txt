cmake_minimum_required(VERSION 3.14)
project(HospitalHelper LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(HED_DIRS
    "${BASE_DIR}/Header Files"
    "${BASE_DIR}/Header Files/R Tree Header Files"
    "${BASE_DIR}/Header Files/KD Tree Header Files"
)

add_executable(HospitalHelper
    main.cpp
    RTree.cpp
    "Header Files/KD Tree Header Files/KD-Tree.cpp"
    parser.cpp
)

target_include_directories(HospitalHelper PRIVATE ${HED_DIRS})
target_compile_options(HospitalHelper PRIVATE -Wall -Wextra -Wpedantic)

add_subdirectory(${BASE_DIR}/extern/Catch2)

add_executable(HospitalHelperTests
    tests.cpp
)

target_include_directories(HospitalHelperTests PRIVATE ${HED_DIRS})
target_link_libraries(HospitalHelperTests PRIVATE Catch2::Catch2WithMain)
target_compile_options(HospitalHelperTests PRIVATE -Wall -Wextra -Wpedantic)