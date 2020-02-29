#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "resources.h"

#define RESOURCES_READFILE_CHUNK 262144 /* 256 kB */

int readWholeFile(FILE* file, char** fileContent, size_t* fileSize) {
    if (file == NULL || fileContent == NULL || fileSize == NULL) {
        return RESOURCES_INVALID;
    }
    if (ferror(file)) {
        return RESOURCES_ERROR;
    }
    size_t totalBytes = 0;
    void* buffer = NULL;
    void* bufferRealloc;
    for (size_t i = 1;; ++i)
    {
        bufferRealloc = realloc(buffer, i * RESOURCES_READFILE_CHUNK);
        if (bufferRealloc == NULL) {
            free(buffer);
            return RESOURCES_ERROR_NOMEM;
        }
        buffer = bufferRealloc;
        size_t chunkBytes = fread(buffer + totalBytes, 1, RESOURCES_READFILE_CHUNK, file);
        totalBytes += chunkBytes;
        if (chunkBytes < RESOURCES_READFILE_CHUNK) {
            break;
        }
    }
    if (ferror(file)) {
        free(buffer);
        return RESOURCES_ERROR;
    }
    bufferRealloc = realloc(buffer, totalBytes + 1);
    if (bufferRealloc == NULL) {
        free(buffer);
        return RESOURCES_ERROR_NOMEM;
    }
    buffer = bufferRealloc;
    *fileContent = (char*)buffer;
    (*fileContent)[totalBytes] = '\0';
    *fileSize = totalBytes;
    return RESOURCES_SUCCESS;
}

int readFile(const char* pathname, char** fileContent, size_t* fileSize) {
    if (pathname == NULL || fileContent == NULL || fileSize == NULL) {
        return RESOURCES_INVALID;
    }
    FILE* file = fopen(pathname, "r");
    if (file == NULL) {
        return RESOURCES_ERROR_FOPEN;
    }
    int success = readWholeFile(file, fileContent, fileSize);
    if (success != RESOURCES_SUCCESS) {
        return success;
    }
    if (fclose(file) != 0) {
        return RESOURCES_ERROR_FCLOSE;
    } else {
        return RESOURCES_SUCCESS;
    }
}