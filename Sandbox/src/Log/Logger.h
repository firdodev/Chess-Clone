#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>

enum class LogLevel {
    INFO,
    WARN,
    ERROR,
    TRACE
};

class Logger {
public:
    template <typename... Args>
    void Log(LogLevel level, bool shouldThrow, Args... args) {
        std::ostringstream oss;
        (oss << ... << args);
        Log(level, shouldThrow, oss.str());
    }

private:
    void Log(LogLevel level, bool shouldThrow, const std::string& msg);
};

extern Logger g_Logger;

#define LOG_INFO(...) g_Logger.Log(LogLevel::INFO, false, __VA_ARGS__)
#define LOG_WARN(...) g_Logger.Log(LogLevel::WARN, false, __VA_ARGS__)
#define LOG_ERROR(...) g_Logger.Log(LogLevel::ERROR, true, __VA_ARGS__)
#define LOG_TRACE(...) g_Logger.Log(LogLevel::TRACE, false, __VA_ARGS__)