#ifndef RESOURCES_H
#define RESOURCES_H

#include <stdlib.h>

#define RESOURCES_SUCCESS       0
#define RESOURCES_INVALID       1
#define RESOURCES_ERROR         2
#define RESOURCES_ERROR_NOMEM   3
#define RESOURCES_ERROR_FOPEN   4
#define RESOURCES_ERROR_FCLOSE  5

/**
 * Read the whole file into `fileContent`, a null-terminated string.
 * After use, you have to free it yourself.
 */
int readFile(const char* pathname, char** fileContent, size_t* fileSize);

#endif