#include "include/util_strings.h"

const char **addStringsToArray(const char **originalArray, unsigned int originalCount, const char **newStrings,
                               unsigned int newCount) {
    // Allocate memory for the new array with extra slots for new strings
    const char **newArray = malloc((originalCount + newCount) * sizeof(char *));
    if (!newArray) {
        // Handle memory allocation error
        perror("Failed to allocate memory for newArray");
        return NULL;
    }

    // Copy original strings
    for (unsigned int i = 0; i < originalCount; i++) {
        newArray[i] = originalArray[i];
    }

    // Add new strings to the array
    for (unsigned int i = 0; i < newCount; i++) {
        newArray[originalCount + i] = newStrings[i];
    }

    return newArray;
}
