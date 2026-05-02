add_subdirectory(Core)
add_subdirectory(Backends)

add_library(sokoke_engine INTERFACE)
add_library(sokoke::engine ALIAS sokoke_engine)

target_link_libraries(sokoke_engine
    INTERFACE
        sokoke::core
)

target_include_directories(sokoke_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
