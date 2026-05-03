#include "Log.hpp"

#include <filesystem>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/ringbuffer_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace sokoke {

std::shared_ptr<spdlog::logger> Log::s_CoreLogger;

static std::shared_ptr<spdlog::sinks::ringbuffer_sink_mt> s_RingBufferSink;

void Log::Init()
{
    std::filesystem::create_directories("logs");

    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
        "logs/sokoke.log",
        true
    );

    s_RingBufferSink = std::make_shared<spdlog::sinks::ringbuffer_sink_mt>(512);

    consoleSink->set_level(spdlog::level::trace);
    fileSink->set_level(spdlog::level::trace);
    s_RingBufferSink->set_level(spdlog::level::trace);

    s_CoreLogger = std::make_shared<spdlog::logger>(
        "Sokoke",
        spdlog::sinks_init_list{
            consoleSink,
            fileSink,
            s_RingBufferSink
        }
    );

    s_CoreLogger->set_level(spdlog::level::trace);

    /*
        Pattern:
        [%H:%M:%S]    time
        [%^%l%$]      colored log level in console
        %v            message
    */
    s_CoreLogger->set_pattern("[%H:%M:%S] [%^%l%$] %v");

    s_CoreLogger->flush_on(spdlog::level::warn);

    spdlog::set_default_logger(s_CoreLogger);

    s_CoreLogger->info("Logger initialized");
}

void Log::Shutdown()
{
    if (s_CoreLogger) {
        s_CoreLogger->info("Logger shutdown");
        s_CoreLogger->flush();
    }

    spdlog::shutdown();

    s_CoreLogger.reset();
    s_RingBufferSink.reset();
}

std::shared_ptr<spdlog::logger>& Log::Core()
{
    return s_CoreLogger;
}

std::vector<std::string> Log::GetRecentLines()
{
    std::vector<std::string> lines;

    if (!s_RingBufferSink) {
        return lines;
    }

    auto formatted = s_RingBufferSink->last_formatted();
    lines.reserve(formatted.size());

    for (const auto& line : formatted) {
        lines.emplace_back(line);
    }

    return lines;
}

}