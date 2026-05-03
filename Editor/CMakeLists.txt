add_executable(sokoke_editor
    Main.cpp
    Panels/Hierarchy.cpp
)

target_link_libraries(sokoke_editor
    PRIVATE
        sokoke::engine
        sokoke::backend::sdl
        sokoke::backend::sdl_gpu
)

target_include_directories(sokoke_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
