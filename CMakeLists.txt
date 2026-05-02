cmake_minimum_required(VERSION 3.26)

project(Sokoke
    VERSION 0.0.0
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(Engine)
# add_subdirectory(Runtime)
# add_subdirectory(Editor)
add_subdirectory(Samples)