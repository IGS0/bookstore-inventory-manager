#ifndef LOGGER_H
#define LOGGER_H

#define BUFFER_SIZE 256

typedef enum {
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR
} LogLevel;

void set_log_file(const char *filename);
void set_log_level(LogLevel level);
void log_message(LogLevel level, const char *format, ...);
void close_log_file();

#endif // LOGGER_H
