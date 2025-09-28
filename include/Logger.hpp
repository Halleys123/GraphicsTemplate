#pragma once
#include <string>

class Logger
{
public:
    enum LogLevel
    {
        LOG_SILLY = 0,
        LOG_DEBUG = 1,
        LOG_VERBOSE = 2,
        LOG_INFO = 3,
        LOG_WARN = 4,
        LOG_ERROR = 5,
        LOG_FATAL = 6,
        LOG_SUCCESS = 7
    };

private:
    static int maxNameLen;
    static int nextLoggerId;

    // Foreground colors
    static const char *RED;
    static const char *BLUE;
    static const char *GREEN;
    static const char *YELLOW;
    static const char *PURPLE;
    static const char *CYAN;
    static const char *ORANGE;
    static const char *GRAY;
    static const char *WHITE;
    static const char *SUCCESS_GREEN;
    static const char *BRIGHT_RED;
    static const char *BRIGHT_GREEN;
    static const char *BRIGHT_BLUE;
    static const char *BRIGHT_YELLOW;
    static const char *BRIGHT_CYAN;
    static const char *BRIGHT_MAGENTA;
    static const char *BRIGHT_WHITE;
    static const char *DARK_GRAY;
    static const char *LIGHT_GRAY;

    // Background colors
    static const char *BG_RED;
    static const char *BG_GREEN;
    static const char *BG_BLUE;
    static const char *BG_YELLOW;
    static const char *BG_PURPLE;
    static const char *BG_CYAN;
    static const char *BG_GRAY;
    static const char *BG_DARK_GRAY;
    static const char *BG_BRIGHT_RED;
    static const char *BG_BRIGHT_GREEN;

    // Text styles
    static const char *RESET;
    static const char *BOLD;
    static const char *ITALIC;
    static const char *UNDERLINE;
    static const char *DIM;
    static const char *BLINK;

    LogLevel currentLevel;
    std::string loggerName;
    int loggerId;
    std::string loggerColor;

    std::string getCurrentTimestamp() const;
    std::string generateColorFromId(int id) const;

public:
    Logger();
    Logger(const std::string &name);

    ~Logger();

    void setLogLevel(LogLevel level);
    void setLoggerName(const std::string &name);
    std::string getLoggerName() const;
    int getLoggerId() const;

    std::string getPaddedLoggerName() const;

    void silly(const char *format, ...) const;
    void debug(const char *format, ...) const;
    void verbose(const char *format, ...) const;
    void info(const char *format, ...) const;
    void warn(const char *format, ...) const;
    void error(const char *format, ...) const;
    void fatal(const char *format, ...) const;
    void success(const char *format, ...) const;
    void log(const char *format, ...) const;
};