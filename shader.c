#include <stdio.h>
#include <errno.h>

#include "shader.h"
#include "glad/glad.h"
#include "resources.h"

GLint checkShaderCompileError(GLuint shader) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar* infoLog = (char*) malloc(infoLogLength * sizeof(char));
        if (infoLog == NULL) {
            fprintf(stderr, "Critical: checkShaderCompileError could not allocate memory (%d)\n", errno);
            exit(EXIT_FAILURE);
        }
        glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);
        fprintf(stderr, "Error: Shader Compilation failed: %s\n", infoLog);
        free(infoLog);
    }
    return success;
}

GLint checkProgramLinkError(GLuint program) {
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLint infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar* infoLog = (char*) malloc(infoLogLength * sizeof(char));
        if (infoLog == NULL) {
            fprintf(stderr, "Critical: checkProgramLinkError could not allocate memory (%d)\n", errno);
            exit(EXIT_FAILURE);
        }
        glGetProgramInfoLog(program, infoLogLength, NULL, infoLog);
        fprintf(stderr, "Error: Linking Program failed: %s\n", infoLog);
        free(infoLog);
    }
    return success;
}

GLuint createShader(const char* shaderPath, GLenum shaderType) {
    if (shaderPath == NULL || !(shaderType != GL_VERTEX_SHADER || shaderType != GL_FRAGMENT_SHADER)) {
        fprintf(stderr, "Error: Invalid argument when creating shader: %s\n", shaderPath);
        return 0;
    }
    char* source;
    size_t sourceSize;
    int success = readFileContents(shaderPath, &source, &sourceSize);
    if (success != RESOURCES_SUCCESS) {
        fprintf(stderr, "Error: Reading shader file failed: %s\n", shaderPath);
        return 0;
    }
    const GLchar* const sourceContent = source;
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &sourceContent, NULL);
    glCompileShader(shader);
    freeFileContents(&source);
    if (!checkShaderCompileError(shader)) {
        return 0;
    }
    return shader;
}

void freeShader(GLuint shader) {
    glDeleteShader(shader);
}

GLuint createShaderProgram(const char* const vertexPath, const char* const fragmentPath) {
    GLuint vertexShader = createShader(vertexPath, GL_VERTEX_SHADER);
    if (vertexShader == 0) {
        fprintf(stderr, "Error: Creation of vertex shader failed: %s\n", vertexPath);
        return 0;
    }
    GLuint fragmentShader = createShader(fragmentPath, GL_FRAGMENT_SHADER);
    if (fragmentShader == 0) {
        fprintf(stderr, "Error: Creation of fragment shader failed: %s\n", fragmentPath);
        return 0;
    }
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    if (!checkProgramLinkError(program)) {
        return 0;
    }
    freeShader(vertexShader);
    freeShader(fragmentShader);
    return program;
}

void freeShaderProgram(GLuint program) {
    glDeleteProgram(program);
};