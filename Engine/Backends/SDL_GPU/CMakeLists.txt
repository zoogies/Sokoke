find_package(SDL3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

add_library(backend_sdl_gpu STATIC)
add_library(sokoke::backend::sdl_gpu ALIAS backend_sdl_gpu)

target_sources(backend_sdl_gpu
    PRIVATE
        SDL_GPU.cpp

    PUBLIC
        SDL_GPU.hpp
)

target_link_libraries(backend_sdl_gpu
    PUBLIC
        sokoke::core
        sokoke::backend::sdl
        SDL3::SDL3
    PRIVATE
        imgui::imgui
)

target_include_directories(backend_sdl_gpu
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
