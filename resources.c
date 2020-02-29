#include <stdlib.h>
#include <stdio.h>

#include "resources.h"

#define RESOURCES_READFILE_CHUNK 262144 /* 256 kB */

int readWholeFile(FILE* file, char** fileContent, size_t* fileSize) {
    if (file == NULL || fileContent == NULL || fileSize == NULL) {
        fputs("Error: Invalid argument to readWholeFile.\n", stderr);
        return RESOURCES_INVALID;
    }
    if (ferror(file)) {
        fputs("Error: Error indicator of file is set.\n", stderr);
        return RESOURCES_ERROR;
    }
    size_t totalBytes = 0;
    void* buffer = NULL;
    void* bufferRealloc;
    for (size_t i = 1;; ++i) {
        bufferRealloc = realloc(buffer, i * RESOURCES_READFILE_CHUNK);
        if (bufferRealloc == NULL) {
            fputs("Error: Could not allocate memory for file contents.\n", stderr);
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
        fputs("Error: Error indicator of file is set.\n", stderr);
        free(buffer);
        return RESOURCES_ERROR;
    }
    bufferRealloc = realloc(buffer, totalBytes + 1);
    if (bufferRealloc == NULL) {
        fputs("Error: Could not reallocate memory for file contents.\n", stderr);
        free(buffer);
        return RESOURCES_ERROR_NOMEM;
    }
    buffer = bufferRealloc;
    *fileContent = (char*)buffer;
    (*fileContent)[totalBytes] = '\0';
    *fileSize = totalBytes;
    return RESOURCES_SUCCESS;
}

int readFileContents(const char* pathname, char** fileContent, size_t* fileSize) {
    if (pathname == NULL || fileContent == NULL || fileSize == NULL) {
        fprintf(stderr, "Error: Invalid argument to readFileContents: %s\n", pathname);
        return RESOURCES_INVALID;
    }
    FILE* file = fopen(pathname, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Opening file failed: %s\n", pathname);
        return RESOURCES_ERROR_FOPEN;
    }
    int success = readWholeFile(file, fileContent, fileSize);
    if (success != RESOURCES_SUCCESS) {
        fprintf(stderr, "Error: Reading file failed: %s\n", pathname);
        return success;
    }
    if (fclose(file) != 0) {
        fprintf(stderr, "Error: Closing file failed: %s\n", pathname);
        return RESOURCES_ERROR_FCLOSE;
    } else {
        return RESOURCES_SUCCESS;
    }
}

void freeFileContents(char** fileContent) {
    free(*fileContent);
}