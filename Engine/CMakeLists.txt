add_subdirectory(Core)

add_library(sokoke_engine INTERFACE)
add_library(sokoke::engine ALIAS sokoke_engine)

target_link_libraries(sokoke_engine
    INTERFACE
        sokoke::core
)