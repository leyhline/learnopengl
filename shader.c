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
    char* source;
    size_t sourceSize;
    int success = readFile(shaderPath, &source, &sourceSize);
    if (success != RESOURCES_SUCCESS) {
        return success;
    }
    const GLchar* const sourceContent = source;
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &sourceContent, NULL);
    glCompileShader(shader);
    free(source);
    checkShaderCompileError(shader);
    return shader;
}

GLuint createShaderProgram(const char* const vertexPath, const char* const fragmentPath) {
    GLuint vertexShader = createShader(vertexPath, GL_VERTEX_SHADER);
    GLuint fragmentShader = createShader(fragmentPath, GL_FRAGMENT_SHADER);
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    checkProgramLinkError(program);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}