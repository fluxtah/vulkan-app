#ifndef APP_ERROR_H
#define APP_ERROR_H
#include <stdio.h>
#include <stdlib.h>

// Function to actually log the error message
void logErrorImpl(const char* message, const char* file, int line);
// Macro that wraps the logErrorImpl function, automatically filling in the file and line number
#define LOG_ERROR(message) logErrorImpl(message, __FILE__, __LINE__)

#endif //APP_ERROR_H
