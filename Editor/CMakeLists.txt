add_executable(sokoke_editor
    Main.cpp
)

target_link_libraries(sokoke_editor
    PRIVATE
        sokoke::engine
        sokoke::backend::sdl
        sokoke::backend::sdl_gpu
)