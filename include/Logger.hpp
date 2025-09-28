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
    static const char *RESET;
    static const char *BG_RED;
    static const char *BOLD;

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