#include "Logger.hpp"

#include <cstdio>
#include <cstdarg>
#include <chrono>
#include <iomanip>
#include <sstream>

// Initialize static member
int Logger::nextLoggerId = 1;

// Color constants
const char *Logger::RED = "\x1b[38;2;255;99;99m";
const char *Logger::BLUE = "\x1b[38;2;99;150;255m";
const char *Logger::GREEN = "\x1b[38;2;120;255;120m";
const char *Logger::YELLOW = "\x1b[38;2;255;220;99m";
const char *Logger::PURPLE = "\x1b[38;2;200;120;255m";
const char *Logger::CYAN = "\x1b[38;2;99;255;255m";
const char *Logger::ORANGE = "\x1b[38;2;255;165;99m";
const char *Logger::GRAY = "\x1b[38;2;170;170;170m";
const char *Logger::WHITE = "\x1b[38;2;255;255;255m";
const char *Logger::RESET = "\x1b[0m";
const char *Logger::BG_RED = "\x1b[48;2;80;20;20m";
const char *Logger::BOLD = "\x1b[1m";

// Default constructor
Logger::Logger() : currentLevel(LOG_INFO), loggerName("DEFAULT"), loggerId(nextLoggerId++)
{
    loggerColor = generateColorFromId(loggerId);
}

// Named constructor
Logger::Logger(const std::string &name) : currentLevel(LOG_INFO), loggerName(name), loggerId(nextLoggerId++)
{
    loggerColor = generateColorFromId(loggerId);
}

// Generate distinct colors based on logger ID
std::string Logger::generateColorFromId(int id) const
{
    // Use a formula to generate distinct colors
    // We'll use HSV color space converted to RGB for better distribution
    float hue = (id * 137.5f) / 360.0f; // Golden angle approximation for good distribution
    hue = hue - (int)hue;               // Keep fractional part

    // Convert HSV to RGB with fixed saturation and value for consistent brightness
    float s = 0.8f; // High saturation for vibrant colors
    float v = 0.9f; // High value for brightness on black background

    float c = v * s;
    float x = c * (1 - abs(fmod(hue * 6, 2) - 1));
    float m = v - c;

    float r, g, b;
    if (hue < 1.0f / 6)
    {
        r = c;
        g = x;
        b = 0;
    }
    else if (hue < 2.0f / 6)
    {
        r = x;
        g = c;
        b = 0;
    }
    else if (hue < 3.0f / 6)
    {
        r = 0;
        g = c;
        b = x;
    }
    else if (hue < 4.0f / 6)
    {
        r = 0;
        g = x;
        b = c;
    }
    else if (hue < 5.0f / 6)
    {
        r = x;
        g = 0;
        b = c;
    }
    else
    {
        r = c;
        g = 0;
        b = x;
    }

    // Convert to 0-255 range and ensure minimum brightness
    int red = (int)((r + m) * 255);
    int green = (int)((g + m) * 255);
    int blue = (int)((b + m) * 255);

    // Ensure minimum brightness for visibility on black background
    red = red < 100 ? 100 + (red % 156) : red;
    green = green < 100 ? 100 + (green % 156) : green;
    blue = blue < 100 ? 100 + (blue % 156) : blue;

    char colorBuffer[32];
    snprintf(colorBuffer, sizeof(colorBuffer), "\x1b[38;2;%d;%d;%dm", red, green, blue);
    return std::string(colorBuffer);
}

std::string Logger::getCurrentTimestamp() const
{
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                  now.time_since_epoch()) %
              1000;

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

void Logger::setLogLevel(LogLevel level)
{
    currentLevel = level;
}

void Logger::setLoggerName(const std::string &name)
{
    loggerName = name;
}

std::string Logger::getLoggerName() const
{
    return loggerName;
}

int Logger::getLoggerId() const
{
    return loggerId;
}

void Logger::silly(const char *format, ...) const
{
    if (currentLevel <= LOG_SILLY)
    {
        std::string timestamp = getCurrentTimestamp();
        printf("%s[%s] %s[%s]%s %sSILLY%s   ",
               GRAY, timestamp.c_str(),
               loggerColor.c_str(), loggerName.c_str(), RESET,
               CYAN, RESET);

        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        printf("%s\n", RESET);
    }
}

void Logger::debug(const char *format, ...) const
{
    if (currentLevel <= LOG_DEBUG)
    {
        std::string timestamp = getCurrentTimestamp();
        printf("%s[%s] %s[%s]%s %sDEBUG%s   ",
               GRAY, timestamp.c_str(),
               loggerColor.c_str(), loggerName.c_str(), RESET,
               PURPLE, RESET);

        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        printf("%s\n", RESET);
    }
}

void Logger::verbose(const char *format, ...) const
{
    if (currentLevel <= LOG_VERBOSE)
    {
        std::string timestamp = getCurrentTimestamp();
        printf("%s[%s] %s[%s]%s %sVERBOSE%s ",
               GRAY, timestamp.c_str(),
               loggerColor.c_str(), loggerName.c_str(), RESET,
               BLUE, RESET);

        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        printf("%s\n", RESET);
    }
}

void Logger::info(const char *format, ...) const
{
    if (currentLevel <= LOG_INFO)
    {
        std::string timestamp = getCurrentTimestamp();
        printf("%s[%s] %s[%s]%s %sINFO%s    ",
               GRAY, timestamp.c_str(),
               loggerColor.c_str(), loggerName.c_str(), RESET,
               WHITE, RESET);

        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        printf("%s\n", RESET);
    }
}

void Logger::warn(const char *format, ...) const
{
    if (currentLevel <= LOG_WARN)
    {
        std::string timestamp = getCurrentTimestamp();
        printf("%s[%s] %s[%s]%s %s%sWARN%s    ",
               GRAY, timestamp.c_str(),
               loggerColor.c_str(), loggerName.c_str(), RESET,
               YELLOW, BOLD, RESET);

        printf("%s", YELLOW);
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        printf("%s\n", RESET);
    }
}

void Logger::error(const char *format, ...) const
{
    if (currentLevel <= LOG_ERROR)
    {
        std::string timestamp = getCurrentTimestamp();
        printf("%s[%s] %s[%s]%s %s%sERROR%s   ",
               GRAY, timestamp.c_str(),
               loggerColor.c_str(), loggerName.c_str(), RESET,
               RED, BOLD, RESET);

        printf("%s", RED);
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        printf("%s\n", RESET);
    }
}

void Logger::fatal(const char *format, ...) const
{
    if (currentLevel <= LOG_FATAL)
    {
        std::string timestamp = getCurrentTimestamp();
        printf("%s[%s] %s[%s]%s %s%s%sFATAL%s   ",
               GRAY, timestamp.c_str(),
               loggerColor.c_str(), loggerName.c_str(), RESET,
               BG_RED, WHITE, BOLD, RESET);

        printf("%s%s", RED, BOLD);
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        printf("%s\n", RESET);
    }
}

void Logger::log(const char *format, ...) const
{
    std::string timestamp = getCurrentTimestamp();
    printf("%s[%s] %s[%s]%s %sLOG%s     ",
           GRAY, timestamp.c_str(),
           loggerColor.c_str(), loggerName.c_str(), RESET,
           GREEN, RESET);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("%s\n", RESET);
}