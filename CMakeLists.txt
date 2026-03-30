cmake_minimum_required(VERSION 3.14)
project(DAProject)

set(CMAKE_CXX_STANDARD 17)

add_executable(myProg
        code/main.cpp
        code/utils/DataLoader.cpp
        code/algorithms/EdmondsKarp.cpp
        code/App/Assignments.cpp
        code/App/BatchMode.cpp
        code/App/Menu.cpp
        code/data_structures/UFDS.cpp
)

target_include_directories(myProg PRIVATE
        ${CMAKE_SOURCE_DIR}/code
)