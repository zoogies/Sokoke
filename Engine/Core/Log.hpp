#pragma once

#include <cstdlib>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <spdlog/spdlog.h>

namespace sokoke {

class Log {
public:
    static void Init();
    static void Shutdown();

    static std::shared_ptr<spdlog::logger>& Core();

    static std::vector<std::string> GetRecentLines();

    template<typename... Args>
    static void LogAt(
        spdlog::level::level_enum level,
        spdlog::format_string_t<Args...> fmt,
        Args&&... args
    ) {
        Core()->log(level, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void ErrorAt(
        const spdlog::source_loc& loc,
        spdlog::format_string_t<Args...> fmt,
        Args&&... args
    ) {
        Core()->log(
            spdlog::level::err,
            "[{}:{} {}] {}",
            loc.filename,
            loc.line,
            loc.funcname,
            fmt::format(fmt, std::forward<Args>(args)...)
        );
    }

    template<typename... Args>
    [[noreturn]] static void FatalAt(
        const spdlog::source_loc& loc,
        spdlog::format_string_t<Args...> fmt,
        Args&&... args
    ) {
        Core()->log(
            spdlog::level::critical,
            "[{}:{} {}] {}",
            loc.filename,
            loc.line,
            loc.funcname,
            fmt::format(fmt, std::forward<Args>(args)...)
        );

        Core()->flush();
        std::abort();
    }

private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
};

}

// TODO: way down the line figure out if this is a real
//       macro i want to maintain for the engine seperately
#ifndef SK_DIST
    #define SK_TRACE(...) \
        ::sokoke::Log::LogAt(spdlog::level::trace, __VA_ARGS__)

    #define SK_DEBUG(...) \
        ::sokoke::Log::LogAt(spdlog::level::debug, __VA_ARGS__)
#else
    #define SK_TRACE(...)
    #define SK_DEBUG(...)
#endif

#define SK_INFO(...) \
    ::sokoke::Log::LogAt(spdlog::level::info, __VA_ARGS__)

#define SK_WARN(...) \
    ::sokoke::Log::LogAt(spdlog::level::warn, __VA_ARGS__)

#define SK_ERROR(...) \
    ::sokoke::Log::ErrorAt({__FILE__, __LINE__, __func__}, __VA_ARGS__)

#define SK_FATAL(...) \
    ::sokoke::Log::FatalAt({__FILE__, __LINE__, __func__}, __VA_ARGS__)