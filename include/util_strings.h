#ifndef APP_UTILS_H
#define APP_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // For strcmp

const char **addStringsToArray(const char **originalArray, unsigned int originalCount, const char **newStrings, unsigned int newCount);

#endif // APP_UTILS_H