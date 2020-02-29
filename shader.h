#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"

/// Create a shader from a GLSL file at `shaderPath`.
GLuint createShader(const char* shaderPath, GLenum shaderType);

/// Delete `shader` after it is linked in a shader program.
void freeShader(GLuint shader);

/// Create a shader program from a vertex shader file (`vertexPath`) and a fragment shader file (`fragmentPath`).
GLuint createShaderProgram(const char* vertexPath, const char* fragmentPath);

/// Delete `program`.
void freeShaderProgram(GLuint program);

#endif