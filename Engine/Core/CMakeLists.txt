add_library(sokoke_core STATIC)
add_library(sokoke::core ALIAS sokoke_core)

target_sources(sokoke_core
    PRIVATE
        Sokoke.cpp
        Log.cpp

    PUBLIC
        Sokoke.hpp
        Log.hpp
        PlatformEvents.hpp
        Entities/Entity.hpp
)

target_compile_features(sokoke_core
    PUBLIC
        cxx_std_23
)

find_package(EnTT CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

target_link_libraries(sokoke_core
    PUBLIC # NOTE: should maybe be private
        EnTT::EnTT
        glm::glm
        spdlog::spdlog
)