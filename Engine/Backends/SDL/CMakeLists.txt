find_package(SDL3 CONFIG REQUIRED)

add_library(backend_sdl STATIC)
add_library(sokoke::backend::sdl ALIAS backend_sdl)

target_sources(backend_sdl
    PRIVATE
        Window.cpp

    PUBLIC
        Window.hpp
)

target_include_directories(backend_sdl
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(backend_sdl
    PUBLIC
        sokoke::core
    PRIVATE
        SDL3::SDL3
)