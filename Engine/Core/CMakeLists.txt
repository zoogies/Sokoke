add_library(sokoke_core STATIC)
add_library(sokoke::core ALIAS sokoke_core)

target_sources(sokoke_core
    PRIVATE
        Sokoke.cpp

    PUBLIC
        Sokoke.hpp
)

target_include_directories(sokoke_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(sokoke_core
    PUBLIC
        cxx_std_23
)