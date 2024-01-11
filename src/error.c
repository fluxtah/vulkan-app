#include "include/error.h"

void logErrorImpl(const char* message, const char* file, int line) {
    fprintf(stderr, "Error [%s:%d]: %s\n", file, line, message);
    // Add additional logging actions here (e.g., logging to a file)
}