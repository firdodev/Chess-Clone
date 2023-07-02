#include "Logger.h"

void Logger::Log(LogLevel level, bool shouldThrow, const std::string& msg) {
    std::string level_str;
    std::string color_code;
    switch (level) {
    case LogLevel::INFO:
        level_str = "INFO";
        color_code = "\033[36m";
        break;
    case LogLevel::WARN:
        level_str = "WARN";
        color_code = "\033[33m";
        break;
    case LogLevel::ERROR:
        level_str = "ERROR";
        color_code = "\033[31m";
        break;
    case LogLevel::TRACE:
        level_str = "TRACE";
        color_code = "\033[35m";
        break;
    }
    std::cout << color_code << "[" << level_str << "] " << msg << "\033[0m" << std::endl;
}

Logger g_Logger;