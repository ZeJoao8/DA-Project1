cmake_minimum_required(VERSION 3.14)
project(DAProject)

set(CMAKE_CXX_STANDARD 17)

add_executable(main
        main.cpp
        utils/DataLoader.cpp
        algorithms/EdmondsKarp.cpp
)

target_include_directories(main PRIVATE
        ${CMAKE_SOURCE_DIR}
)