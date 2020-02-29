#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"

GLuint createShader(const char* shaderPath, GLenum shaderType);
GLuint createShaderProgram(const char* vertexPath, const char* fragmentPath);

#endif