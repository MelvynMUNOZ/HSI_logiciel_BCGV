/**
 * \file log.h
 * \brief Logging functions.
 * \author Raphael CAUSSE
 */

#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <time.h>

#define log_info(fmt, ...) log_write("INFO", __func__, __LINE__, fmt, ##__VA_ARGS__)
#define log_warn(fmt, ...) log_write("WARN", __func__, __LINE__, fmt, ##__VA_ARGS__)
#define log_error(fmt, ...) log_write("ERR", __func__, __LINE__, fmt, ##__VA_ARGS__)

/**
 * \brief Writes a log message to the log file or console.
 * \param level Logging level (e.g., "INFO", "WARN", "ERROR").
 * \param func Function where the log call is made.
 * \param line Line number in the source file.
 * \param fmt Format string (similar to printf).
 */
void log_write(const char *level, const char *func, int line, const char *fmt, ...);

#endif /* LOG_H */