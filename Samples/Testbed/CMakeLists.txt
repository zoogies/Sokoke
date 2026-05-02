add_executable(testbed
    Main.cpp
)

target_link_libraries(testbed
    PRIVATE
        sokoke::engine
        sokoke::backend::sdl
)