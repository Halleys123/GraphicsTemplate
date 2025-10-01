#include "Logger.hpp"

#include <cstdio>
#include <cstdarg>
#include <chrono>
#include <iomanip>
#include <sstream>

int Logger::nextLoggerId = 1;
int Logger::maxNameLen = 1;
bool Logger::globalDisable = false;

// Foreground colors
const char *Logger::RED = "\x1b[38;2;255;99;99m";
const char *Logger::BLUE = "\x1b[38;2;99;150;255m";
const char *Logger::GREEN = "\x1b[38;2;120;255;120m";
const char *Logger::YELLOW = "\x1b[38;2;255;220;99m";
const char *Logger::PURPLE = "\x1b[38;2;200;120;255m";
const char *Logger::CYAN = "\x1b[38;2;99;255;255m";
const char *Logger::ORANGE = "\x1b[38;2;255;165;99m";
const char *Logger::GRAY = "\x1b[38;2;170;170;170m";
const char *Logger::WHITE = "\x1b[38;2;255;255;255m";
const char *Logger::SUCCESS_GREEN = "\x1b[38;2;50;200;50m";
const char *Logger::BRIGHT_RED = "\x1b[38;2;255;85;85m";
const char *Logger::BRIGHT_GREEN = "\x1b[38;2;85;255;85m";
const char *Logger::BRIGHT_BLUE = "\x1b[38;2;85;170;255m";
const char *Logger::BRIGHT_YELLOW = "\x1b[38;2;255;255;85m";
const char *Logger::BRIGHT_CYAN = "\x1b[38;2;85;255;255m";
const char *Logger::BRIGHT_MAGENTA = "\x1b[38;2;255;85;255m";
const char *Logger::BRIGHT_WHITE = "\x1b[38;2;255;255;255m";
const char *Logger::DARK_GRAY = "\x1b[38;2;100;100;100m";
const char *Logger::LIGHT_GRAY = "\x1b[38;2;200;200;200m";

// Background colors
const char *Logger::BG_RED = "\x1b[48;2;120;30;30m";
const char *Logger::BG_GREEN = "\x1b[48;2;30;120;30m";
const char *Logger::BG_BLUE = "\x1b[48;2;30;60;120m";
const char *Logger::BG_YELLOW = "\x1b[48;2;120;100;20m";
const char *Logger::BG_PURPLE = "\x1b[48;2;80;40;120m";
const char *Logger::BG_CYAN = "\x1b[48;2;20;100;120m";
const char *Logger::BG_GRAY = "\x1b[48;2;60;60;60m";
const char *Logger::BG_DARK_GRAY = "\x1b[48;2;40;40;40m";
const char *Logger::BG_BRIGHT_RED = "\x1b[48;2;180;40;40m";
const char *Logger::BG_BRIGHT_GREEN = "\x1b[48;2;40;180;40m";

// Text styles
const char *Logger::RESET = "\x1b[0m";
const char *Logger::BOLD = "\x1b[1m";
const char *Logger::ITALIC = "\x1b[3m";
const char *Logger::UNDERLINE = "\x1b[4m";
const char *Logger::DIM = "\x1b[2m";
const char *Logger::BLINK = "\x1b[5m";

Logger::Logger() : currentLevel(LOG_INFO), loggerName("DEFAULT"), loggerId(nextLoggerId++)
{
    disabled = globalDisable;
    loggerColor = generateColorFromId(loggerId);
}

Logger::Logger(const std::string &name) : currentLevel(LOG_INFO), loggerName(name), loggerId(nextLoggerId++)
{
    disabled = globalDisable;
    loggerColor = generateColorFromId(loggerId);
    setLoggerName(name);
}

// Generate distinct colors based on logger ID
std::string Logger::generateColorFromId(int id) const
{
    float hue = (id * 137.5f) / 360.0f;
    hue = hue - (int)hue;

    float s = 0.8f;
    float v = 0.9f;

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

    int red = (int)((r + m) * 255);
    int green = (int)((g + m) * 255);
    int blue = (int)((b + m) * 255);

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

std::string Logger::getPaddedLoggerName() const
{
    size_t padLen = maxNameLen > loggerName.size() ? maxNameLen - loggerName.size() : 0;
    return loggerName + std::string(padLen, ' ');
}

void Logger::setLogLevel(LogLevel level)
{
    currentLevel = level;
}

void Logger::setLoggerName(const std::string &name)
{
    loggerName = name;
    maxNameLen = maxNameLen > name.size() ? maxNameLen : name.size();
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
    if (this->disabled)
        return;
    if (currentLevel <= LOG_SILLY)
    {
        std::string timestamp = getCurrentTimestamp();
        printf("%s%s[%s]%s %s%s[%s]%s %s%s%s SILLY %s  ",
               DIM, DARK_GRAY, timestamp.c_str(), RESET,
               loggerColor.c_str(), ITALIC, getPaddedLoggerName().c_str(), RESET,
               BG_DARK_GRAY, BRIGHT_CYAN, BOLD, RESET);

        printf("%s%s", CYAN, DIM);
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        printf("%s\n", RESET);
    }
}

void Logger::debug(const char *format, ...) const
{
    if (this->disabled)
        return;
    if (currentLevel <= LOG_DEBUG)
    {
        std::string timestamp = getCurrentTimestamp();
        printf("%s%s[%s]%s %s%s[%s]%s %s%s%s DEBUG %s ",
               DIM, LIGHT_GRAY, timestamp.c_str(), RESET,
               loggerColor.c_str(), BOLD, getPaddedLoggerName().c_str(), RESET,
               BG_PURPLE, BRIGHT_WHITE, BOLD, RESET);

        printf("%s", BRIGHT_MAGENTA);
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        printf("%s\n", RESET);
    }
}

void Logger::verbose(const char *format, ...) const
{
    if (this->disabled)
        return;
    if (currentLevel <= LOG_VERBOSE)
    {
        std::string timestamp = getCurrentTimestamp();
        printf("%s%s[%s]%s %s%s[%s]%s %s%s%sVERBOSE%s ",
               DIM, GRAY, timestamp.c_str(), RESET,
               loggerColor.c_str(), BOLD, getPaddedLoggerName().c_str(), RESET,
               BG_BLUE, BRIGHT_WHITE, BOLD, RESET);

        printf("%s%s", BRIGHT_BLUE, ITALIC);
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        printf("%s\n", RESET);
    }
}

void Logger::info(const char *format, ...) const
{
    if (this->disabled)
        return;
    if (currentLevel <= LOG_INFO)
    {
        std::string timestamp = getCurrentTimestamp();
        printf("%s%s[%s]%s %s%s[%s]%s %s%s%s INFO  %s ",
               LIGHT_GRAY, BOLD, timestamp.c_str(), RESET,
               loggerColor.c_str(), BOLD, getPaddedLoggerName().c_str(), RESET,
               BG_GRAY, BRIGHT_WHITE, BOLD, RESET);

        printf("%s", BRIGHT_WHITE);
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        printf("%s\n", RESET);
    }
}

void Logger::warn(const char *format, ...) const
{
    if (this->disabled)
        return;
    if (currentLevel <= LOG_WARN)
    {
        std::string timestamp = getCurrentTimestamp();
        printf("%s%s[%s]%s %s%s[%s]%s %s%s%s%s WARN  %s ",
               YELLOW, BOLD, timestamp.c_str(), RESET,
               loggerColor.c_str(), BOLD, getPaddedLoggerName().c_str(), RESET,
               BG_YELLOW, BOLD, UNDERLINE, BRIGHT_WHITE, RESET);

        printf("%s%s%s", BRIGHT_YELLOW, BOLD, ITALIC);
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        printf("%s\n", RESET);
    }
}

void Logger::error(const char *format, ...) const
{
    if (this->disabled)
        return;
    if (currentLevel <= LOG_ERROR)
    {
        std::string timestamp = getCurrentTimestamp();
        printf("%s%s[%s]%s %s%s[%s]%s %s%s%s%s ERROR %s ",
               RED, BOLD, timestamp.c_str(), RESET,
               loggerColor.c_str(), BOLD, getPaddedLoggerName().c_str(), RESET,
               BG_RED, BRIGHT_WHITE, BOLD, UNDERLINE, RESET);

        printf("%s%s", BRIGHT_RED, BOLD);
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        printf("%s\n", RESET);
    }
}

void Logger::fatal(const char *format, ...) const
{
    if (this->disabled)
        return;
    if (currentLevel <= LOG_FATAL)
    {
        std::string timestamp = getCurrentTimestamp();
        printf("%s%s%s[%s]%s %s%s%s[%s]%s %s%s%s%s%s FATAL %s ",
               BG_BRIGHT_RED, BRIGHT_WHITE, BOLD, timestamp.c_str(), RESET,
               BG_BRIGHT_RED, loggerColor.c_str(), BOLD, getPaddedLoggerName().c_str(), RESET,
               BG_BRIGHT_RED, BRIGHT_WHITE, BOLD, UNDERLINE, BLINK, RESET);

        printf("%s%s%s%s", BG_BRIGHT_RED, BRIGHT_WHITE, BOLD, BLINK);
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        printf("%s\n", RESET);
    }
}

void Logger::success(const char *format, ...) const
{
    if (this->disabled)
        return;
    if (currentLevel <= LOG_SUCCESS)
    {
        std::string timestamp = getCurrentTimestamp();
        printf("%s%s[%s]%s %s%s[%s]%s %s%s%s%sSUCCESS%s ",
               BRIGHT_GREEN, BOLD, timestamp.c_str(), RESET,
               loggerColor.c_str(), BOLD, getPaddedLoggerName().c_str(), RESET,
               BG_BRIGHT_GREEN, BRIGHT_WHITE, BOLD, UNDERLINE, RESET);

        printf("%s%s", BRIGHT_GREEN, BOLD);
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        printf("%s\n", RESET);
    }
}

void Logger::log(const char *format, ...) const
{
    if (this->disabled)
        return;
    std::string timestamp = getCurrentTimestamp();
    printf("%s%s[%s]%s %s%s[%s]%s %s%s%s LOG   %s ",
           LIGHT_GRAY, ITALIC, timestamp.c_str(), RESET,
           loggerColor.c_str(), BOLD, getPaddedLoggerName().c_str(), RESET,
           BG_PURPLE, BRIGHT_WHITE, BOLD, RESET);

    printf("%s%s", PURPLE, ITALIC);
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("%s\n", RESET);
}

Logger::~Logger() {}

void Logger::setDisable(bool newStatus)
{
    this->disabled = !this->globalDisable && newStatus;
}