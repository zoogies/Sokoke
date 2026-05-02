cmake_minimum_required(VERSION 3.26)

# avoids SDL pdb write contention in CI
cmake_policy(SET CMP0141 NEW)
if (NOT DEFINED CMAKE_MSVC_DEBUG_INFORMATION_FORMAT)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<$<CONFIG:Debug,RelWithDebInfo>:Embedded>")
endif()

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
